#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "food.h"


Food::Food(int x, int y, int size) {
    this->x = x;
    this->y = y;
    this->half_part = size / 2;
}


void Food::draw() {
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex2i(x-half_part,y-half_part);
        glVertex2i(x+half_part,y-half_part);
        glVertex2i(x+half_part,y+half_part);
        glVertex2i(x-half_part,y+half_part);
    glEnd();
}