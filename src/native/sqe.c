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

#include "uring.h"

int SQEInit(PyObject *self, PyObject *args, PyObject *kwds) {
  SQE *sqe = (SQE *)self;
  sqe->flags = PyDict_New();

  PyObject *fixed_file = PyLong_FromLong(IOSQE_FIXED_FILE);
  PyObject *io_drain = PyLong_FromLong(IOSQE_IO_DRAIN);
  PyObject *io_link = PyLong_FromLong(IOSQE_IO_LINK);
  PyObject *io_hardlink = PyLong_FromLong(IOSQE_IO_HARDLINK);
  PyObject *async = PyLong_FromLong(IOSQE_ASYNC);
  PyObject *buffer_select = PyLong_FromLong(IOSQE_BUFFER_SELECT);

  PyDict_SetItemString(sqe->flags, "FIXED_FILE", fixed_file);
  PyDict_SetItemString(sqe->flags, "IO_DRAIN", io_drain);
  PyDict_SetItemString(sqe->flags, "IO_LINK", io_link);
  PyDict_SetItemString(sqe->flags, "IO_HARDLINK", io_hardlink);
  PyDict_SetItemString(sqe->flags, "ASYNC", async);
  PyDict_SetItemString(sqe->flags, "BUFFER_SELECT", buffer_select);

  return 0;
}

void SQEDestructor(void *self) { SQE *sqe = (SQE *)self; }

PyObject *SQESetFD(PyObject *self, PyObject *args) {
  SQE *sqe = (SQE *)self;
  PyArg_ParseTuple(args, "l", &sqe->entry->fd);
  return Py_None;
}

PyObject *SQESetOC(PyObject *self, PyObject *args) {
  SQE *sqe = (SQE *)self;
  PyArg_ParseTuple(args, "l", &sqe->entry->opcode);
  return Py_None;
}

PyObject *SQESetFlags(PyObject *self, PyObject *args) {
  SQE *sqe = (SQE *)self;
  PyArg_ParseTuple(args, "l", &sqe->entry->flags);
  return Py_None;
}

static PyMemberDef sqe_members[] = {
    {"flags", T_OBJECT, sizeof(PyObject) + sizeof(struct io_uring_sqe), 1,
     "Flags of SQE"},
    {NULL, 0, 0, 0, NULL}};

static PyMethodDef sqe_methods[] = {
    {"set_fd", SQESetFD, METH_VARARGS, "Prepare for NOP"},
    {"set_opcode", SQESetOC, METH_NOARGS, "Set opcode"},
    {"set_flags", SQESetFlags, METH_NOARGS, "Set flags"},
    {NULL, NULL, 0, NULL}};

PyTypeObject sqe_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0) "_uring_io.SQE", /* tp_name */
    sizeof(SQE),                                            /* tp_basicsize */
    0,                                                      /* tp_itemsize */
    (destructor)SQEDestructor,                              /* tp_dealloc */
    0,                                                      /* tp_print */
    0,                                                      /* tp_getattr */
    0,                                                      /* tp_setattr */
    0,                                                      /* tp_reserved */
    (reprfunc)NULL,                                         /* tp_repr */
    0,                                                      /* tp_as_number */
    0,                                                      /* tp_as_sequence */
    0,                                                      /* tp_as_mapping */
    0,                                                      /* tp_hash */
    0,                                                      /* tp_call */
    0,                                                      /* tp_str */
    0,                                                      /* tp_getattro */
    0,                                                      /* tp_setattro */
    0,                                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                     /* tp_flags */
    "Uring IO Submission Queue Entry",                      /* tp_doc */
    (traverseproc)NULL,                                     /* tp_traverse */
    (inquiry)NULL,                                          /* tp_clear */
    0,                                                      /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    sqe_methods,         /* tp_methods */
    sqe_members,         /* tp_members */
    0,                   /* tp_getset */
    0,                   /* tp_base */
    0,                   /* tp_dict */
    0,                   /* tp_descr_get */
    0,                   /* tp_descr_set */
    0,                   /* tp_dictoffset */
    SQEInit,             /* tp_init */
    PyType_GenericAlloc, /* tp_alloc */
    PyType_GenericNew,   /* tp_new */
};

extern void register_sqe(PyObject *mod) {
  Py_INCREF(&sqe_type);
  if (PyModule_AddObject(mod, "SQE", (PyObject *)&sqe_type) < 0)
    Py_DECREF(&sqe_type);
}