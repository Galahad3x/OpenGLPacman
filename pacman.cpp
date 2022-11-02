#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include <algorithm>
#include<time.h>
#include"graphic.h"
#include"map.h"
#include"agent.h"
#include "food.h"



//-------------------------
// OpenGL functions
void display();
void special_input(int key, int x, int y);
void idle();
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
Ghost ghosts[3];

// Map object, not initialized
Map map;
void put_food();

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50,50);

    sq_size = min(MAX_WIDTH / COLS, MAX_HEIGHT / ROWS);

    WIDTH = sq_size * COLS;
    HEIGHT = sq_size * ROWS;

    // Generar fantasmes aqui

    pair<int, int> start_positions = map.start_position();
    pacman.initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
    pacman.color = ORANGE;

    for(int ag = 0; ag<3;ag++){
        pair<int, int> start_positions = map.start_position();
        ghosts[ag].initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
        ghosts[ag].color = RED;
    }

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glutDisplayFunc(display);
    glutSpecialFunc(special_input);
    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,HEIGHT-1,0);

    glutMainLoop();
    return 0;
}

void display(){
    int i, j;

    // Set wall color as grey
    glClearColor(0.2,0.2,0.2,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    map.draw(sq_size);

    // Afegir lo que falti de baix a dins de map.draw()

    set_3f_color(ORANGE);
    // Print corridor colors
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            if(map.mesh[i][j] == CELL_VISITED || map.mesh[i][j] == BASE_CELL){
                draw_square(j*sq_size, i*sq_size, sq_size);
            }
        }
  	}
    // Draw food

    // Draw agents
    pacman.draw();

    for(int ag = 0; ag<3;ag++){
        ghosts[ag].draw();
    }

    glutSwapBuffers();

}


void put_food() {
    float food_size = 7;
    for (int y=0; y < map.n_rows; y++) {
        for (int x=0; x < map.n_cols; x++) {

            // calculate cell  position
            float cell_origin_x = x * sq_size;
            float cell_origin_y = y * sq_size;

            // calculate cell center
            float center_d = sq_size / 2;
            float food_d = food_size /2;

            // Calculate food cosition
            float food_x = cell_origin_x + center_d - food_d;
            float food_y = cell_origin_y + center_d - food_d;

            if(map.mesh[y][x] == CELL_VISITED){
                // put food
                Food(food_x, food_y, food_size).draw();
            }
        }
    }
}
void idle() {
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);

    pacman.integrate(t-last_t);

    for(int ag = 0; ag<3;ag++){
        ghosts[ag].integrate(t-last_t);
        ghosts[ag].generate_new_movement(t-last_t);
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
