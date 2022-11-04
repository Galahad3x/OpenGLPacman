#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include"graphic.h"

void draw_rectangle(int x, int y, int width, int height){
    glBegin(GL_QUADS);

    glVertex2i(x,y);
    glVertex2i(x,y+height);
    glVertex2i(x+width, y+height);
    glVertex2i(x+width, y);

    glEnd();
}

void draw_square(int x, int y, int size){
    draw_rectangle(x, y, size, size);
}

void draw_squaref(float x, float y, float size){
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x,y+size);
    glVertex2f(x+size, y+size);
    glVertex2f(x+size, y);
    glEnd();
}



void set_3f_color(int color){
    switch (color) {
        case BLACK:
            glColor3f(0,0,0);
            break;
        case LIGHT_GREY:
            glColor3f(0.8,0.8,0.8);
            break;
        case DARK_GREY:
            glColor3f(0.2,0.2,0.2);
            break;
        case RED:
            glColor3f(1,0,0);
            break;
        case ORANGE:
            glColor3f(scale(236),scale(151),scale(4));
            break;
        case PINK:
            glColor3f(scale(241),scale(186),scale(161));
            break;
        case GREEN:
            glColor3f(0,1,0);
            break;
        case DARK_GREEN:
            glColor3f(scale(52),scale(76),scale(17));
            break;
        case BLUE:
            glColor3f(0,0,1);
            break;
        case COOL_BLUE:
            glColor3f(scale(33),scale(112),scale(116));
            break;
    }
}

double scale(int input){
    int input_start = 0.0;
    int input_end = 255.0;
    double output_start = 0.0;
    double output_end = 1.0;
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}
