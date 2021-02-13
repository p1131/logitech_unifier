Logitech Unifier
================

A program to pair devices to a Logitech Unifying Receiver on Linux. Also works
for Nano Receiver (Non-unifying).

Logitech only provides a Windows program to do this, so if you're running
Linux, you either need to pair your devices on a Windows or use the `unify.c`
program.

Usage
-----

1. Compile it:
```
gcc -o unify unify.c
```

2. Find your hidraw device. Run the following for every `hidrawX` on your
   system until you find `HID_NAME=Logitech USB Receiver`:
```
cat /sys/class/hidraw/hidraw0/device/uevent
```

 If `hidraw0` isn't it, try `hidraw1` and so on. If you have multiple `hidrawX`
 devices that match, try all of them.

3. Pair it:
```
sudo ./unify /dev/hidrawX
```

 Where `X` is the matching number from the previous step.

4. Turn on your device like the tool will say and that's it! Device should work
   now.

Thanks
------

To Tycho for this article:
http://tycho.ws/blog/2011/12/logitech-unifying-receiver

And Benjamin for writing the program:
https://groups.google.com/forum/?fromgroups#!msg/linux.kernel/zYS6yddI8yU/9cMvg3k9xTYJ

COPYING
-------

	Copyright 2011 Benjamin Tissoires <benjamin.tissoires@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
