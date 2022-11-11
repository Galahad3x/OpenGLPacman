#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include"graphic.h"
#include"texture.h"

int offset = 0;
int raised = 0;
float color_offset = 0;
int saved_color;
int saved_texture;

void draw_rectangle(int x, int y, int width, int height){
    glBegin(GL_QUADS);

    glVertex3i(x+offset,0+raised,y+offset);
    glVertex3i(x+offset,0+raised,y+height+offset);
    glVertex3i(x+width+offset, 0+raised, y+height+offset);
    glVertex3i(x+width+offset, 0+raised,y+offset);

    glEnd();
}

void draw_square(int x, int y, int size){
    draw_rectangle(x, y, size, size);
}

void draw_rectangle_textured(int x, int y, int width, int height){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,saved_texture);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0,1.0); glVertex3i(x+offset,0+raised,y+offset);
    glTexCoord2f(0.0,0.0); glVertex3i(x+offset,0+raised,y+height+offset);
    glTexCoord2f(1.0,0.0); glVertex3i(x+width+offset, 0+raised, y+height+offset);
    glTexCoord2f(1.0,1.0); glVertex3i(x+width+offset, 0+raised,y+offset);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void draw_square_textured(int x, int y, int size){
    draw_rectangle_textured(x, y, size, size);
}

void draw_squaref(float x, float y, float size){
    glBegin(GL_QUADS);
    glVertex3f(x+offset,5+raised,y+offset);
    glVertex3f(x+offset,5+raised,y+size+offset);
    glVertex3f(x+size+offset,5+raised,y+size+offset);
    glVertex3f(x+size+offset,5+raised,y+offset);
    glEnd();
}

