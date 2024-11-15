from setuptools import setup
from wasmpy_build import build_ext
from Cython.Build import cythonize

setup(
    name='Hello world app',
    ext_modules=cythonize("test.py"),
    cmdclass={"build_ext": build_ext},
)