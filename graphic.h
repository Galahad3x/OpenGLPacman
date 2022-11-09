using namespace std;

#define BLACK 0
#define LIGHT_GREY 1
#define DARK_GREY 2
#define RED 3
#define ORANGE 4
#define PINK 5
#define GREEN 6
#define DARK_GREEN 7
#define BLUE 8
#define COOL_BLUE 9
#define FULVOUS 10
#define RED_PIGMENT 11
#define RED_CRAYOLA 12
#define BLUE_CRAYOLA 13
#define GREEN_SHEEN 14

void draw_rectangle(int x, int y, int width, int height);
void draw_square(int x, int y, int size);
void draw_squaref(float x, float y, float size);

void draw_prism(int x, int y, int z, int width, int height, int length);

void draw_cube(int size, int x, int y, int z);
void draw_sphere(int radi, int x, int y, int z);

void set_offset(int new_offset);
void set_raised(int new_raised);

void set_3f_color(int color);

void color_darken();
void color_lighten();

double scale(int input);
