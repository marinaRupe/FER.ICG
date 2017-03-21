from numpy import array
from numpy.linalg import solve

default_input = "1, 1, 1, 6, -1, -2, 1, -2, 2, 1, 3, 13"

user_input = raw_input("Insert equations (or press enter for default): ").strip()
user_input = default_input if user_input == '' else user_input
user_input = [float(x) for x in user_input.split(', ')]

a = []
b = []
N = 3
for i in range(N):
    eq = user_input[:N+1]
    user_input = user_input[N+1:]

    a.append(eq[:-1])
    b.append(eq[-1])

result = solve(array(a), array(b))
print result

