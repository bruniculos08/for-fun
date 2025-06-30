# Regressão polinomial (inclui a linear):
import numpy

# Tabela de dados (em pares (x,y) ):
table = [(1,1), (1,2), (7,2), (4,2), (5,2), (8,3), (4,5), (5,8)]
X, Y = map(list, zip(*table))
print(X)

n = len(X)
x_mean = (1/n) * sum(x_i for x_i in X)
y_mean = (1/n) * sum(y_i for y_i in Y)
a = sum((x_i - x_mean) * (y_i - y_mean) for (x_i, y_i) in zip(X, Y)) / sum((x_i - x_mean) ** 2 for x_i in X)
b = y_mean - (a * x_mean)

print(b)
print(a)

def regression(degree : int, table : list[tuple]) -> list:
    X, Y = map(list, zip(*table))
    n = len(X)
    matrix = []
    for k in range(0, degree+1):
        row = []
        for j in range(0, degree+1):
            row.append(sum(X[i-1] ** (j + k) for i in range(1, n+1)))
        matrix.append(row)

    matrix_right = [ sum(Y[i-1] * (X[i-1] ** k) for i in range(1, n+1)) for k in range(0, degree+1) ]
    return numpy.linalg.solve(matrix, matrix_right)

print(regression(1, table))
