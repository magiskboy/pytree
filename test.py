import random
from timeit import timeit
from copy import deepcopy
import heapq
import heap

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
    h = heap.Heap(len(data))
    for i in data:
        h.insert(i)
    for _ in range(len(data)):
        h.extract()

no = 100
a = timeit(test_heapq, number=no)
b = timeit(test_heap, number=no)
print(f"{a} / {b} = {a / b}")
