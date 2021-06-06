
/* Copyright 2011 Benjamin Tissoires <benjamin.tissoires@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define USB_VENDOR_ID_LOGITECH            (__u32)0x046D
#define USB_DEVICE_ID_UNIFYING_RECEIVER   (__s16)0xC52B
#define USB_DEVICE_ID_NANO_RECEIVER       (__s16)0xC52F
#define USB_DEVICE_ID_NANO_RECEIVER_ALT   (__s16)0xC534
#define USB_DEVICE_ID_UNIFYING_RECEIVER_2 (__s16)0xC532

int main(int argc, char *argv[])
{
	int fd, ret = EXIT_FAILURE;
	if (argc != 2) {
		errno = EINVAL;
		perror("No hidraw device given");
		exit(ret);
	}
	/* Open the Device with non-blocking. */
	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror("open() failed");
		exit(ret);
	}
	/* Get Raw Info */
	{
		struct hidraw_devinfo info;
		const int res = ioctl(fd, HIDIOCGRAWINFO, &info);
		if (res < 0)
			perror("error while getting info from device");
		else if (info.bustype != BUS_USB ||
				info.vendor != USB_VENDOR_ID_LOGITECH ||
				(info.product != USB_DEVICE_ID_UNIFYING_RECEIVER &&
				 info.product != USB_DEVICE_ID_NANO_RECEIVER &&
				 info.product != USB_DEVICE_ID_NANO_RECEIVER_ALT &&
				 info.product != USB_DEVICE_ID_UNIFYING_RECEIVER_2)) {
			errno = EPERM;
			perror("The given device is not a Logitech Unifying Receiver");
			goto end;
		}
	}
	/* Send the magic sequence to the Device */
	{
		static const unsigned char magic_seq[] = {
			0x10,
			0xFF,
			0x80,
			0xB2,
			0x01,
			0x00,
			0x00
		};
		const ssize_t res = write(fd, magic_seq, sizeof(magic_seq));
		if (res < 0)
			perror("write failed()");
		else if (res != sizeof(magic_seq)) {
			errno = ENOMEM;
			perror("write failed()");
			fprintf(stderr, "%ld were written instead of %lu.\n",
					res, sizeof(magic_seq));
		}
		else {
			puts("The receiver is ready to pair a new device.\n"
				 "Switch your device (off and) on to pair it.");
			ret = EXIT_SUCCESS;
		}
	}
end:
	close(fd);
	exit(ret);
}
