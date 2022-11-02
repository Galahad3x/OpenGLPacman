#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "food.h"
#include "graphic.h"


Food::Food(float x, float y, float size) {
    this->x = x;
    this->y = y;
    this->size = size;
}


void Food::draw() {
    glColor3f(0,0,0);
    draw_squaref(x, y, size);
}