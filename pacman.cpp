#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include <algorithm>
#include"graphic.h"
#include"map.h"
#include"agent.h"



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

Agent some_agent;

// Map object, not initialized
Map map;

int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Usage: ./pacman <half_of_rows> <half_of_columns>\n");
        exit(-1);
    }


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
    // TODO Generate random valid start point
    some_agent.initialize(sq_size, sq_size-7);

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

    set_3f_color(COOL_BLUE);
    // Print corridor colors
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            if(map.mesh[i][j] == CELL_VISITED){
                draw_square(j*sq_size, i*sq_size, sq_size);
            }
        }
  	}

    // Draw food

    // Draw agents
    some_agent.draw();

    glutSwapBuffers();

}

void idle() {
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);

    some_agent.integrate(t-last_t);

    last_t = t;
    glutPostRedisplay();
}

void special_input(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_F1:
            exit(0);
            break;
        default:
            some_agent.treat_input(key);
            break;
    }
    glutPostRedisplay();
}
