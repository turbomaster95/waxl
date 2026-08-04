#!/usr/bin/env bash
set -e

mkdir -p sysroot

if [ ! -d musl-src ]; then
    git clone --depth 1 https://github.com/turbomaster95/musl-libc musl-src;
fi

cd musl-src

SYSROOT_ABS="$(cd .. && pwd)/sysroot"

./configure --prefix="$SYSROOT_ABS" --enable-shared --enable-static
make -j$(nproc)
make install

cd ..

cd sysroot/lib
if [ -f libc.so ]; then
    cp libc.so ld-musl-x86_64.so.1
fi
