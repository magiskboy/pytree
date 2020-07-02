from ctypes import *

lib = cdll.LoadLibrary('./heap.so')

class node_t(Structure):
    _fields_ = [('key', c_int32),
                ('value', c_void_p)]


class heap_t(Structure):
    _fields_ = [('data', POINTER(node_t)),
                ('capacity', c_size_t),
                ('size', c_size_t)]

class Heap:
    _insert = lib.insert
    _extract = lib.extract
    _top = lib.top
    _extract.argtypes = [POINTER(heap_t), POINTER(py_object)]
    _insert.argtypes = [POINTER(heap_t), c_size_t, py_object]
    _top.argtypes = [POINTER(heap_t), POINTER(py_object)]

    def __init__(self, capacity):
        self._h = heap_t()
        lib.make_heap(pointer(self._h), capacity)

    def insert(self, key, o):
        el = py_object(o)
        pythonapi.Py_IncRef(el)
        self.__class__._insert(pointer(self._h), key, el)

    def extract(self):
        _r = py_object()
        self.__class__._extract(pointer(self._h), pointer(_r))
        return _r.value if _r else None

    @property
    def top(self):
        _r = py_object()
        self.__class__._top(pointer(self._h), pointer(_r))
        return _r.value

    @property
    def size(self):
        return self._h.size


if __name__ == '__main__':
    # import random
    # l = [random.randint(1, 100) for _ in range(100)]
    # h = Heap(len(l))
    # for i in l:
    #     h.insert(i, i)
    # ll = list()
    # j = h.extract()
    # while (j != None):
    #     ll.append(j)
    #     j = h.extract()
    # print(ll[:10])

    class Person:
        def __init__(self, name):
            self.name = name
        def __str__(self):
            return self.name

    h = Heap(4)
    a = Person("lmao")
    b = Person("lmfao")
    h.insert(1, a)
    h.insert(2, b)

    print(h.extract())
    print(h.extract())
