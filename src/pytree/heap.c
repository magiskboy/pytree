#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#define LESS_THAN(u, v) ((u) == NULL ? 1 : ((v) == NULL ? 0 : PyObject_RichCompareBool((u), (v), Py_LT)))

inline void swap(PyObject **a, PyObject **b)
{
    PyObject *c = *a;
    *a = *b;
    *b = c;
}

typedef struct {
    PyObject_HEAD
    int32_t capacity;
    int32_t size;
    PyObject **arr;
} HeapObject;

void heapify(HeapObject *heap, int32_t r)
{
    uint32_t lc, rc, largest;
    while (r < heap->size) {
        lc = (r << 1) + 1;
        rc = (r << 1) + 2;
        largest = r;
        if (lc < heap->size && LESS_THAN(heap->arr[largest], heap->arr[lc])) largest = lc;
        if (rc < heap->size && LESS_THAN(heap->arr[largest], heap->arr[rc])) largest = rc;
        if (largest == r) break;
        swap(&(heap->arr[largest]), &(heap->arr[r]));
        r = largest;
    }
}

static void Heap_dealloc(HeapObject *self)
{
    free(self->arr);
    Py_TYPE(self)->tp_free(self);
}

static PyObject *Heap_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    HeapObject *self;
    self = (HeapObject *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

static int Heap_init(HeapObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"capacity", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &self->capacity))
        return -1;
    if (self->capacity <= 0) {
        PyErr_SetString(PyExc_ValueError, "Can't not allocate for heap, capacity must be greate than 0"); return -1;
    }
    self->arr = (PyObject **)malloc(self->capacity * sizeof(PyObject *));
    return 0;
}

static PyObject *Heap_insert(HeapObject *self, PyObject *el)
{
    if (self->size == self->capacity) {
        PyErr_SetString(PyExc_MemoryError, "Memory over size");
        return PyErr_Occurred();
    }

    Py_INCREF(el);
    self->arr[self->size] = el;
    int32_t curr = self->size++ - 1;
    while (curr >= 0) {
        curr = curr >> 1;
        heapify((HeapObject *) self, curr--);
    }
    Py_RETURN_NONE;
}

static PyObject *Heap_extract(HeapObject *self, PyObject *Py_UNUSED(ignored))
{
    if (self->size == 0)
        Py_RETURN_NONE;
    PyObject *ret = self->arr[0];
    self->arr[0] = self->arr[self->size - 1];
    self->arr[self->size - 1] = NULL;
    heapify((HeapObject *) self, 0);
    --self->size;
    return ret;
}

static PyObject *Heap_tolist(HeapObject *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *l = PyList_New(self->capacity);
    for (int32_t i = 0; i < self->capacity; ++i) {
        if (self->arr[i] != NULL){
            Py_INCREF(self->arr[i]);
            PyList_SetItem(l, (Py_ssize_t) i, self->arr[i]);
        }
        else
            PyList_SetItem(l, (Py_ssize_t) i, Py_None);
    }
    return l;
}

static PyObject *Heap_top(HeapObject *self, PyObject *Py_UNUSED(ignored))
{
    if (self->arr[0] == NULL)
        Py_RETURN_NONE;
    return self->arr[0];
}

static PyMemberDef Heap_members[] = {
    {"capacity", T_INT, offsetof(HeapObject, capacity), 0, "capacity"},
    {"size", T_INT, offsetof(HeapObject, size), 0, "size"},
    {NULL}
};

static PyMethodDef Heap_methods[] = {
    {"insert", (PyCFunction) Heap_insert, METH_O},
    {"extract", (PyCFunction) Heap_extract, METH_NOARGS},
    {"tolist", (PyCFunction) Heap_tolist, METH_NOARGS},
    {"top", (PyCFunction) Heap_top, METH_NOARGS},
    {NULL}
};

static PyTypeObject HeapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "heap.Heap",
        .tp_doc = "Heap",
        .tp_basicsize = sizeof(HeapObject),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        .tp_new = Heap_new,
        .tp_init = (initproc) Heap_init,
        .tp_dealloc = (destructor) Heap_dealloc,
        .tp_members = Heap_members,
        .tp_methods = Heap_methods,
};

static PyModuleDef heap_mod = {
    PyModuleDef_HEAD_INIT,
        .m_name = "heap",
        .m_doc = "Heap module",
        .m_size = -1,
};

PyMODINIT_FUNC
PyInit_heap(void)
{
    PyObject *m;
    if (PyType_Ready(&HeapType) < 0)
        return NULL;

    m = PyModule_Create(&heap_mod);
    if (m == NULL)
        return NULL;

    Py_INCREF(&HeapType);
    if (PyModule_AddObject(m, "Heap", (PyObject*)&HeapType) < 0) {
        Py_DECREF(&HeapType);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}
