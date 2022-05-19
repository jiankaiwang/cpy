# Interface between C++ and Python

In the following, we are going to solve the problem about the dependency of multiple dynamic libraries. The following is the scenario.

```text
              implemented
C++ Functions ----------> A dynamic library (A.so)
                               |
                               | referenced from
                               |
                               V
                          B dynamic library (B.so)
                               |
                               | a wrapper
                               |
                               V
Python APIs <---------- Wrapped by a Python interface
```

For many conditions, you probably get the shared library from a repository or a team. It's hard to get the source code. 
However, the runtime you target is on Python. This repository targets the condition.

## Using Cython

In the following, we start from using Cython. Fix the Python environment.

```sh
conda create --name py36 python=3.6
conda activate py36
```

Install the package for Cython. You should make sure the cooresponding Python version.

```sh
sudo apt update
sudo apt install -y python3.6-dev
```

### The calculating shared library

Next, we are going to build the dynamic libraries first. In the shared library, we define two simple calculations.

```cpp
// ...

namespace std {

int addNum(int a, int b) {
  return a + b;
}

int mulNum(int a, int b) {
  return a * b;
}

}

// ...
```

After that, we can build the shared library by the following commands. You will find it named `libmops.so` under the path `./build`.

```sh
cd ./libs
chmod +x ./build-Linux.sh
./build-Linux.sh
```

### The higher level calculating shared library

Next, we build the shared library based on the previous elementary one.
The first thing is to copy the `mops.hpp` under `./libs` and the `libmops.so` under the `./libs/build` to the path `./src`.
We now can build another shared library by using the previos `libmops.so`.

```cpp
#include <cstdlib>
#include "mops.hpp"

namespace std {

int add_mul(int a, int b) {
  // both the addNum, and the mulNum, are defined in libs
  return addNum(a, b) + mulNum(a, b);
}

}
```

The following is the data structure for building the shared library.

```text
+ src
  - infer.cpp
  - infer.hpp
  - mops.hpp     // the header file of mops
  - libmops.so   // the shared library of mops
  - main.cpp
  - ...
```

After that, we can build the second shared library that requires the `libmops.so`.

```sh
cp ./libs/mops.hpp ./libs/build/libmops.so ./src
cd ./src
chmod +x ./build-Linux.sh
./build-Linux.sh
```

### Wrap the C++ code to Python with Cython

In the final step, we are going to wrap the shared library of `libinfer.so` built from the previous step. The following is the simple example. You can define different types of interfaces, here, in `api.cc`.

```cpp
// ...

#include "infer.hpp"

// ...

PyObject* add_and_mul(PyObject* self, PyObject* args) {
    int v1, v2;

    if (! PyArg_ParseTuple(args, "ii", &v1, &v2)) {
        cout << "Error!" << endl;
        return NULL;
    }    

    int result = add_mul(v1, v2);
    return PyLong_FromLong(static_cast<long>(result));
}

// ...
```

It is recommended that you can copy the previous two shared library to the path of g++ tools, for example, `/lib/x86_64-linux-gnu/` or `/lib/aarch64-linux-gnu/`. Instead of putting the shared libraries to the GNU default path, you can use the `export LD_LIBRARY_PATH=.` to help find the shared libaries.

Next, you have to prepare the `setup.py` for compiling the shared library as a module for importing in Python.

```py
from setuptools import setup, Extension

sfc_module = Extension('mapi', sources = ["api.cc"])

setup(
    name='mapi',
    version='1.0',
    description='Python Package with the C++ extension',
    ext_modules=[sfc_module],
)
```

After you create the `setup.py`, you can build the Python package by using the command.

```sh
# build the Python package as the shared library
python3 setup.py build_ext --inplace

# you can also install directly
# python3 setup.py install
```

## Putting them together

We first create a docker image and run it. 

```sh
cd $HOME/Desktop
git clone https://github.com/jiankaiwang/cpy.git

cd ./cpy
docker build -t cpy:latest -f ./environ/Dockerfile .
docker run -it --rm -v $HOME/Desktop/cpy:/cpy/cpy cpy:latest bash
```

In the container, we build the base shared library, `libmops.so`.

```sh
cd /cpy/cpy/libs
./build-Linux.sh
```

Second, we build a shared library, `libinfer.so`, dependent on `libmops.so`.

```sh
cd /cpy/cpy/src

cp /cpy/cpy/libs/build/libmops.so /cpy/cpy/src
cp /cpy/cpy/libs/mops.hpp /cpy/cpy/src

./build-Linux.sh
```

In the final, we build the Python package. It requires both `libmops.so` and `libinfer.so`. 

```sh
cd /cpy/cpy/api

cp /cpy/cpy/libs/build/libmops.so /cpy/cpy/api
cp /cpy/cpy/libs/mops.hpp /cpy/cpy/api
cp /cpy/cpy/src/build/libinfer.so /cpy/cpy/api
cp /cpy/cpy/src/infer.hpp /cpy/cpy/api

./build-Linux-so.sh
```

After running the above command, you should see the output.

```text
add_mul(40, 50): 2090, valid: 1
```

Next run the following command to build the Python package.

```sh
./build-Linux-cpy.sh
```

### Troubleshooting

You should see `mapi.cpython-36m-x86_64-linux-gnu.so` generated. 
You can also use `ldd` to check whether the shared library available.

```sh
ldd mapi.cpython-36m-x86_64-linux-gnu.so
```

You should see two dependent shared libraries available.

```text
root@7c90707dca7b:/cpy/cpy/api# ldd ./mapi.cpython-36m-x86_64-linux-gnu.so 
        linux-vdso.so.1 (0x00007ffff6014000)
        libinfer.so => ./libinfer.so (0x00007fec1594b000)
        libmops.so => ./libmops.so (0x00007fec15749000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fec153c0000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fec151a1000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fec14db0000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fec14a12000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fec15d50000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fec147fa000)
```

