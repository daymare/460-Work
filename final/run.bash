#!/bin/bash

qemu-system-arm -M versatilepb -m 256M -sd sdimage -kernel wanix \
-serial mon:stdio

