#!/bin/bash

#
# Copyright 2020 Dragos R. Rugescu aka empyrion
#

PROJECT_NAME="JWT_PRACTICE"

if [ "$(whoami)" = "root" ]; then
  echo "  Do not run as root..."
  exit
fi

REPO=$(dirname "$0")
rm -rf "$REPO"/out
rm -rf "$REPO"/build
mkdir -p "$REPO"/build && mkdir -p "$REPO"/lib && cd "$REPO"/l8w8jwt || exit

# Replace Shared libraries On with Off to build static .a required
echo "Preparing static library build..."
touch build2.sh
sed '/cmake -DBUILD_SHARED_LIBS/','/On/s/On/Off/' build.sh > build2.sh

# Build libraries
echo "Building libraries..."
source build2.sh

# Copy to our lib
pwd
cp -r ./mbedtls/library/*.a ../../lib/
cp ./l8w8jwt/bin/release/libl8w8jwt.a ../../lib/

# Now build our programs
cd ../../build
cmake ..
make