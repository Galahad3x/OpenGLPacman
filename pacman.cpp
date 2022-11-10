#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<time.h>
#include<list>
#include<tgmath.h>
#include"graphic.h"
#include"map.h"
#include"agent.h"
#include"food.h"
#include"ghost.h"

#define PI 3.1416

//-------------------------
// OpenGL functions
void display();
void special_input(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void idle();

// 3D Special functions
void positionObserver(float alpha, float beta, int radi);
void draw_edges();

float alpha_angle = 45.0;
float beta_angle = 45.0;
int radi_cam = 450;
float multi = 0.65;
//-------------------------

// Maze size (cells)
int ROWS;
int COLS;

// Max window size (pixels)
int MAX_WIDTH = 1000;
int MAX_HEIGHT = 600;

// min number of cols and rows
int MIN_ROWS_COLS = 15;

// Final window size and projection size (pixels)
int WIDTH;
int HEIGHT;

// Size of a corridor square (pixels)
int sq_size;

long last_t = 0;

Agent pacman;
list<Ghost> ghosts;
list<Food> foodList;

Map map;

void put_food();
void draw_food();
void check_collisions();
void food_collision();
bool collides(pair<float, float> obj1, pair<float, float> obj2);
void move_ghosts_to_base();

int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Usage: ./pacman <half_of_rows> <half_of_columns>\n");
        exit(-1);
    }

    srand(clock());

    // Calculate the number of rows and cols
    ROWS = atoi(argv[1]);
    ROWS = (ROWS % 2 == 0 ? ROWS + 1 : ROWS) * 2+1;
    COLS = atoi(argv[2]);
    COLS = (COLS % 2 == 0 ? COLS + 1 : COLS) * 2 + 1;
    if(ROWS < MIN_ROWS_COLS || COLS <  MIN_ROWS_COLS) {
        printf("The number of half_of_columns and half_of_rows must be greater Than or Equal to %d\n", MIN_ROWS_COLS);
        exit(0);
    }


    map.generate(ROWS,COLS);
    map.print_map();

    // init the windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);

    sq_size = min(MAX_WIDTH / COLS, MAX_HEIGHT / ROWS);

    WIDTH = sq_size * COLS;
    HEIGHT = sq_size * ROWS;
    printf("WIDTH: %i HEIGHT %i\n", WIDTH, HEIGHT);
    set_offset(-300);

    // Generar fantasmes aqui

    pair<int, int> start_positions = map.start_position();
    pacman.initialize(sq_size, sq_size-5, start_positions.first, start_positions.second, map);
    pacman.color = FULVOUS;

    // calculate number of ghosts
    int n_ghosts =  max(COLS, ROWS) / 5;
    for(int i = 0; i < n_ghosts; i++){
        pair<int, int> start_positions = map.base_start_position();
        Ghost ghost;
        ghost.initialize(sq_size, sq_size-5, start_positions.first, start_positions.second, map);
        ghost.initialize_autonomous(i);
        ghosts.push_back(ghost);
    }
    // put food
    put_food();

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutSpecialFunc(special_input);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,HEIGHT-1,0);

    glutMainLoop();
    return 0;
}

void display(){

    // Set wall color as grey
    glClearColor(0.2,0.2,0.2,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    positionObserver(alpha_angle, beta_angle, radi_cam);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH*multi, WIDTH*multi, -HEIGHT*multi, HEIGHT*multi, 10, 2000);

    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    map.draw(sq_size);
    // Draw food
    draw_food();

    // Draw agents
    pacman.draw();

    // Draw ghosts
    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        ghost->draw();
    }

    // draw_edges();

    glutSwapBuffers();

}


void put_food() {
    // calculate food size
    float food_size = sq_size/4;
    food_size = ((int)food_size % 2 == 0) ? food_size +1: food_size;
    for (int y=0; y < map.n_rows; y++) {
        for (int x=0; x < map.n_cols; x++) {
            if(map.mesh[y][x] == CELL_VISITED){
                // calculate cell  position
                float cell_origin_x = x * sq_size;
                float cell_origin_y = y * sq_size;
                // calculate cell center
                float center_d = sq_size / 2;
                float food_d = food_size /2;
                // Calculate food cosition
                float food_x = cell_origin_x + center_d - food_d;
                float food_y = cell_origin_y + center_d - food_d;
                foodList.push_back(Food(food_x, food_y, food_size));
            }
        }
    }
}

