#!/usr/bin/env bash
set -e

mkdir -p sysroot
SYSROOT_ABS="$(cd .. && pwd)/build/sysroot"

if [ ! -d luajit-src ]; then
    git clone --depth 1 https://github.com/LuaJIT/LuaJIT luajit-src;
fi

cd luajit-src

make -j$(nproc)

make install PREFIX="$SYSROOT_ABS"

cd ..

cd sysroot/lib
if [ -f libluajit-5.1.a ] && [ ! -f libluajit.a ]; then
    ln -sf libluajit-5.1.a libluajit.a
fi

cd ../..
