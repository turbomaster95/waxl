#!/usr/bin/bash

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


DIR='$1'
OUT='$2'
CC='gcc'
CFLAGS='

mkdir -p staging/lib
cp ../apps/hello/WAX-MANIFEST staging/ 
cp hello.so staging/lib/app.so
tar -cf hello.wax -C staging WAX-MANIFEST lib/app.so 
rm -rf staging
