#!/bin/bash
set -

# for aarch64
# GCC_COMPILER=aarch64-linux-gnu

# for x86_64
GCC_COMPILER=x86_64-linux-gnu

CRTPATH=$(pwd)
EXE="main"

BUILD="$CRTPATH/build"
if [[ -d $BUILD ]]; then
  echo "Deleted the build folder."
  rm -rf $BUILD
fi

echo "Create the build folder."
mkdir -p $BUILD

cd $BUILD
cmake .. \
  -DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
  -DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++
cmake --build .

if [[ ! -f "$BUILD/$EXE" ]];then 
  echo "Failed in building binary executable."
  exit
fi

chmod +x "$BUILD/$EXE"
"$BUILD/$EXE"
