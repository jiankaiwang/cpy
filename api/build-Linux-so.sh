#!/bin/bash
set -

ARCH=$(uname -a)
if [[ $ARCH =~ 'x86_64' ]]; then
  GCC_COMPILER=x86_64-linux-gnu
  # GCC_LIBS=/lib/x86_64-linux-gnu/
elif [[ $ARCH =~ 'aarch64' ]]; then
  GCC_COMPILER=aarch64-linux-gnu
  # GCC_LIBS=/lib/aarch64-linux-gnu/
elif [[ $ARCH =~ 'arm64' ]]; then
  # for MacOS, we use the default g++ compiler
  GCC_COMPILER=llvm
  # GCC_LIBS=/Library/Developer/CommandLineTools/usr/lib
else
  echo "Error: Can't find the proper compiler."
  exit 1
fi

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

