#ifndef URING_H_
#define URING_H_

#if defined(_MSC_VER) && defined(_DEBUG)
// To avoid auto-linking (#pragma comment(lib, <name>)) debug library in
// pyconfig.h Uncomment if your install contains the debug library
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <liburing.h>
#include <listobject.h>
#include <methodobject.h>
#include <modsupport.h>
#include <object.h>
#include <pyerrors.h>
#include <structmember.h>

/**
 * @brief Python struct for sqe
 *
 */
typedef struct {
  PyObject_HEAD struct io_uring_sqe *entry;
  PyObject *flags;
} SQE;

/**
 * @brief Python struct for cqe
 *
 */
typedef struct {
  PyObject_HEAD struct io_uring_cqe *entry;
} CQE;

extern void register_ring(PyObject *mod);
extern void register_sqe(PyObject *mod);
extern void register_cqe(PyObject *mod);
#endif