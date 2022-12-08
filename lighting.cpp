#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "lighting.h"
#include <stdio.h>

int light_offset = 0;

// parameter: GL_AMBIENT, GL_SPECULAR, GL_DIFFUSE
void set_lighting_color(int light_id, int parameter, int color_id){
    GLfloat color[4];
    switch (color_id) {
        case RED_LIGHT:
            color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=1.0;
            break;
        case BLUE_LIGHT:
            color[0]=0.0; color[1]=0.0; color[2]=1.0; color[3]=1.0;
            break;
        case WHITE_LIGHT:
            color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=1.0;
            break;
        case AMBIENT_LIGHT:
            color[0]=0.1; color[1]=0.1; color[2]=0.1; color[3]=0.1;
            break;
        case SAGE_LIGHT:
            color[0]=0.9; color[1]=0.74; color[2]=0.23; color[3]=1.0;
            break;
        case ZEROS_LIGHT:
            color[0]=0.0; color[1]=0.0; color[2]=0.0; color[3]=0.0;
            break;
    }
    glLightfv(light_id,parameter,color);
}

void set_light_position(int light_id, int x, int y, int z){
    GLint position[4];
    position[0]=x; position[1]=y; position[2]=z; position[3]=1;
    glLightiv(light_id,GL_POSITION,position);
}

void set_light_direction(int light_id, int x, int y, int z){
    GLint position[4];
    position[0]=x; position[1]=y; position[2]=z; position[3]=1;
    glLightiv(light_id,GL_SPOT_DIRECTION,position);
}

void set_directional_light(int light_id, int x, int y, int z){
    GLint position[4];
    position[0]=x; position[1]=y; position[2]=z; position[3]=0;
    glLightiv(light_id,GL_POSITION,position);
}

void set_material(float r, float g, float b){
    GLfloat material[4];
    material[0]=r; material[1]=g; material[2]=b; material[3]=1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
}

void set_material_id(int material_id){
    int r,g,b;
    GLfloat material[4];
    switch (material_id) {
        case FULVOUS_MATERIAL:
            material[0]=scale_l(226); material[1]=scale_l(139); material[2]=scale_l(19); material[3]=1.0;
            break;
        case RED_CRAYOLA_MATERIAL:
            material[0]=scale_l(234); material[1]=scale_l(23); material[2]=scale_l(68); material[3]=1.0;
            break;
        case DARK_GREEN_MATERIAL:
            material[0]=scale_l(52); material[1]=scale_l(76); material[2]=scale_l(17); material[3]=1.0;
            break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
}

double scale_l(int input){
    int input_start = 0.0;
    int input_end = 255.0;
    double output_start = 0.0;
    double output_end = 1.0;
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}

void set_light_offset(int offset){
    light_offset = offset;
}

//---------------FLASHLIGHT---------------

Flashlight::Flashlight(){}

void Flashlight::draw() {
    //glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.005);
    //glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.0);
    set_light_position(this->light_id, this->x, this->y, this->z);

    set_lighting_color(this->light_id, GL_SPECULAR, this->color);
    set_lighting_color(this->light_id, GL_DIFFUSE, this->color);

    set_light_direction(this->light_id, dx, dy, dz);

    glLighti(this->light_id,GL_SPOT_CUTOFF,90);
    glLighti(this->light_id,GL_SPOT_EXPONENT,8);

    glEnable(this->light_id);
}

void Flashlight::set_position(float x, float y, float z){
    this->x = (int) x+light_offset;
    this->y = (int) y;
    this->z = (int) z+light_offset;
}

void Flashlight::set_direction(int dx, int dy, int dz){
    this->dx = dx;
    this->dy = dy;
    this->dz = dz;
}

void Flashlight::set_to_direction(int keydir){
    switch (keydir) {
    case GLUT_KEY_UP:
        set_direction(0,-1,-4);
        break;
    case GLUT_KEY_DOWN:
        set_direction(0,-1,4);
        break;
    case GLUT_KEY_LEFT:
        set_direction(-4,-1,0);
        break;
    case GLUT_KEY_RIGHT:
        set_direction(4,-1,0);
        break;
    default:
        break;
    }
}
