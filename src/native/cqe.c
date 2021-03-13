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

#include "uring.h"

int CQEInit(PyObject *self, PyObject *args, PyObject *kwds) {
  (void)args;
  (void)kwds;

  CQE *cqe = (CQE *)self;
  return 0;
}

PyObject *CQEGetData(PyObject *self, void *args) {
  (void)args;
  CQE *cqe = (CQE *)self;
  Py_INCREF(cqe->entry->user_data);
  return (PyObject *)(cqe->entry->user_data);
}

PyObject *CQEGetResult(PyObject *self, void *args) {
  (void)args;
  CQE *cqe = (CQE *)self;
  return PyLong_FromLong(cqe->entry->res);
}

PyObject *CQEGetFlags(PyObject *self, void *args) {
  (void)args;
  CQE *cqe = (CQE *)self;
  return PyLong_FromLong(cqe->entry->flags);
}

int CQESetter(PyObject *self, PyObject *val, void *enc) {
  (void)self;
  (void)val;
  (void)enc;
  PyErr_SetString(PyExc_TypeError, "CQE object is immutable");
  return -1;
}

void CQEDestructor(void *self) { CQE *cqe = (CQE *)self; }

static PyGetSetDef cqe_getset[] = {
    {"data", CQEGetData, CQESetter, "User data of CQE", NULL},
    {"result", CQEGetResult, CQESetter, "Result of operation", NULL},
    {"flags", CQEGetFlags, CQESetter, "Flags of operation", NULL},
    {NULL, NULL, NULL, NULL, NULL}};

static PyMethodDef cqe_methods[] = {{NULL, NULL, 0, NULL}};

PyTypeObject cqe_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0) "_uring_io.CQE", /* tp_name */
    sizeof(CQE),                                            /* tp_basicsize */
    0,                                                      /* tp_itemsize */
    (destructor)CQEDestructor,                              /* tp_dealloc */
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
    "Uring IO Completation Queue Entry",                    /* tp_doc */
    (traverseproc)NULL,                                     /* tp_traverse */
    (inquiry)NULL,                                          /* tp_clear */
    0,                                                      /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    cqe_methods,         /* tp_methods */
    0,                   /* tp_members */
    cqe_getset,          /* tp_getset */
    0,                   /* tp_base */
    0,                   /* tp_dict */
    0,                   /* tp_descr_get */
    0,                   /* tp_descr_set */
    0,                   /* tp_dictoffset */
    CQEInit,             /* tp_init */
    PyType_GenericAlloc, /* tp_alloc */
    NULL,                /* tp_new */
};

extern void register_cqe(PyObject *mod) {
  Py_INCREF(&cqe_type);
  if (PyModule_AddObject(mod, "CQE", (PyObject *)&cqe_type) < 0)
    Py_DECREF(&cqe_type);
}