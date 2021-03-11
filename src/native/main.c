/*
 * Copyright (c) 2021 Reza Mahdi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <sys/utsname.h>

#include "uring.h"

static PyObject *supported(PyObject *self, PyObject *args) {
  struct utsname name;
  uname(&name);
  unsigned int major;
  unsigned int minor;
  unsigned int patch;
  sscanf(name.release, "%d.%d.%d", &major, &minor, &patch);

  if (major < 5 && minor < 6) return PyBool_FromLong(0);
  return PyBool_FromLong(1);
}

static PyMethodDef methods[] = {
    {"_supported", (PyCFunction)supported, METH_NOARGS,
     "Checks whether uring_io is supported or not"},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef uring_io_module = {
    PyModuleDef_HEAD_INIT, .m_name = "_uring_io",
    .m_doc = "low-level api of uring io", .m_size = -1, .m_methods = methods};

// The module init function
PyMODINIT_FUNC PyInit__uring_io(void) {
  PyObject *mod = PyModule_Create(&uring_io_module);
  register_ring(mod);
  register_sqe(mod);
  register_cqe(mod);

  return mod;
}
