from numpy import array, matrix, dot, cross
from numpy.linalg import norm
from math import sqrt

v1 = array([2, 3, -4]) + array([-1, 4, -1])
s = dot(v1, array([-1, 4, -1]))
v2 = cross(v1, array([2, 2, 4]))
v3 = v2 / norm(v2)
v4 = -v2

M1 = matrix('1 2 3; 2 1 3; 4 5 1') + matrix('-1 2 -3; 5 -2 7; -4 -1 3')
M2 = matrix('1 2 3; 2 1 3; 4 5 1') * matrix('-1 2 -3; 5 -2 7; -4 -1 3').T
M3 = matrix('1 2 3; 2 1 3; 4 5 1') * matrix('-1 2 -3; 5 -2 7; -4 -1 3').I

print "v1 = " + str(v1)
print "s = " + str(s)
print "v2 = " + str(v2)
print "v3 = " + str(v3)
print "v4 = " + str(v4)
print "\nM1 = \n" + str(M1)
print "\nM2 = \n" + str(M2)
print "\nM3 = \n" + str(M3)

