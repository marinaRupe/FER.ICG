from numpy import array, matrix

default_coordinates = "1, 0, 1, 5, 0, 1, 3, 8, 1, 3, 4, 1"

coordinates = raw_input("Insert coordinates (or press enter for default): ").strip()
coordinates = default_coordinates if coordinates == "" else coordinates
coordinates = [float(x) for x in coordinates.split(', ')]

triangle = [[], [], []]
t = []
for i in range(4):
    coord = coordinates[:3]
    coordinates = coordinates[3:]

    if i < 3:
        for j in range(3):
            triangle[j].append(coord[j])
    else:
        t.append(coord)

triangle = matrix(array(triangle)).I
t = matrix(array(t)).T

print str(triangle) + "\n"
print str(t) + "\n"

barycentric_coordinates = triangle * t
print "Barycentric coordinates:\n" + str(barycentric_coordinates)
