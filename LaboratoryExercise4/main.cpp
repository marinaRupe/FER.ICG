#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <fstream>

using namespace std;

class Point3D {
public:
    float x;
    float y;
    float z;
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {};
};

class PlaneCoefficients {
public:
    float A;
    float B;
    float C;
    float D;
    PlaneCoefficients(float A, float B, float C, float D) : A(A), B(B), C(C), D(D) {};
    PlaneCoefficients() : A(0), B(0), C(0), D(0) {};
};

class Polygon {
public:
    vector<Point3D*> vertices;
    PlaneCoefficients* coefficients;
    Polygon(vector<Point3D*> &vertices) : vertices(vertices), coefficients(new PlaneCoefficients()) {};
    void calculatePlaneCoefficients() {
        coefficients->A = (vertices[1]->y - vertices[0]->y) * (vertices[2]->z - vertices[0]->z)
                          - (vertices[1]->z - vertices[0]->z) * (vertices[2]->y - vertices[0]->y);

        coefficients->B = -(vertices[1]->x - vertices[0]->x) * (vertices[2]->z - vertices[0]->z)
                          + (vertices[1]->z - vertices[0]->z) * (vertices[2]->x - vertices[0]->x);

        coefficients->C = (vertices[1]->x - vertices[0]->x) * (vertices[2]->y - vertices[0]->y)
                          - (vertices[1]->y - vertices[0]->y) * (vertices[2]->x - vertices[0]->x);

        coefficients->D = -vertices[0]->x * coefficients->A -vertices[0]->y * coefficients->B - vertices[0]->z * coefficients->C;
    }
    bool containsPoint(Point3D *testPoint) {
        double dotProduct = coefficients->A * testPoint->x
                     + coefficients->B * testPoint->y
                     + coefficients->C * testPoint->z
                     + coefficients->D;

        return (dotProduct <= 0.0);
    }
    void draw() {
        int n = (int)vertices.size();
        for (int i = 0, i0 = n - 1; i < n; i++) {
            glBegin(GL_LINES);
            glVertex3f(vertices[i0]->x, vertices[i0]->y, vertices[i0]->z);
            glVertex3f(vertices[i]->x, vertices[i]->y, vertices[i]->z);
            glEnd();
            i0 = i;
        }
    }
};

class Figure {
public:
    vector<Polygon*> polygons;
    Figure(vector<Polygon*> &polygons) : polygons(polygons) {
        calculatePlaneCoefficients();
    };
    bool isPointInsideFigure(Point3D *testPoint) {
        for (int i = 0; i < polygons.size(); i++) {
            if (! (polygons[i]->containsPoint(testPoint))) return false;
        }
        return true;
    }
    void draw() {
        for (int i = 0; i < polygons.size(); i++) {
            polygons[i]->draw();
        }
    }
private:
    void calculatePlaneCoefficients() {
        for (int i = 0; i < polygons.size(); i++) {
            polygons[i]->calculatePlaneCoefficients();
        }
    }
};

Figure* figure;
string DIR = "/home/objects/";
float xMax, yMax, zMax;
float xMin, yMin, zMin;

void myDisplay();
void renderScene();
void myReshape(int w, int h);
void loadObject();
Point3D* setPoint();

int main(int argc, char ** argv) {
    loadObject();
    Point3D* testPoint = setPoint();
    cout << "Točka je " << (figure->isPointInsideFigure(testPoint) ? "unutar" : "izvan") << " tijela." << endl;
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_DOUBLE ) ;
    glutInitWindowSize(700, 700) ;
    glutInitWindowPosition(100, 100) ;
    glutCreateWindow("Polygon") ;
    glutDisplayFunc(myDisplay) ;
    glutReshapeFunc(myReshape) ;
    glutMainLoop() ;
    return 0;
}

void myDisplay() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT) ;
    glLoadIdentity();
    renderScene();
    glutSwapBuffers();
}

void renderScene() {
    glPointSize(1.5f);
    glColor3f(0.0f, 0.0f, 0.0f);
    //glRotatef(45.0, 0.0, 1.0, 0.0);
    figure->draw();
}

void myReshape(int w, int h) {
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION) ;
    glLoadIdentity();

    float min = xMin, max = xMax;
    if (yMin < min) min = yMin; if (zMin < min) min = zMin;
    if (yMax > max) max = yMax; if (zMax > max) max = zMax;
    if (min == 0) min = -(max/2);

    glOrtho(1.5f * min, 1.5f * max, 1.5f * min, 1.5f * max, 1.5f * min, 1.5f * max);
    glMatrixMode(GL_MODELVIEW);
}

void loadObject() {
    string object;
    cout << "Odaberite tijelo: " << endl; getline (cin, object);
    ifstream file(DIR + object + ".obj");

    if(!file) {
        cout << "Cannot open input file.\n";
        exit(1);
    }
    vector<Point3D*> vertices;
    vector<Polygon*> polygons;
    string line;
    int n = 0;
    while (getline(file, line)) {
        char startsWith = line[0];

        if (startsWith == '#') continue;

        else if (startsWith == 'v') {
            int len = (int) line.length();
            vector<float> point;
            n++;
            for (int i = 0; i < 4; i++) {
                int index = (int) line.find(" ");

                if (i != 0) {  // skip starting sign ('v')
                    float p = stof(line.substr(0, (unsigned long) index));
                    point.push_back(p);
                }
                line = line.substr((unsigned long) (index + 1), (unsigned long) len);
            }
            vertices.push_back(new Point3D(point[0], point[1], point[2]));

            if (point[0] > xMax) xMax = point[0]; if (point[0] < xMin) xMin = point[0];
            if (point[1] > yMax) yMax = point[1]; if (point[1] < yMin) yMin = point[1];
            if (point[2] > zMax) zMax = point[2]; if (point[2] < zMin) zMin = point[2];
            point.clear();
        }
        else if (startsWith == 'f') {
            int len = (int) line.length();
            vector<Point3D*> polygonVertices;

            for (int i = 0; i < 4; i++) {
                int index = (int) line.find(" ");

                if (i != 0) {  // skip starting sign ('f')
                    int vertexIndex = stoi(line.substr(0, (unsigned long) index)) - 1;
                    polygonVertices.push_back(vertices[vertexIndex]);
                }
                line = line.substr((unsigned long) (index + 1), (unsigned long) len);
            }
            polygons.push_back(new Polygon(polygonVertices));
            polygonVertices.clear();
        }
    }
    file.close();
    figure = new Figure(polygons);
    vertices.clear(); polygons.clear();
}

Point3D* setPoint() {
    const int N = 3;
    string pointStr;
    cout << "Molim koordinate točke?" << endl; getline (cin, pointStr);
    int len = (int) pointStr.length();

    vector<float> point;
    for (int i = 0; i < N; i++) {
        int index = (int) pointStr.find(" ");
        point.push_back(stoi(pointStr.substr(0, (unsigned long) index)));
        pointStr = pointStr.substr((unsigned long) (index + 1), (unsigned long) len);
    }
    return new Point3D(point[0], point[1], point[2]);
}
