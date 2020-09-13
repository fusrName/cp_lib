def floor_sum(n: int, m: int, a: int, b: int) -> int:
    ans = 0
    while True:
        ans += (n - 1) * n // 2 * (a // m)
        a %= m
        ans += n * (b // m)
        b %= m
        y_max = (a * n + b) // m
        if y_max == 0:
            return ans
        x_max = y_max * m - b
        ans += (n - (x_max + a - 1) // a) * y_max
        n, m, a, b = y_max, a, m, (-x_max) % a