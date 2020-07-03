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
        self._hp = pointer(self._h)
        lib.make_heap(self._hp, capacity+1)

    def insert(self, key, o):
        el = py_object(o)
        pythonapi.Py_IncRef(el)
        self.__class__._insert(self._hp, key, el)

    def extract(self):
        _r = py_object()
        self.__class__._extract(self._hp, pointer(_r))
        pythonapi.Py_DecRef(_r)
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
    from timeit import timeit
    from random import randint
    import heapq

    data = [randint(1, 1000) for _ in range(1000000)]

    def test_heapq():
        global data
        l = []
        for i in data:
            heapq.heappush(l, i)
        for i in range(len(data)):
            heapq.heappop(l)

    def test_heap():
        global data
        h = Heap(len(data))
        ll = []
        for i in data:
            h.insert(1000-i, i)
        for i in range(len(data)):
            h.extract()

    a = timeit(test_heapq, number=1)
    b = timeit(test_heap, number=1)
    print(f'{a} / {b} = {a/b}')
