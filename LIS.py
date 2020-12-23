from bisect import bisect_left
def LIS(x):
    d = []
    for xi in x:
        idx = bisect_left(d, xi)
        if idx == len(d):
            d.append(xi)
        else:
            d[idx] = xi
    return len(d)
