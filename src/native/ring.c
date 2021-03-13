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
#include <string.h>
#include <time.h>

#include "uring.h"

extern PyTypeObject sqe_type;
extern PyTypeObject cqe_type;

static char *ring_init_kwds[] = {
    "entries",        "sq_entries", "cq_entries", "flags", "sq_thread_cpu",
    "sq_thread_idle", "features",   "wq_fd",      NULL,
};

/* Initializer of Ring objects */
int RingInit(PyObject *self, PyObject *args, PyObject *kwds) {
  Ring *ring = (Ring *)self;
  struct io_uring_params params;
  unsigned int entries;

  memset(&params, 0, sizeof(params));

  if (!PyArg_ParseTupleAndKeywords(
          args, kwds, "I|$IIIIIII", ring_init_kwds, &entries,
          &params.sq_entries, &params.cq_entries, &params.flags,
          &params.sq_thread_cpu, &params.sq_thread_idle, &params.features,
          &params.wq_fd)) {
    Py_INCREF(self);
    return -1;
  }

  if (params.sq_entries < 1) params.sq_entries = entries;

  if (params.cq_entries < 1) params.cq_entries = entries;

  int err = io_uring_queue_init_params(entries, &ring->ring, &params);
  if (err != 0) {
    Py_INCREF(self);
    PyErr_SetString(PyExc_RuntimeError, strerror(-err));
    return -1;
  }
  ring->entries = PyLong_FromLong(entries);
  return 0;
}

/* Get a single SQE */
PyObject *RingGetSQE(PyObject *self, PyObject *args) {
  (void)args;
  Ring *ring = (Ring *)self;
  SQE *sqe = PyObject_New(SQE, &sqe_type);

  struct io_uring_sqe *s = io_uring_get_sqe(&ring->ring);

  if (s == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "Submission queue full");
    return NULL;
  }

  sqe->entry = s;
  sqe->entry->user_data = (__u64)Py_None;
  Py_INCREF(Py_None);
  return PyObject_Init((PyObject *)sqe, &sqe_type);
}

/* Submit a ring*/
PyObject *RingSubmit(PyObject *self, PyObject *args) {
  (void)args;
  Ring *ring = (Ring *)self;

  int num = io_uring_submit(&ring->ring);

  return PyLong_FromLong(num);
}

/* Submit a ring and wait*/
PyObject *RingSubmitAndWait(PyObject *self, PyObject *args) {
  Ring *ring = (Ring *)self;
  unsigned int count;

  if (!PyArg_ParseTuple(args, "I", &count)) return NULL;

  int err = io_uring_submit_and_wait(&ring->ring, count);

  if (err != 0) {
    PyErr_SetString(PyExc_RuntimeError, strerror(-err));
    return NULL;
  }

  Py_RETURN_NONE;
}

/* Wait for a completation */
PyObject *RingWaitForCQE(PyObject *self, PyObject *args) {
  (void)args;
  Ring *ring = (Ring *)self;
  CQE *cqe = PyObject_New(CQE, &cqe_type);
  cqe->entry=NULL;
  PyObject_Init((PyObject *)cqe, &cqe_type);

  int err = io_uring_wait_cqe(&ring->ring, &cqe->entry);
  if (err != 0) {
    PyErr_SetString(PyExc_RuntimeError, strerror(-err));
    return NULL;
  }

  if(cqe->entry == NULL) {
    Py_RETURN_NONE;
  }

  return (PyObject *)cqe;
}

/* Wait for a specific count of complementations */
PyObject *RingWaitForCQENr(PyObject *self, PyObject *args) {
  Ring *ring = (Ring *)self;
  struct io_uring_cqe *cqe_list;
  unsigned int count;

  if (!PyArg_ParseTuple(args, "I", &count)) return NULL;
  if (!io_uring_wait_cqe_nr(&ring->ring, &cqe_list, count)) {
    PyErr_BadArgument();
    return NULL;
  }

  PyObject *list = PyList_New(count);
  for (int i = 0; i < count; i++) {
    CQE *cqe = PyObject_New(CQE, &cqe_type);
    PyObject_Init((PyObject *)cqe, &cqe_type);
    cqe->entry = &cqe_list[i];
    PyList_SetItem(list, i, (PyObject *)cqe);
  }
  return list;
}

/* Wait for a specific count of complementations with a timeout */
PyObject *RingWaitForCQETO(PyObject *self, PyObject *args) {
  Ring *ring = (Ring *)self;
  CQE *cqe = PyObject_New(CQE, &cqe_type);
  PyObject_Init((PyObject *)cqe, &cqe_type);
  struct __kernel_timespec ts = {.tv_nsec = 0, .tv_sec = 0};

  cqe->entry = NULL;

  if (!PyArg_ParseTuple(args, "II", &ts.tv_sec, &ts.tv_nsec)) return NULL;

  if (!io_uring_wait_cqe_timeout(&ring->ring, &cqe->entry, &ts)) {
    PyErr_BadArgument();
    return NULL;
  }

  if (cqe->entry == NULL) {
    Py_DECREF(cqe);
    Py_RETURN_NONE;
  }

  return (PyObject *)cqe;
}

