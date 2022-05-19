#!/bin/bash
set -

rm -rf ./mapi.cpython-36m-x86_64-linux-gnu.so
rm -rf ./build ./dist ./*.egg-info

# you can copy the shared libraries to the default path for convenience
cp ./libinfer.so ./libmops.so /lib/x86_64-linux-gnu/

# build the Python package
python3 setup.py build_ext --inplace

# compile again for including the self-defined 
x86_64-linux-gnu-g++ \
  -pthread -shared -Wl,-O1 \
  -Wl,-Bsymbolic-functions \
  -Wl,-Bsymbolic-functions \
  -Wl,-z,relro \
  -Wl,-Bsymbolic-functions \
  -Wl,-z,relro -g -fstack-protector-strong -Wformat \
  -Werror=format-security \
  -Wdate-time -D_FORTIFY_SOURCE=2 \
  build/temp.linux-x86_64-3.6/api.o \
  -linfer -lmops \
  -o build/lib.linux-x86_64-3.6/mapi.cpython-36m-x86_64-linux-gnu.so
cp build/lib.linux-x86_64-3.6/mapi.cpython-36m-x86_64-linux-gnu.so ./

# instead that, you can export here as the LD_LIBRARY_PATH
# export LD_LIBRARY_PATH=.

# must export here as LD_LIBRARY_PATH
python3 -c "import mapi" 
python3 main.py

