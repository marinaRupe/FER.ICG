#include <iostream>
#include <math.h>
#include <algorithm>
#include <GL/glut.h>
#include "bresenham.h"

using namespace std;

class iPoint2D {
public:
    int x;
    int y;
    iPoint2D(int x, int y) : x(x), y(y) {};
};

class iEdge2D {
    public:
        int a;
        int b;
        int c;
        iEdge2D() : a(0), b(0), c(0) {};
        iEdge2D(int a, int b, int c) : a(a), b(b), c(c) {};
};

class iPolyElem {
    public:
        iPoint2D* vertex;
        iEdge2D* edge;
        bool isLeft;
        iPolyElem(iPoint2D* vertex): vertex(vertex), edge(new iEdge2D()), isLeft(false) {}
};

class iPolygon {
public:
    vector<iPolyElem *> polyElems;
    int n;
    bool isConvex;
    int orientation; // 1 -> clockwise
    iPolygon(vector<iPolyElem *> &polyElems, int n) : polyElems(polyElems), n(n) {
        calculateEdges();
        setEdgesOrientation();
    };

    void calculateEdges() {
        int i0 = n - 1;
        for (int i = 0; i < n; i0 = i++) {
            // dot product of vertices
            polyElems[i0]->edge->a = polyElems[i0]->vertex->y - polyElems[i]->vertex->y;
            polyElems[i0]->edge->b = -(polyElems[i0]->vertex->x - polyElems[i]->vertex->x);
            polyElems[i0]->edge->c = (polyElems[i0]->vertex->x * polyElems[i]->vertex->y
                                      - polyElems[i0]->vertex->y * polyElems[i]->vertex->x);
            // is left or right edge
            polyElems[i0]->isLeft = polyElems[i0]->vertex->y < polyElems[i]->vertex->y;
        }
    }

    void setEdgesOrientation() {
        int dotProduct, up, down, on;
        down = up = on = 0;

        for (int i = 0, i0 = n - 2; i < n; i++, i0++) {
            if (i0 >= n) i0 = 0;
            dotProduct = polyElems[i0]->edge->a * polyElems[i]->vertex->x
                         + polyElems[i0]->edge->b * polyElems[i]->vertex->y
                         + polyElems[i0]->edge->c;

            if (dotProduct == 0) on++;
            else if (dotProduct > 0) up++;
            else down++;
        }

        if (down == 0) {
            isConvex = true;
            orientation = 0;
        } else if (up == 0) {
            isConvex = true;
            orientation = 1;
        } else {
            isConvex = false;
            orientation = 0;
        }
    }

    bool pointInsidePolygon(int x, int y) {
        int dotProduct;
        for (int i = 0; i < n; i++) {
            dotProduct = polyElems[i]->edge->a * x + polyElems[i]->edge->b * y + polyElems[i]->edge->c;

            if (dotProduct > 0) return false;
        }
        return true;
    }
    void reverseOrientation() {
        reverse(polyElems.begin(), polyElems.end()); orientation = 1;
    }

    void draw() {
        if (isConvex) {
            for (int i = 0, i0 = n - 1; i < n; i++) {
                bresenham(polyElems[i0]->vertex->x, polyElems[i0]->vertex->y, polyElems[i]->vertex->x,
                          polyElems[i]->vertex->y);
                i0 = i;
            }
        }
    }

