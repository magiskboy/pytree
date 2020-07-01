import random
from timeit import timeit
import heapq
from pytree import Heap


def benchmark_heap(n=1000000):
    data = [random.randint(-10000, 10000) for _ in range(n)]
    def test_heapq():
        nonlocal data
        l = list()
        for i in data:
            heapq.heappush(l, i)
        for _ in range(len(l)):
            heapq.heappop(l)


    def test_heap():
        nonlocal  data
        h = Heap(len(data))
        for i in data:
            h.insert(i)
        for _ in range(len(data)):
            h.extract()

    no = 1
    a = timeit(test_heapq, number=no)
    b = timeit(test_heap, number=no)
    print(f"{a} / {b} = {a / b}")


if __name__ == '__main__':
    import sys
    mod = sys.argv[1]

    if mod == 'heap':
        benchmark_heap()
