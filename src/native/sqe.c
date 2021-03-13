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

#include "uring.h"

int SQEInit(PyObject *self, PyObject *args, PyObject *kwds) {
  (void)args;
  (void)kwds;
  SQE *sqe = (SQE *)self;
  return 0;
}

void SQEDestructor(void *self) { SQE *sqe = (SQE *)self; }

/////////////////////// opcode
int SQESetOC(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "opcode must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->opcode = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetOC(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->opcode);
}

/////////////////////// flags
int SQESetFlags(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "flags must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->flags = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetFlags(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->flags);
}

/////////////////////// ioprio
int SQESetIOPrio(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "ioprio must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->ioprio = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetIOPrio(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->ioprio);
}

/////////////////////// fd
int SQESetFD(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "fd must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->fd = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetFD(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->fd);
}

/////////////////////// off
int SQESetOff(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "off must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->off = PyLong_AsLongLong(args);
  return 0;
}
PyObject *SQEGetOff(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->off);
}

/////////////////////// len
int SQESetLen(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "len must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->len = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetLen(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->len);
}

/////////////////////// op_flags
int SQESetOpFlags(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "op_flags must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->rw_flags = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetOpFlags(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->rw_flags);
}

/////////////////////// data
int SQESetData(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  Py_DECREF(sqe->entry->user_data);
  sqe->entry->user_data = (__u64)args;
  Py_INCREF(args);
  return 0;
}
PyObject *SQEGetData(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return (PyObject *)sqe->entry->user_data;
}

/////////////////////// buf_index
int SQESetBufIndex(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "buf_index must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->buf_index = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetBufIndex(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->buf_index);
}

/////////////////////// buf_group
int SQESetBufGroup(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "buf_group must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->buf_group = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetGroup(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->buf_group);
}

/////////////////////// flags
int SQESetPersonality(PyObject *self, PyObject *args, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  if (!PyLong_Check(args)) {
    PyErr_Format(PyExc_TypeError, "personality must be an 'int'. got '%s'",
                 args->ob_type->tp_name);
    return -1;
  }
  sqe->entry->personality = PyLong_AsLong(args);
  return 0;
}
PyObject *SQEGetPersonality(PyObject *self, void *enc) {
  (void)enc;
  SQE *sqe = (SQE *)self;
  return PyLong_FromLong(sqe->entry->personality);
}

static PyGetSetDef sqe_setget[] = {
    {"opcode", SQEGetOC, SQESetOC, "Opcode", NULL},
    {"flags", SQEGetFlags, SQESetFlags, "Flags of SQE", NULL},
    {"ioprio", SQEGetIOPrio, SQESetIOPrio, "IO priority", NULL},

    {"fd", SQEGetFD, SQESetFD, "fd to do the IO", NULL},

    {"off", SQEGetOff, SQESetOff, "Offset into file", NULL},
    //{"addr2", NULL, NULL, "addr2", NULL},

    {"len", SQEGetLen, SQESetLen, "Length of buffer or number of iovecs", NULL},

    {"op_flags", SQEGetOpFlags, SQESetOpFlags, "fd to do the IO", NULL},

    {"data", SQEGetData, SQESetData, "User data of SQE", NULL},

    {"buf_index", SQEGetBufIndex, SQESetBufIndex, "Buffer index", NULL},
    {"buf_group", SQEGetBufIndex, SQESetBufGroup, "Buffer group", NULL},

    {"personality", SQEGetPersonality, SQESetPersonality, "Personality", NULL},
    {NULL, NULL, NULL, NULL, NULL}};

static PyMethodDef sqe_methods[] = {{NULL, NULL, 0, NULL}};

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
    0,                   /* tp_members */
    sqe_setget,          /* tp_getset */
    0,                   /* tp_base */
    0,                   /* tp_dict */
    0,                   /* tp_descr_get */
    0,                   /* tp_descr_set */
    0,                   /* tp_dictoffset */
    SQEInit,             /* tp_init */
    PyType_GenericAlloc, /* tp_alloc */
    NULL,                /* tp_new */
};

extern void register_sqe(PyObject *mod) {
  Py_INCREF(&sqe_type);
  if (PyModule_AddObject(mod, "SQE", (PyObject *)&sqe_type) < 0)
    Py_DECREF(&sqe_type);
}