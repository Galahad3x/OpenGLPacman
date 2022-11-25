using namespace std;

#define RED_LIGHT 0
#define BLUE_LIGHT 1
#define WHITE_LIGHT 2
#define AMBIENT_LIGHT 3

#define FULVOUS_MATERIAL -1
#define RED_CRAYOLA_MATERIAL -2

// parameter: GL_AMBIENT, GL_SPECULAR, GL_DIFFUSE
void set_lighting_color(int light_id, int parameter, int color);

void set_light_position(int light_id, int x, int y, int z);
void set_directional_light(int light_id, int x, int y, int z);

void set_material(float r, float g, float b);
void set_material_id(int material_id);

double scale_l(int input);
