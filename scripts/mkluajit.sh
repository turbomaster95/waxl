#!/usr/bin/env bash
set -e

mkdir -p sysroot
SYSROOT_ABS="$(cd .. && pwd)/build/sysroot"
CFLAG="-nostdinc -nostdlib -isystem $SYSROOT_ABS/include -ffreestanding"
LDFLAG="-B$PWD/build/sysroot/lib -L$PWD/build/sysroot/lib -lc"

if [ ! -d luajit-src ]; then
    git clone --depth 1 https://github.com/LuaJIT/LuaJIT luajit-src;
fi

cd luajit-src

make -j$(nproc) CC="gcc $CFLAG $LDFLAG" HOST_CC="gcc"

make install PREFIX="$SYSROOT_ABS"

cd ..

cd sysroot/lib
if [ -f libluajit-5.1.a ] && [ ! -f libluajit.a ]; then
    ln -sf libluajit-5.1.a libluajit.a
fi

cd ../..
