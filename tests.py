import random
from timeit import timeit
import heapq
import unittest
from pytree import Heap


class HeapTestCase(unittest.TestCase):
    def test_heap(self):
        a = [random.randint(-100, 100) for _ in range(5)]
        b = sorted(a, reverse=True)
        h = Heap(len(a))
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
