#!/usr/bin/env bash

(cp $(fd --type x . 'build/' -d 1 | paste -sd " ") ~/sysroot/bin/ && cd ~/sysroot && find . -mindepth 1 -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio)
