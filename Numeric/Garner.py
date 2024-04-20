
def gcd(a, b):
    if(b == 0):
        return a
    return gcd(b, a % b)

def EuclidesExt(a, b):
    if (b == 0):
        x = 1
        y = 0
        return a, x, y
    g, x1, y1 = EuclidesExt(b, a % b)
    return g, y1, (x1 - y1 * (a//b))

def InverseModulo(a, n):
    if (gcd(a, n) != 1):
        print("There is no inverse modulo for " + str(a))
        return None
    g, x, y = EuclidesExt(a, n)
    return x

def Garner(primes):
    