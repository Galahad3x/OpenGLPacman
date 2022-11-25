#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "lighting.h"

// parameter: GL_AMBIENT, GL_SPECULAR, GL_DIFFUSE
void set_lighting_color(int light_id, int parameter, int color_id){
    GLfloat color[4];
    switch (color_id) {
        case RED_LIGHT:
            color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=1;
            break;
        case BLUE_LIGHT:
            color[0]=0.0; color[1]=0.0; color[2]=1.0; color[3]=1;
            break;
        case WHITE_LIGHT:
            color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=1;
            break;
        case AMBIENT_LIGHT:
            color[0]=0.05; color[1]=0.05; color[2]=0.05; color[3]=1;
            break;
    }
    glLightfv(light_id,parameter,color);
}

void set_light_position(int light_id, int x, int y, int z){
    GLint position[4];
    position[0]=x; position[1]=y; position[2]=z; position[3]=1;
    glLightiv(light_id,GL_POSITION,position);
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
