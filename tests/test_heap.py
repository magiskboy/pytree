import random
from timeit import timeit
import heapq
import unittest
from pytree import Heap

n = 10000
data = [random.randint(-10000, 10000) for _ in range(n)]


def test_heapq():
    global data
    l = list()
    for i in data:
        heapq.heappush(l, i)
    for _ in range(len(l)):
        heapq.heappop(l)


def test_heap():
    global data
    h = Heap(len(data))
    for i in data:
        h.insert(i)
    for _ in range(len(data)):
        h.extract()

no = 100
a = timeit(test_heapq, number=no)
b = timeit(test_heap, number=no)
print(f"{a} / {b} = {a / b}")


class HeapTestCase(unittest.TestCase):
    def test_heap(self):
        a = [random.randint(-100, 100) for _ in range(5)]
        b = sorted(a, reverse=True)
        h = Heap(len(a) * 2)
        for i in a:
            h.insert(i)
        c = list()
        for i in range(len(a)):
            c.append(h.extract())
        self.assertIsNone(h.extract())
        print(a)
        print(b)
        print(c)
        for x, y in zip(b, c):
            self.assertEqual(x, y)
