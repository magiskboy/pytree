#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>


void swap(PyObject **a, PyObject **b)
{
    PyObject *c = *a;
    *a = *b;
    *b = c;
}

int8_t get_parent(uint32_t i)
{
    return (i - 1) / 2;
}

int8_t lt_object(PyObject **u, PyObject **v) {
    if (*u == NULL) return 1;
    if (*v == NULL) return 0;
    if (PyObject_RichCompare(*u, *v, Py_LT)) {
        return 1;
    }
    return 0;
}

typedef struct {
    PyObject_HEAD
    uint32_t capacity;
    uint32_t size;
    PyObject **arr;
} HeapObject;

void heapify(HeapObject *heap, uint32_t r)
{
    uint32_t _l = 2 * r + 1;
    if (_l < heap->size && lt_object(&(heap->arr[r]), &(heap->arr[_l])) == 1) {
        swap(&(heap->arr[_l]), &(heap->arr[r]));
        heapify(heap, _l);
    }
    else {
        uint32_t _r = 2 * r + 2;
        if (_r < heap->size && lt_object(&(heap->arr[r]), &(heap->arr[_r])) == 1) {
            swap(&(heap->arr[_r]), &(heap->arr[r]));
            heapify(heap, _r);
        }
    }
}

static void Heap_dealloc(HeapObject *self)
{
    uint32_t i;
    for (i = 0; i < self->capacity; ++i) {
        Py_XDECREF(self->arr[i]);
    }
    free(self->arr);
    Py_TYPE(self)->tp_free(self);
}

static PyObject *Heap_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    HeapObject *self;
    self = (HeapObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->capacity = 0;
    }
    self->arr = (PyObject **)malloc(self->capacity * sizeof(PyObject *));
    for (uint32_t i = 0; i < self->capacity; ++i) self->arr[i] = NULL;
    self->size = 0;
    return (PyObject *) self;
}

static int Heap_init(HeapObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"capacity", NULL};
    int32_t capacity = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &capacity))
        return -1;
    if (capacity <= 0) {
        PyErr_SetString(PyExc_ValueError, "Can't not allocate for heap, capacity must be than 0");
        return -1;
    }
    self->capacity = (uint32_t) capacity;
    self->arr = (PyObject **)malloc(self->capacity * sizeof(PyObject *));
    for (uint32_t i = 0; i < self->capacity; ++i) self->arr[i] = NULL;
    self->size = 0;
    return 0;
}

static PyObject *Heap_insert(HeapObject *self, PyObject *el)
{
    Py_INCREF(el);
    self->arr[self->size] = el;
    int32_t pi = (self->size - 1) / 2;
    ++self->size;
    while (pi >= 0) {
        heapify((HeapObject *) self, pi);
        pi = (pi - 1) / 2;
    }
    Py_RETURN_NONE;
}

static PyObject *Heap_extract(HeapObject *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *ret = self->arr[0];
    if (ret == NULL)
        Py_RETURN_NONE;

    self->arr[0] = NULL;
    heapify((HeapObject *) self, 0);

    return ret;
}

static PyObject *Heap_top(HeapObject *self, PyObject *Py_UNUSED(ignored))
{
    if (self->arr[0] == NULL)
        Py_RETURN_NONE;
    return self->arr[0];
}

static PyMemberDef Heap_members[] = {
    {"capacity", T_INT, offsetof(HeapObject, capacity), 0, "capacity"},
    {NULL}
};

static PyMethodDef Heap_methods[] = {
    {"insert", (PyCFunction) Heap_insert, METH_O},
    {"extract", (PyCFunction) Heap_extract, METH_NOARGS},
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
