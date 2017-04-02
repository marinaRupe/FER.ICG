#include <GL/glut.h>

void bresenham (GLint xa, GLint ya, GLint xb, GLint yb);
void bresenham1(GLint xa, GLint ya, GLint xb, GLint yb);
void bresenham2(GLint xa, GLint ya, GLint xb, GLint yb);

void bresenham (GLint xa, GLint ya, GLint xb, GLint yb) {
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