/* Peek a single complementation from CQ */
PyObject *RingPeekCQE(PyObject *self, PyObject *args) {
  (void)args;
  Ring *ring = (Ring *)self;
  CQE *cqe = PyObject_New(CQE, &cqe_type);
  PyObject_Init((PyObject *)cqe, &cqe_type);

  cqe->entry = NULL;

  int err = io_uring_peek_cqe(&ring->ring, &cqe->entry);

  if (err != 0) {
    PyErr_SetString(PyExc_RuntimeError, strerror(-err));
    return NULL;
  }

  if (cqe->entry == NULL) {
    Py_DECREF(cqe);
    Py_RETURN_NONE;
  }

  return (PyObject *)cqe;
}

/* Peek a batch of complementations from CQ */
PyObject *RingPeekCQEBatch(PyObject *self, PyObject *args) {
  Ring *ring = (Ring *)self;
  struct io_uring_cqe *cqe_list;
  unsigned int count;

  if (!PyArg_ParseTuple(args, "I", &count)) return NULL;
  count = io_uring_peek_batch_cqe(&ring->ring, &cqe_list, count);

  PyObject *list = PyList_New(count);
  for (int i = 0; i < count; i++) {
    CQE *cqe = PyObject_New(CQE, &cqe_type);
    PyObject_Init((PyObject *)cqe, &cqe_type);
    cqe->entry = &cqe_list[i];
    PyList_SetItem(list, i, (PyObject *)cqe);
  }
  return list;
}

/* Signals the ring that this complementation is checked */
PyObject *RingCQESeen(PyObject *self, PyObject *args) {
  Ring *ring = (Ring *)self;
  CQE cqe;
  if (!PyArg_ParseTuple(args, "O!", &cqe_type, &cqe)) return NULL;

  io_uring_cqe_seen(&ring->ring, cqe.entry);

  // TODO(reza): decrease reference of cqe data
  Py_RETURN_NONE;
}

/* destructor of ring */
void RingDestructor(void *self) {
  Ring *ring = (Ring *)self;
  io_uring_queue_exit(&ring->ring);
}

static PyMemberDef ring_members[] = {
    {"entries", T_OBJECT, sizeof(PyObject) + sizeof(struct io_uring), 1,
     "Number of entries in ring"},
    {"flags", T_UINT, offsetof(Ring, ring) + offsetof(struct io_uring, flags),
     1, "Flags of ring"},
    {"fd", T_INT, offsetof(Ring, ring) + offsetof(struct io_uring, ring_fd), 1,
     "file descriptor of ring"},
    {NULL, 0, 0, 0, NULL}};

static PyMethodDef ring_methods[] = {
    {"get_sqe", RingGetSQE, METH_NOARGS, "Get a single SQE"},
    {"submit", RingSubmit, METH_NOARGS, "Submit the ring"},
    {"submit_and_wait", RingSubmitAndWait, METH_VARARGS,
     "Submit the ring and wait for an specific count of complementation"},
    {"wait_for_cqe", RingWaitForCQE, METH_NOARGS,
     "Block-and-wait for a single complementation"},
    {"wait_for_cqe_nr", RingWaitForCQENr, METH_VARARGS,
     "Block-and-wait for a batch of complementations"},
    {"wait_for_cqe_timeout", RingWaitForCQETO, METH_VARARGS,
     "Block-and-wait for a single complementation with timeout"},
    {"peek_cqe", RingPeekCQE, METH_NOARGS, "Peek a single CQE from ring"},
    {"peek_cqe_batch", RingPeekCQEBatch, METH_VARARGS,
     "Peek a batch of CQEs from ring"},
    {"cqe_seen", RingCQESeen, METH_VARARGS, "Mark CQE as seen"},
    {NULL, NULL, 0, NULL}};

static PyTypeObject ring_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0) "_uring_io.Ring", /* tp_name */
    sizeof(Ring),                                            /* tp_basicsize */
    0,                                                       /* tp_itemsize */
    (destructor)RingDestructor,                              /* tp_dealloc */
    0,                                                       /* tp_print */
    0,                                                       /* tp_getattr */
    0,                                                       /* tp_setattr */
    0,                                                       /* tp_reserved */
    (reprfunc)NULL,                                          /* tp_repr */
    0,                                                       /* tp_as_number */
    0,                   /* tp_as_sequence */
    0,                   /* tp_as_mapping */
    0,                   /* tp_hash */
    0,                   /* tp_call */
    0,                   /* tp_str */
    0,                   /* tp_getattro */
    0,                   /* tp_setattro */
    0,                   /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,  /* tp_flags */
    "Uring IO ring",     /* tp_doc */
    (traverseproc)NULL,  /* tp_traverse */
    (inquiry)NULL,       /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    ring_methods,        /* tp_methods */
    ring_members,        /* tp_members */
    0,                   /* tp_getset */
    0,                   /* tp_base */
    0,                   /* tp_dict */
    0,                   /* tp_descr_get */
    0,                   /* tp_descr_set */
    0,                   /* tp_dictoffset */
    RingInit,            /* tp_init */
    PyType_GenericAlloc, /* tp_alloc */
    PyType_GenericNew,   /* tp_new */
};

void register_ring(PyObject *mod) {
  Py_INCREF(&ring_type);
  if (PyModule_AddObject(mod, "Ring", (PyObject *)&ring_type) < 0)
    Py_DECREF(&ring_type);
}