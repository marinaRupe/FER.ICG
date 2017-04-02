#include <stdio.h>
#include <GL/glut.h>

GLdouble Lx[2], Ly[2];
GLint Ix;

GLuint window;
GLuint width = 300, height = 300;

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myLine(GLint xa, GLint ya, GLint xb, GLint yb);
void bresenham1(GLint xa, GLint ya, GLint xb, GLint yb);
void bresenham2(GLint xa, GLint ya, GLint xb, GLint yb);

int main(int argc, char ** argv) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = (GLuint) glutCreateWindow("Glut OpenGL Linija");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    printf("Lijevom tipkom misa zadaj tocke - algoritam Bresenham-a\n");
    printf("Tipke r, g, b, k mijenjaju boju.\n");

    glutMainLoop();
    return 0;
}

void myDisplay() {
    glFlush();
}

void myReshape(int w, int h) {
    width = (GLuint) w; height = (GLuint) h;
    Ix = 0;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
}

void myLine(GLint xa, GLint ya, GLint xb, GLint yb) {
    glBegin(GL_LINES);
    {
        glVertex2i(xa, ya + 5);
        glVertex2i(xb, yb + 5);
    }
    glEnd();

    if (xa <= xb) {
        if (ya <= yb) bresenham1(xa, ya, xb, yb);
        else bresenham2(xa, ya, xb, yb);
    }
    else {
        if (ya >= yb) bresenham1(xb, yb, xa, ya);
        else bresenham2(xb, yb, xa, ya);
    }
}

void bresenham1(GLint xa, GLint ya, GLint xb, GLint yb) {
    GLint  a, yf;
    GLint  x, yc, correction;

    glBegin(GL_POINTS);
    if (yb - ya <= xb - xa) {
        a = 2 * (yb - ya);
        yc = ya;
        yf = -(xb - xa);
        correction = -2 * (xb - xa);

        for (x = xa; x <= xb; x++) {
            glVertex2i(x, yc);
            yf += a;

            if (yf >= 0) {
                yf += correction;
                yc++;
            }
        }
    }
    else {
        x = xb; xb = yb; yb = x;
        x = xa; xa = ya; ya = x;
        a = 2 * (yb - ya);
        yc = ya;
        yf = -(xb - xa);
        correction = -2 * (xb - xa);

        for (x = xa; x <= xb; x++) {
            glVertex2i(yc, x);
            yf += a;

            if (yf >= 0) {
                yf += correction;
                yc++;
            }
        }
    }
    glEnd();
}

void bresenham2(GLint xa, GLint ya, GLint xb, GLint yb) {
    GLint  a, yf;
    GLint  x, yc, correction;

    glBegin(GL_POINTS);
    if (-(yb - ya) <= xb - xa) {
        a = 2 * (yb - ya);
        yc = ya;
        yf = xb - xa;
        correction = 2 * (xb - xa);

        for (x = xa; x <= xb; x++) {
            glVertex2i(x, yc);
            yf += a;

            if (yf <= 0) {
                yf += correction;
                yc--;
            }
        }
    }
    else {
        x = xb; xb = ya; ya = x;
        x = xa; xa = yb; yb = x;
        a = 2 * (yb - ya);
        yc = ya;
        yf = xb - xa;
        correction = 2 * (xb - xa);

        for (x = xa; x <= xb; x++) {
            glVertex2i(yc, x);
            yf += a;

            if (yf <= 0) {
                yf += correction;
                yc--;
            }
        }
    }
    glEnd();
}


void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Lx[Ix] = x;
        Ly[Ix] = height - y;
        Ix = Ix ^ 1;

        if (Ix == 0)	myLine((int)Lx[0], (int)Ly[0], (int)Lx[1], (int)Ly[1]);
        else			glVertex2i(x, height - y);

        printf("Koordinate tocke %d: %d %d \n", Ix ^ 1, x, y);
        glFlush();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        myReshape(width, height);
    }
}


void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
    switch (theKey) {
        case 'r':
            glColor3f(1, 0, 0);
            break;

        case 'g':
            glColor3f(0, 1, 0);
            break;

        case 'b':
            glColor3f(0, 0, 1);
            break;

        case 'k':
            glColor3f(0, 0, 0);

    }
    glRecti(width - 15, height - 15, width, height);
    glFlush();
}