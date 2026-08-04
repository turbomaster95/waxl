#!/usr/bin/env bash
set -e

mkdir -p sysroot
SYSROOT_ABS="$(cd .. && pwd)/sysroot"

if [ ! -d linux-src ]; then
    git clone --depth 1 --single-branch https://github.com/torvalds/linux.git linux-src
fi

cd linux-src

make headers

make INSTALL_HDR_PATH="$SYSROOT_ABS" headers_install

cd ..
