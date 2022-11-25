#ifndef LIGHT_H
#define LIGHT_H

using namespace std;

#define RED_LIGHT 0
#define BLUE_LIGHT 1
#define WHITE_LIGHT 2
#define AMBIENT_LIGHT 3

#define FULVOUS_MATERIAL -1
#define RED_CRAYOLA_MATERIAL -2
#define DARK_GREEN_MATERIAL -3

// parameter: GL_AMBIENT, GL_SPECULAR, GL_DIFFUSE
void set_lighting_color(int light_id, int parameter, int color);

void set_light_position(int light_id, int x, int y, int z);
void set_directional_light(int light_id, int x, int y, int z);

void set_material(float r, float g, float b);
void set_material_id(int material_id);

double scale_l(int input);

void set_light_offset(int offset);

class Flashlight {
    public:
    Flashlight();

    float x, y, z;
    int dx, dy, dz;
    int color;

    int light_id;

    void draw();

    void set_position(float x, float y, float z);
    void set_direction(int dx, int dy, int dz);
};

#endif
