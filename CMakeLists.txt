cmake_minimum_required(VERSION 3.12)
project(asyncio_uring)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# when installing directly in CMake, redirect its intall root to dist dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_SOURCE_DIR}/dist"
      CACHE PATH "default install path" FORCE)
endif()

find_package(
  Python3
  COMPONENTS Interpreter Development
  REQUIRED)
find_package(Liburing REQUIRED)

# get the suffix for python extension module and store in
# PYTHON_MODULE_EXTENSION
execute_process(
  COMMAND
    "${Python3_EXECUTABLE}" -c
    "from distutils import sysconfig;print(sysconfig.get_config_var('EXT_SUFFIX'))"
  RESULT_VARIABLE _result
  OUTPUT_VARIABLE PYTHON_MODULE_EXTENSION
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

add_subdirectory(src)

