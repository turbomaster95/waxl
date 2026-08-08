#!/usr/bin/env sh
set -x
set -eu

ARCH="$(uname -m)"
SYSROOT_ABS="$(cd .. && pwd)/build/sysroot"
OUTDIR="$SYSROOT_ABS/lib"
APKDIR="./apk"
TARGET_LIB="libgcc_eh.a"

mkdir -p sysroot "$APKDIR"

BASE="https://dl-cdn.alpinelinux.org/alpine"

PKG="gcc"

ALPINE_RELEASE='edge'
APK_ARCH='x86_64'

APK_FILENAME='gcc-15.2.0-r8.apk'
APK_URL="$BASE/$ALPINE_RELEASE/main/$APK_ARCH/$APK_FILENAME"


if [ ! -f "$APKDIR/$APK_FILENAME" ]; then
	curl -fSL "$APK_URL" -o "$APKDIR/$APK_FILENAME"
fi

rm -rf "$APKDIR/extract"
mkdir -p "$APKDIR/extract"
cd "$APKDIR/extract"
cp "../$APK_FILENAME" ./package.apk
tar -xzf package.apk

cd ../..

find "$APKDIR/extract" -type f -name "$TARGET_LIB" -print | while read -r f; do
  cp -f "$f" "$OUTDIR/"
done

COUNT="$(find "$OUTDIR" -type f -name "$TARGET_LIB" | wc -l | tr -d ' ')"
if [ "$COUNT" -eq 0 ]; then
  echo "Found no $TARGET_LIB inside gcc package extraction." >&2
  exit 1
fi


if [ ! -f "$OUTDIR/libgcc_musl_eh.a" ]; then
	mv "$OUTDIR/$TARGET_LIB" "$OUTDIR/libgcc_musl_eh.a"
fi