    void fill() {
        int xMin, xMax, yMin, yMax;
        xMin = xMax = polyElems[0]->vertex->x;
        yMin = yMax = polyElems[0]->vertex->y;

        for (int i = 1; i < n; i++) {
            if (xMin > polyElems[i]->vertex->x) xMin = polyElems[i]->vertex->x;
            if (xMax < polyElems[i]->vertex->x) xMax = polyElems[i]->vertex->x;
            if (yMin > polyElems[i]->vertex->y) yMin = polyElems[i]->vertex->y;
            if (yMax < polyElems[i]->vertex->y) yMax = polyElems[i]->vertex->y;
        }

        int i0;
        double L, D, x;
        for (int y = yMin; y <= yMax; y++) {
            L = xMin;
            D = xMax;
            i0 = n - 1;

            for (int i = 0; i < n; i0 = i++) {
                if (polyElems[i0]->edge->a == 0.) {
                    if (polyElems[i0]->vertex->y == y) {
                        if (polyElems[i0]->vertex->x < polyElems[i]->vertex->x) {
                            L = polyElems[i0]->vertex->x;
                            D = polyElems[i]->vertex->x;
                        } else {
                            L = polyElems[i]->vertex->x;
                            D = polyElems[i0]->vertex->x;
                        }
                        break;
                    }
                } else {
                    x = (-polyElems[i0]->edge->b * y - polyElems[i0]->edge->c) / (double) polyElems[i0]->edge->a;

                    if (polyElems[i0]->isLeft) {
                        if (L < x) L = x;
                    } else {
                        if (D > x) D = x;
                    }
                }
            }
            bresenham((GLint) round(L), y, (GLint) round(D), y);
        }
    }
};

void myDisplay();
void myReshape(int width, int height);
void setPolygon();
void setPoint();
void renderScene ();

iPolygon* polygon;
bool isPointSetUp = false;

int main (int argc, char ** argv) {
    //Examples:
    //3, (20, 10), (10, 50), (60, 10), inside: (30, 20), outside: (10, 10)
    //4, (50, 200), (150, 350), (300, 150), (100, 50), inside: (150, 200)
    setPolygon();
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_DOUBLE ) ;
    glutInitWindowSize(700, 700) ;
    glutInitWindowPosition(100 , 100) ;
    glutCreateWindow("Polygon") ;
    glutDisplayFunc(myDisplay) ;
    glutReshapeFunc(myReshape) ;
    glutMainLoop() ;
    return 0;
}

void myDisplay () {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT) ;
    glLoadIdentity();
    renderScene();
    glutSwapBuffers();
    if (!isPointSetUp) setPoint();
}

void renderScene () {
    glPointSize (1.0f);
    if (! isPointSetUp) {
        cout << "Nacrtaj!" << endl;
        glColor3f (0.0f, 0.0f, 0.0f) ;
        polygon->draw();
    }
    else {
        cout << "Ispuni!" << endl;
        glColor3f (1.0f, 0.0f, 0.0f);
        polygon->fill();
    }
}

void myReshape(int w, int h) {
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION) ;
    glLoadIdentity ();
    glOrtho(0, w - 1, h - 1, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void setPolygon() {
    int n = 0;
    string space;
    vector<iPolyElem*> polyElem;

    cout << "Molim broj vrhova poligona? "; cin >> n; getline (cin, space);
    polyElem.resize((unsigned long) n);

    for (int i = 0; i < n; i++) {
        string v;
        cout << "Molim koordinate vrha?" << endl; getline (cin, v);
        int index = (int) v.find(" ");
        int x = stoi(v.substr(0, (unsigned long) index));
        int y = stoi(v.substr((unsigned long) index + 1, v.length()));

        iPoint2D* vertex = new iPoint2D(x, y);
        polyElem[i] = new iPolyElem(vertex);
    }
    polygon = new iPolygon(polyElem, n);

    if (! polygon->isConvex) {
        cout << "Poligon je konkavan!" << endl;
        exit(1);
    }
    if (polygon->orientation == 0) {
        cout << "Redoslijed vrhova suprotan od smjera kazaljke na satu. Okrećem..." << endl;
        polygon->reverseOrientation();
        polygon->calculateEdges();
    }
}

void setPoint() {
    string point;
    cout << "Molim koordinate točke?" << endl; getline (cin, point);
    int index = (int) point.find(" ");
    int x = stoi(point.substr(0, (unsigned long) index));
    int y = stoi(point.substr((unsigned long) index + 1, point.length()));

    bool isPointInsidePOlygon = polygon->pointInsidePolygon(x, y);
    if (isPointInsidePOlygon) cout << "TOČKA JE UNUTAR POLIGONA!" << endl;
    else cout << "TOČKA JE IZVAN POLIGONA!" << endl;
    isPointSetUp = true;
}
