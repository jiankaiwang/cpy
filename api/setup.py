from setuptools import setup, Extension

sfc_module = Extension('mapi' 
                       , sources = ["api.cc"]
                       , extra_compile_args= ["-linfer", "-lmops"])

setup(
    name='mapi',
    version='1.0',
    description='Python Package with the C++ extension',
    ext_modules=[sfc_module],
)