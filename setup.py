from cmaketools import setup

setup(
    name="asyncio_uring",
    version="0.0.1",
    author="Reza Mahdi",
    author_email="rmahdi.develop@gmail.com",
    description="asyncio loop based on liburing for CPython",
    url="https://github.com/python-cmaketools/cpython-example.git",
    license="MIT License",
    src_dir="src",
    ext_module_hint=r"Python3_add_library",
    has_package_data=False,
    install_requires=["cmaketools"]
)