void draw_food() {
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food){
        food->draw();
    }
}

void move_ghosts_to_base() {
    std::list<Ghost>::iterator ghost;
    int counter = 0;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        pair<int, int> start_positions = map.base_start_position();
        ghost->is_out = false;
        ghost->initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
        ghost->initialize_autonomous(counter);
        counter++;
    }
}

void food_collision() {
    Food *food_to_remove = 0;
    float dist = sq_size / 2;
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food){
        pair<float, float> obj1 = make_pair(pacman.x, pacman.y);
        pair<float, float> obj2 = make_pair(food->x, food->y);
        if (collides(obj1, obj2)) {
            food_to_remove = &(*food);
        }
    }
    if (food_to_remove != 0){
        foodList.remove(*food_to_remove);
    }
}


void ghost_collision() {
    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        float dx = abs(ghost->x - pacman.x);
        float dy = abs(ghost->y - pacman.y);
        pair<float, float> obj1 = make_pair(pacman.x, pacman.y);
        pair<float, float> obj2 = make_pair(ghost->x, ghost->y);
        if (collides(obj1, obj2)) {
            move_ghosts_to_base();
        }
    }
}


void check_collisions() {
    food_collision();
    ghost_collision();
}

bool collides(pair<float, float> obj1, pair<float, float> obj2) {
    float dist = sq_size/2;
    float dx = abs(obj1.first - obj2.first);
    float dy = abs(obj1.second - obj2.second);
    return dx  +  dy <= dist;
}


void idle() {
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);

    check_collisions();

    pacman.integrate(t-last_t);

    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        int movement = calculate_ghost_behaviour(*ghost, pacman, map);
        ghost->treat_input(movement);
        ghost->integrate(t-last_t);
        ghost->integrate_timer(t-last_t);
        //ghost->generate_new_movement(t-last_t);
    }
    last_t = t;
    glutPostRedisplay();
}

void special_input(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_F1:
            exit(0);
            break;
        default:
            pacman.treat_input(key);
            break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            alpha_angle += 0.5;
            if (alpha_angle >= 360.0){
                alpha_angle -= 360;
            }
            break;
        case 'd':
            alpha_angle -= 0.5;
            if (alpha_angle <= 0.0){
                alpha_angle += 360;
            }
            break;
        case 'w':
            if (beta_angle < 65.0){
                beta_angle += 0.5;
            }
            break;
        case 's':
            if (beta_angle > 30.0){
                beta_angle -= 0.5;
            }
            break;
        case 'e':
            if (multi > 0.45){
                multi -= 0.05;
            }
            break;
        case 'r':
            if (multi < 0.75){
                multi += 0.05;
            }
            break;
    }
}

void positionObserver(float alpha,float beta,int radi) {
  float x,y,z;
  float upx,upy,upz;
  float modul;

  x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
  y = (float)radi*sin(beta*2*PI/360.0);
  z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

  if (beta>0)
    {
      upx=-x;
      upz=-z;
      upy=(x*x+z*z)/y;
    }
  else if(beta==0)
    {
      upx=0;
      upy=1;
      upz=0;
    }
  else
    {
      upx=x;
      upz=z;
      upy=-(x*x+z*z)/y;
    }

  modul=sqrt(upx*upx+upy*upy+upz*upz);

  upx=upx/modul;
  upy=upy/modul;
  upz=upz/modul;

  gluLookAt(x,y,z,    0.0, 0.0, 0.0,     upx,upy,upz);
}

void draw_edges(){
    for (int i = 0; i<2000;i++){
      glColor3f(0, 0, 1);
      glBegin(GL_QUADS);
      glVertex3i(i,0,0);
      glVertex3i(i,5,0);
      glVertex3i(i+5,5,0);
      glVertex3i(0,5,0);
      glEnd();
      glColor3f(0, 1, 0);
      glBegin(GL_QUADS);
      glVertex3i(0,i,0);
      glVertex3i(0,i,5);
      glVertex3i(0,i+5,5);
      glVertex3i(0,0,5);
      glEnd();
      glColor3f(1, 0, 0);
      glBegin(GL_QUADS);
      glVertex3i(0,0,i);
      glVertex3i(5,0,i);
      glVertex3i(5,0,i+5);
      glVertex3i(5,0,0);
      glEnd();
  }
}
