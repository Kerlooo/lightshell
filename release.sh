#!/usr/bin/env bash
set -e

make clean
mkdir -p dist
make
mv lightshell dist/

cd dist
sha256sum lightshell > lightshell.sha256
md5sum lightshell > lightshell.md5
