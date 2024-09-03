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

#include <liburing.h>
#include <liburing/io_uring.h>
#include <stdio.h>
#include <sys/utsname.h>

#include "uring.h"

static PyObject *supported(PyObject *self, PyObject *args) {
  (void)self;
  (void)args;

  struct utsname name;
  uname(&name);
  unsigned int major;
  unsigned int minor;
  unsigned int patch;
  sscanf(name.release, "%d.%d.%d", &major, &minor, &patch);

  if (major < 5 && minor < 6) return PyBool_FromLong(0);
  return PyBool_FromLong(1);
}

static PyObject *probe(PyObject *self, PyObject *args) {
  struct io_uring_probe *pr = io_uring_get_probe();

  return PyLong_FromLong(pr->last_op);
}

static PyObject *probe_oc(PyObject *self, PyObject *args) {
  struct io_uring_probe *pr = io_uring_get_probe();
  int opcode;
  if (!PyArg_ParseTuple(args, "I", &opcode)) return NULL;

  return PyBool_FromLong(io_uring_opcode_supported(pr, opcode));
}

static PyMethodDef methods[] = {
    {"supported", (PyCFunction)supported, METH_NOARGS,
     "Checks whether uring_io is supported or not"},
    {"probe", (PyCFunction)probe, METH_NOARGS, "Probe supported opcodes"},
    {"opcode_supported", (PyCFunction)probe_oc, METH_VARARGS,
     "Checks whether opcode is supported"},
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

  PyObject *flags_mod = PyModule_New("flags");
  PyObject *opcodes_mod = PyModule_New("opcodes");

  PyModule_AddIntConstant(flags_mod, "SQE_FIXED_FILE", IOSQE_FIXED_FILE);
  PyModule_AddIntConstant(flags_mod, "SQE_IO_DRAIN", IOSQE_IO_DRAIN);
  PyModule_AddIntConstant(flags_mod, "SQE_IO_LINK", IOSQE_IO_LINK);
  PyModule_AddIntConstant(flags_mod, "SQE_IO_HARDLINK", IOSQE_IO_HARDLINK);
  PyModule_AddIntConstant(flags_mod, "SQE_ASYNC", IOSQE_ASYNC);
  PyModule_AddIntConstant(flags_mod, "SQE_BUFFER_SELECT", IOSQE_BUFFER_SELECT);

  PyModule_AddIntConstant(flags_mod, "RING_SETUP_IOPOLL", IORING_SETUP_IOPOLL);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_SQPOLL", IORING_SETUP_SQPOLL);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_SQ_AFF", IORING_SETUP_SQ_AFF);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_CQSIZE", IORING_SETUP_CQSIZE);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_CLAMP", IORING_SETUP_CLAMP);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_ATTACH_WQ",
                          IOSQE_BUFFER_SELECT);
  PyModule_AddIntConstant(flags_mod, "RING_SETUP_R_DISABLED",
                          IORING_SETUP_R_DISABLED);

  PyModule_AddIntConstant(opcodes_mod, "OP_NOP", IORING_OP_NOP);
  PyModule_AddIntConstant(opcodes_mod, "OP_READV", IORING_OP_READV);
  PyModule_AddIntConstant(opcodes_mod, "OP_WRITEV", IORING_OP_WRITEV);
  PyModule_AddIntConstant(opcodes_mod, "OP_FSYNC", IORING_OP_FSYNC);
  PyModule_AddIntConstant(opcodes_mod, "OP_READ_FIXED", IORING_OP_READ_FIXED);
  PyModule_AddIntConstant(opcodes_mod, "OP_WRITE_FIXED", IORING_OP_WRITE_FIXED);
  PyModule_AddIntConstant(opcodes_mod, "OP_POLL_ADD", IORING_OP_POLL_ADD);
  PyModule_AddIntConstant(opcodes_mod, "OP_POLL_REMOVE", IORING_OP_POLL_REMOVE);
  PyModule_AddIntConstant(opcodes_mod, "OP_SYNC_FILE_RANGE",
                          IORING_OP_SYNC_FILE_RANGE);
  PyModule_AddIntConstant(opcodes_mod, "OP_SENDMSG", IORING_OP_SENDMSG);
  PyModule_AddIntConstant(opcodes_mod, "OP_RECVMSG", IORING_OP_RECVMSG);
  PyModule_AddIntConstant(opcodes_mod, "OP_TIMEOUT", IORING_OP_TIMEOUT);
  PyModule_AddIntConstant(opcodes_mod, "OP_TIMEOUT_REMOVE",
                          IORING_OP_TIMEOUT_REMOVE);
  PyModule_AddIntConstant(opcodes_mod, "OP_ACCEPT", IORING_OP_ACCEPT);
  PyModule_AddIntConstant(opcodes_mod, "OP_ASYNC_CANCEL",
                          IORING_OP_ASYNC_CANCEL);
  PyModule_AddIntConstant(opcodes_mod, "OP_LINK_TIMEOUT",
                          IORING_OP_LINK_TIMEOUT);
  PyModule_AddIntConstant(opcodes_mod, "OP_CONNECT", IORING_OP_CONNECT);
  PyModule_AddIntConstant(opcodes_mod, "OP_FALLOCATE", IORING_OP_FALLOCATE);
  PyModule_AddIntConstant(opcodes_mod, "OP_OPENAT", IORING_OP_OPENAT);
  PyModule_AddIntConstant(opcodes_mod, "OP_CLOSE", IORING_OP_CLOSE);
  PyModule_AddIntConstant(opcodes_mod, "OP_FILES_UPDATE",
                          IORING_OP_FILES_UPDATE);
  PyModule_AddIntConstant(opcodes_mod, "OP_STATX", IORING_OP_STATX);
  PyModule_AddIntConstant(opcodes_mod, "OP_READ", IORING_OP_READ);
  PyModule_AddIntConstant(opcodes_mod, "OP_WRITE", IORING_OP_WRITE);
  PyModule_AddIntConstant(opcodes_mod, "OP_FADVISE", IORING_OP_FADVISE);
  PyModule_AddIntConstant(opcodes_mod, "OP_MADVISE", IORING_OP_MADVISE);
  PyModule_AddIntConstant(opcodes_mod, "OP_SEND", IORING_OP_SEND);
  PyModule_AddIntConstant(opcodes_mod, "OP_RECV", IORING_OP_RECV);
  PyModule_AddIntConstant(opcodes_mod, "OP_OPENAT2", IORING_OP_OPENAT2);
  PyModule_AddIntConstant(opcodes_mod, "OP_EPOLL_CTL", IORING_OP_EPOLL_CTL);
  PyModule_AddIntConstant(opcodes_mod, "OP_SPLICE", IORING_OP_SPLICE);
  PyModule_AddIntConstant(opcodes_mod, "OP_PROVIDE_BUFFERS",
                          IORING_OP_PROVIDE_BUFFERS);
  PyModule_AddIntConstant(opcodes_mod, "OP_REMOVE_BUFFERS",
                          IORING_OP_REMOVE_BUFFERS);
  PyModule_AddIntConstant(opcodes_mod, "OP_TEE", IORING_OP_TEE);
  PyModule_AddIntConstant(opcodes_mod, "OP_SHUTDOWN", IORING_OP_SHUTDOWN);
  PyModule_AddIntConstant(opcodes_mod, "OP_RENAMEAT", IORING_OP_RENAMEAT);
  PyModule_AddIntConstant(opcodes_mod, "OP_ULINKAT", IORING_OP_UNLINKAT);
  PyModule_AddIntConstant(opcodes_mod, "OP_MKDIRAT", IORING_OP_MKDIRAT);
  PyModule_AddIntConstant(opcodes_mod, "OP_LAST", IORING_OP_LAST);

  PyModule_AddObject(mod, "opcodes", opcodes_mod);
  PyModule_AddObject(mod, "flags", flags_mod);

  return mod;
}
