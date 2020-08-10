from cmath import tau, rect

def fft(a, n):
    if n == 1:
        return a
    a0, a1 = (fft(a[i::2], n // 2) for i in range(2))
    zeta = rect(1, tau / n)
    x = 1
    ret = a0 * 2
    for i, f in enumerate(a1):
        ret[i] += x * f
        ret[i + n // 2] += -x * f
        x *= zeta
    return ret

def ifft(a, n):
    if n == 1:
        return a
    a0, a1 = (ifft(a[i::2], n // 2) for i in range(2))
    zeta = rect(1, -tau / n)
    x = 1
    ret = a0 * 2
    for i, f in enumerate(a1):
        ret[i] += x * f
        ret[i + n // 2] += -x * f
        x *= zeta
    return ret
