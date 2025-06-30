import math

n = 100
a = [0 for _ in range(0, n)]
a[0] = 2
m = 3
i = 1
while (i < n):
    b = True
    s = math.sqrt(m)
    j = 0
    while (b and a[j] <= s):
        if (m % a[j] == 0):
            b = False
        else:
            j += 1
    if b:
        a[i] = m
        i += 1
    m = m + 2

print(a)