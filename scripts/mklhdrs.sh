#!/usr/bin/env bash
set -e

mkdir -p sysroot
SYSROOT_ABS="$(cd .. && pwd)/build/sysroot"

if [ ! -d linux-7.1.6 ]; then
	rsync -av --progress rsync://rsync.kernel.org/pub/linux/kernel/v7.x/linux-7.1.6.tar.xz .
	tar xvf linux-7.1.6.tar.xz
fi

cd linux-7.1.6

make headers

make INSTALL_HDR_PATH="$SYSROOT_ABS" headers_install

cd ..
