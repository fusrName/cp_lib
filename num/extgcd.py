def extgcd(a, b):
    ac, bc = (1, 0), (0, 1)
    while b:
        q = a // b
        a, b = b, a % b
        ac, bc = bc, (ac[0]-q*bc[0], ac[1]-q*bc[1])
    return a, ac