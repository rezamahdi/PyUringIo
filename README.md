# PyUringIO

A asyncio loop based on liburing

## Table of Contents

1. [About the Project](#about-the-project)
1. [Project Status](#project-status)
1. [Getting Started](#getting-started)
	1. [Building](#building)
	1. [Installation](#installation)
1. [Release Process](#release-process)
	1. [Versioning](#versioning)
1. [Contributing](#contributing)
1. [License](#license)
1. [Author](#author)

# About the Project

this project implements a liburing based asyncio event loop for faster io. it is fully compatible with
asyncio built-in loop. here is a simple example:

```python
import asyncio
from uring_io import UringIOEventLoop

loop = UringIOEventLoop()

async def main():
    await loop.sleep(2)

loop.run_untile_complete(main())
```

**[Back to top](#table-of-contents)**

# Project Status

this project is under develope and is not fully stable for production. by the way it's underlying infrastructure seems
to become stable.

keep in mind that this implementation is not official and whenever uring-io become fully mature, python community
may decide to implement it themselve.

**[Back to top](#table-of-contents)**

# Getting Started

## Getting the Source

Include a link to your GitHub repository (you have no idea how people will find your code), and also a summary of how to clone.

This project is [hosted on GitHub](https://github.com/rezamahdi/PyUringIO). You can clone this project directly using this command:

```
git clone https://github.com/rezamahdi/PyUringIO
```

## Building

this project is a python package and like a (traditional) normal package, needs setuptools to build.
but since it have some C underling codes, it needs a C99 compiler, cmake and cmaketools python package.
regradless of building, you must have liburing installed also.

## Installation

After building, you can use setup.py script to install package.
It is available in PyPI too so you can use pip to install it.

# Release Process

since uring-io is a kernel level technology, it doesn't change frequently. so changes to this project is due tiny
improvements.

whenever there is a new critical change, I will release a new package.

## Versioning

This project uses [Semantic Versioning](http://semver.org/). For a list of available versions, see the [repository tag list](https://github.com/your/project/tags).

# Contributing

We encourage public contributions! Please review [CONTRIBUTING.md](docs/CONTRIBUTING.md) for details on our code of conduct and development process.

**[Back to top](#table-of-contents)**

# License

Copyright (c) 2021 Reza Mahdi

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

**[Back to top](#table-of-contents)**

# Author

* **[Reza Mahdi](https://github.com/rezamahdi)** - *Initial work* 

See also the list of [contributors](https://github.com/rezamahdi/PyUringIO/contributors)
who participated in this project.

**[Back to top](#table-of-contents)**
