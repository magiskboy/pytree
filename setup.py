from setuptools.extension import Extension
from setuptools import (
    setup,
    find_packages,
)


heap_mod = Extension("heap", sources=["src/pytree/Py_heap.c"])

setup(
    name="pytree",
    version="0.0.1",
    description="Tree base datastructure for Python",
    author="Nguyen Khac Thanh",
    author_email="nguyenkhacthanh244@gmail.com",
    url="https://github.com/magiskboy/pytree",
    ext_modules=[heap_mod,],
    packages=find_packages("src", exclude=["test*", "examples"]),
    package_dir={"": "src"},
    license="MIT",
    python_requires=">=3.6",
)