void draw_prism(int x, int y, int z, int width, int height, int length){
    // Quadrat 6
    glBegin(GL_QUADS);
    set_raised(height);
    glVertex3i(offset+x, raised+y, offset+z+length);
    glVertex3i(offset+x+width, raised+y, offset+z+length);
    glVertex3i(offset+x+width, raised+y, offset+z);
    glVertex3i(offset+x, raised+y, offset+z);
    glEnd();

    // Quadrat 1
    glBegin(GL_QUADS);
    set_raised(0);
    glVertex3i(offset+x+width, raised+y+height, offset+z);
    glVertex3i(offset+x+width, raised+y+height, offset+z+length);
    color_darken();
    color_darken();
    glVertex3i(offset+x+width, raised+y, offset+z+length);
    glVertex3i(offset+x+width, raised+y, offset+z);
    color_lighten();
    color_lighten();
    glEnd();

    // Quadrat 5
    glBegin(GL_QUADS);
    set_raised(0);
    glVertex3i(offset+x+width, raised+y, offset+z+length);
    glVertex3i(offset+x+width, raised+y+height, offset+z+length);
    glVertex3i(offset+x, raised+y+height, offset+z+length);
    glVertex3i(offset+x, raised+y, offset+z+length);
    glEnd();

    // Quadrat 2
    glBegin(GL_QUADS);
    set_raised(0);
    glVertex3i(offset+x, raised+y, offset+z+length);
    glVertex3i(offset+x, raised+y+height, offset+z+length);
    glVertex3i(offset+x, raised+y+height, offset+z);
    glVertex3i(offset+x, raised+y, offset+z);
    glEnd();

    // Quadrat 3
    glBegin(GL_QUADS);
    set_raised(0);
    glVertex3i(offset+x, raised+y, offset+z);
    glVertex3i(offset+x, raised+y+height, offset+z);
    glVertex3i(offset+x+width, raised+y+height, offset+z);
    glVertex3i(offset+x+width, raised+y, offset+z);
    glEnd();
}
void draw_prism_textured(int x, int y, int z, int width, int height, int length){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,saved_texture);
    // Quadrat 6
    glBegin(GL_QUADS);
    set_raised(height);
    glTexCoord2f(0.0,1.0); glVertex3i(offset+x, raised+y, offset+z+length);
    glTexCoord2f(0.0,0.0); glVertex3i(offset+x+width, raised+y, offset+z+length);
    glTexCoord2f(1.0,0.0); glVertex3i(offset+x+width, raised+y, offset+z);
    glTexCoord2f(1.0,1.0);glVertex3i(offset+x, raised+y, offset+z);
    glEnd();

    // Quadrat 1
    glBegin(GL_QUADS);
    set_raised(0);
    glTexCoord2f(0.0,1.0); glVertex3i(offset+x+width, raised+y+height, offset+z);
    glTexCoord2f(0.0,0.0); glVertex3i(offset+x+width, raised+y+height, offset+z+length);
    glTexCoord2f(1.0,0.0); glVertex3i(offset+x+width, raised+y, offset+z+length);
    glTexCoord2f(1.0,1.0); glVertex3i(offset+x+width, raised+y, offset+z);
    glEnd();

    // Quadrat 5
    glBegin(GL_QUADS);
    set_raised(0);
    glTexCoord2f(0.0,1.0); glVertex3i(offset+x+width, raised+y, offset+z+length);
    glTexCoord2f(0.0,0.0); glVertex3i(offset+x+width, raised+y+height, offset+z+length);
    glTexCoord2f(1.0,0.0); glVertex3i(offset+x, raised+y+height, offset+z+length);
    glTexCoord2f(1.0,1.0); glVertex3i(offset+x, raised+y, offset+z+length);
    glEnd();

    // Quadrat 2
    glBegin(GL_QUADS);
    set_raised(0);
    glTexCoord2f(0.0,1.0); glVertex3i(offset+x, raised+y, offset+z+length);
    glTexCoord2f(0.0,0.0); glVertex3i(offset+x, raised+y+height, offset+z+length);
    glTexCoord2f(1.0,0.0); glVertex3i(offset+x, raised+y+height, offset+z);
    glTexCoord2f(1.0,1.0); glVertex3i(offset+x, raised+y, offset+z);
    glEnd();

    // Quadrat 3
    glBegin(GL_QUADS);
    set_raised(0);
    glTexCoord2f(0.0,1.0); glVertex3i(offset+x, raised+y, offset+z);
    glTexCoord2f(0.0,0.0); glVertex3i(offset+x, raised+y+height, offset+z);
    glTexCoord2f(1.0,0.0); glVertex3i(offset+x+width, raised+y+height, offset+z);
    glTexCoord2f(1.0,1.0); glVertex3i(offset+x+width, raised+y, offset+z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void draw_cube(int size, int x, int y, int z){
    glPushMatrix();
        glTranslatef(x+offset,y,z+offset);
        glutSolidCube(size);
    glPopMatrix();
}

void draw_sphere(int radi, int x, int y, int z){
    glPushMatrix();
        glTranslatef(x+offset,y,z+offset);
        glutSolidSphere(radi, 20, 20);
    glPopMatrix();
}

void set_offset(int new_offset){
    offset = new_offset;
}
void set_raised(int new_raised){
    raised = new_raised;
}
void set_texture(int new_texture){
    saved_texture = new_texture;
}

void set_3f_color(int color){
    saved_color = color;
    switch (color) {
        case BLACK:
            glColor3f(0+color_offset,0+color_offset,0+color_offset);
            break;
        case LIGHT_GREY:
            glColor3f(0.8+color_offset,0.8+color_offset,0.8+color_offset);
            break;
        case DARK_GREY:
            glColor3f(0.2+color_offset,0.2+color_offset,0.2+color_offset);
            break;
        case RED:
            glColor3f(1+color_offset,0+color_offset,0+color_offset);
            break;
        case ORANGE:
            glColor3f(scale(236)+color_offset,scale(151)+color_offset,scale(4)+color_offset);
            break;
        case PINK:
            glColor3f(scale(241)+color_offset,scale(186)+color_offset,scale(161)+color_offset);
            break;
        case GREEN:
            glColor3f(0+color_offset,1+color_offset,0+color_offset);
            break;
        case DARK_GREEN:
            glColor3f(scale(52)+color_offset,scale(76)+color_offset,scale(17)+color_offset);
            break;
        case BLUE:
            glColor3f(0+color_offset,0+color_offset,1+color_offset);
            break;
        case COOL_BLUE:
            glColor3f(scale(33)+color_offset,scale(112)+color_offset,scale(116)+color_offset);
            break;
        case FULVOUS:
            glColor3f(scale(226)+color_offset, scale(132)+color_offset, scale(19)+color_offset);
            break;
        case RED_PIGMENT:
            glColor3f(scale(239)+color_offset, scale(39)+color_offset, scale(27)+color_offset);
            break;
        case RED_CRAYOLA:
            glColor3f(scale(234)+color_offset, scale(23)+color_offset, scale(68)+color_offset);
            break;
        case BLUE_CRAYOLA:
            glColor3f(scale(55)+color_offset, scale(119)+color_offset, scale(255)+color_offset);
            break;
        case GREEN_SHEEN:
            glColor3f(scale(119)+color_offset, scale(175)+color_offset, scale(156)+color_offset);
            break;
    }
}

void color_darken(){
    color_offset -= 0.1;
    set_3f_color(saved_color);
}

void color_lighten(){
    color_offset += 0.1;
    set_3f_color(saved_color);
}

double scale(int input){
    int input_start = 0.0;
    int input_end = 255.0;
    double output_start = 0.0;
    double output_end = 1.0;
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}
