from ctypes import *


class node_t(Structure):
    _fields_ = [('key', c_int32),
                ('value', c_void_p)]


class heap_t(Structure):
    _fields_ = [('data', POINTER(node_t)),
                ('capacity', c_size_t),
                ('size', c_size_t)]

class Person:
    name: str
    age: int
    def __init__(self):
        self.name = ''
        self.age = 10


if __name__ == '__main__':
    h = heap_t()
    lib = cdll.LoadLibrary('./heap.so')
    h_p = pointer(h)
    lib.make_heap(h_p, 6)

    extract = lib.extract
    insert = lib.insert
    top = lib.top
    extract.argtypes = [POINTER(heap_t), POINTER(py_object)]
    insert.argtypes = [POINTER(heap_t), c_size_t, py_object]
    top.argtypes = [POINTER(heap_t), POINTER(py_object)]

    insert(h_p, 4, py_object(Person()))
    insert(h_p, 1, py_object(Person()))

    p = py_object()
    extract(h_p, pointer(p))
    extract(h_p, pointer(p))
    print(p.value)
