#!/usr/bin/env bash
set -euo pipefail

# this script helps in generating .wax files from a folder
# folder structure (eg)
#  apps/hello/
#  ├── WAX-MANIFEST
#  └── main.c
# contain a WAX-MANIFEST file like so:
# WAX-Version: 1.0
# App-Name: hello
# App-Version: 1.0.0
# Main-Target: lib/app.so
# Entry-Symbol: main

DIR="$1"
MAIN="$2"
OUT="$3"

CC="gcc"
CFLAGS="-O2 -Wall -Wextra -Wpedantic -fPIC -fstack-protector-strong -ffunction-sections -fdata-sections"
LDFLAGS="-Wl,--gc-sections -Wl,-z,relro -Wl,-z,now -Wl,--no-undefined"

BUILDR="$(cd .. && pwd)/build"
APPDIR="$BUILDR/apps"

GENNAME="${OUT%.wax}.wax"
PKGNAME="${OUT%%.*}"

PKGDIR="$APPDIR/$PKGNAME"
OBJDIR="$PKGDIR/lib"
GENCPY="$PKGDIR/$GENNAME"
BASE="${MAIN%.*}"

mkdir -p "$PKGDIR" "$OBJDIR"

$CC $CFLAGS $LDFLAGS -c "$DIR/$MAIN" -o "$OBJDIR/$BASE.o"
$CC -shared -o "$OBJDIR/app.so" "$OBJDIR/$BASE.o"

cp "$DIR/WAX-MANIFEST" "$PKGDIR/"
tar -cf "$GENCPY" -C "$PKGDIR" WAX-MANIFEST lib/app.so
