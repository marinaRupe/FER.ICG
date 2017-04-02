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
        iEdge2D(int a, int b, int c) : a(a), b(b), c(c) {};
};

class iPolyElem {
    public:
        iPoint2D* vertex;
        iEdge2D* edge;
        bool isLeft;
        iPolyElem(iPoint2D* vertex, iEdge2D* edge, bool isLeft): vertex(vertex), edge(edge), isLeft(isLeft) {}
};

void myDisplay();
void myReshape(int width, int height);
void setPolygon();
void setPoint();
void renderScene ();
void calculateEdges(vector<iPolyElem *> &polyElem, int n);
void drawConvexPolygon (vector<iPolyElem*> &polyElem, int n);
void fillConvexPolygon(vector<iPolyElem*> &polyElem, int n);
void chechEdgesOrientation(vector<iPolyElem *> &polyElem, int n, bool *isConvex, int *orientation);
bool pointInsidePolygon(int x, int y, vector<iPolyElem *> &polyElem, int n);

vector<iPolyElem*> polyElem;
bool isPointSetUp = false;
bool isConvex;
int orientation; // 1 -> clockwise
int n = 0;

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
    if (isConvex && orientation == 1) {
        if (! isPointSetUp) {
            cout << "Nacrtaj!" << endl;
            glColor3f (0.0f, 0.0f, 0.0f) ;
            drawConvexPolygon (polyElem, n);
        }
        else {
            cout << "Ispuni!" << endl;
            glColor3f (1.0f, 0.0f, 0.0f);
            fillConvexPolygon(polyElem, n);
        }
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
    string space;
    cout << "Molim broj vrhova poligona? "; cin >> n; getline (cin, space);
    polyElem.resize((unsigned long) n);

    for (int i = 0; i < n; i++) {
        string v;
        cout << "Molim koordinate vrha?" << endl; getline (cin, v);
        int index = (int) v.find(" ");
        int x = stoi(v.substr(0, (unsigned long) index));
        int y = stoi(v.substr((unsigned long) index + 1, v.length()));

        iPoint2D* vertex = new iPoint2D(x, y);
        iEdge2D* edge = new iEdge2D(0, 0, 0);
        polyElem[i] = new iPolyElem(vertex, edge, true);
    }
    calculateEdges(polyElem, n);
    chechEdgesOrientation(polyElem, n, &isConvex, &orientation);

    if (!isConvex) {
        cout << "Poligon je konkavan!" << endl;
        exit(1);
    }
    if (orientation == 0) {
        cout << "Redoslijed vrhova suprotan od smjera kazaljke na satu. Okrećem..." << endl;
        reverse(polyElem.begin(), polyElem.end()); orientation = 1;
        calculateEdges(polyElem, n);
    }
}

void setPoint() {
    string point;
    cout << "Molim koordinate točke?" << endl; getline (cin, point);
    int index = (int) point.find(" ");
    int x = stoi(point.substr(0, (unsigned long) index));
    int y = stoi(point.substr((unsigned long) index + 1, point.length()));

    bool isPointInsidePOlygon = pointInsidePolygon(x, y, polyElem, n);
    if (isPointInsidePOlygon) cout << "TOČKA JE UNUTAR POLIGONA!" << endl;
    else cout << "TOČKA JE IZVAN POLIGONA!" << endl;
    isPointSetUp = true;
}

bool pointInsidePolygon(int x, int y, vector<iPolyElem *> &polyElem, int n) {
    int dotProduct;
    for (int i = 0; i < n; i++) {
        dotProduct = polyElem[i]->edge->a * x + polyElem[i]->edge->b * y + polyElem[i]->edge->c;

        if (dotProduct > 0) return false;
    }
    return true;
}

void calculateEdges(vector<iPolyElem *> &polyElem, int n) {
    int i0 = n - 1;
    for (int i = 0; i < n; i0 = i++) {
        // dot product of vertices
        polyElem[i0]->edge->a = polyElem[i0]->vertex->y - polyElem[i]->vertex->y;
        polyElem[i0]->edge->b = -(polyElem[i0]->vertex->x - polyElem[i]->vertex->x);
        polyElem[i0]->edge->c = (polyElem[i0]->vertex->x * polyElem[i]->vertex->y
                                 - polyElem[i0]->vertex->y * polyElem[i]->vertex->x);
        // is left or right edge
        polyElem[i0]->isLeft = polyElem[i0]->vertex->y < polyElem[i]->vertex->y;
    }
}

void chechEdgesOrientation(vector<iPolyElem *> &polyElem, int n, bool *isConvex, int *orientation) {
    int dotProduct;
    int up, down, on;
    down = up = on = 0;

    for (int i = 0, i0 = n - 2; i < n; i++, i0++) {
        if (i0 >= n) i0 = 0;
        dotProduct = polyElem[i0]->edge->a * polyElem[i]->vertex->x
            + polyElem[i0]->edge->b * polyElem[i]->vertex->y
            + polyElem[i0]->edge->c;

        if (dotProduct == 0) on++;
        else if (dotProduct > 0) up++;
        else down++;
    }

    if (down == 0) {
        *isConvex = true; *orientation = 0;
    }
    else if (up == 0) {
        *isConvex = true; *orientation = 1;
    }
    else {
        *isConvex = false; *orientation = 0;
    }
}

void drawConvexPolygon (vector<iPolyElem*> &polyElem, int n) {
    for (int i = 0, i0 = n - 1; i < n; i0 = i, i++) {
        bresenham(polyElem[i0]->vertex->x, polyElem[i0]->vertex->y, polyElem[i]->vertex->x, polyElem[i]->vertex->y);
    }
}

void fillConvexPolygon (vector<iPolyElem*> &polyElem, int n) {
    int xMin, xMax, yMin, yMax;
    xMin = xMax = polyElem[0]->vertex->x;
    yMin = yMax = polyElem[0]->vertex->y;

    for (int i = 1; i < n; i++) {
        if (xMin > polyElem[i]->vertex->x) xMin = polyElem[i]->vertex->x;
        if (xMax < polyElem[i]->vertex->x) xMax = polyElem[i]->vertex->x;
        if (yMin > polyElem[i]->vertex->y) yMin = polyElem[i]->vertex->y;
        if (yMax < polyElem[i]->vertex->y) yMax = polyElem[i]->vertex->y;
    }

    int i0; double L, D, x;
    for (int y = yMin; y <= yMax; y++) {
        L = xMin; D = xMax;
        i0 = n - 1;

        for (int i = 0; i < n; i0 = i++) {
            if (polyElem[i0]->edge->a == 0.) {
                if (polyElem[i0]->vertex->y == y) {
                    if (polyElem[i0]->vertex->x < polyElem[i]->vertex->x) {
                        L = polyElem[i0]->vertex->x;
                        D = polyElem[i]->vertex->x;
                    }
                    else {
                        L = polyElem[i]->vertex->x;
                        D = polyElem[i0]->vertex->x;
                    }
                    break;
                }
            }
            else {
                x = (-polyElem[i0]->edge->b * y - polyElem[i0]->edge->c) / (double)polyElem[i0]->edge->a;

                if (polyElem[i0]->isLeft) {
                    if (L < x) L = x;
                }
                else {
                    if (D > x ) D = x;
                }
            }
        }
        bresenham((GLint)round(L), y, (GLint)round(D), y);
    }
}