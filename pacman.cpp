#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include"map.h"

int ROWS;
int COLS;

int MAX_WIDTH = 1000;
int MAX_HEIGHT = 600;

int WIDTH;
int HEIGHT;

int sq_size;

void display();

Map map;

void draw_rectangle(int x, int y, int width, int height){
    glColor3f(0.8,0.8,0);
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

int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Usage: ./pacman <rows> <half_of_columns>\n");
        exit(-1);
    }

    ROWS = atoi(argv[1]);
    COLS = atoi(argv[2])*2+1;
    map.generate(ROWS,COLS);
     // Create a map instance map
    map.print_map();
    // CELL_VISITED constant mark road

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50,50);

    /*
    sq_size = HEIGHT / ROWS;
    while ((sq_size * COLS) > WIDTH){
        sq_size--;
    }
    */
    sq_size = min(MAX_WIDTH / COLS, MAX_HEIGHT / ROWS);

    WIDTH = sq_size * COLS;
    HEIGHT = sq_size * ROWS;

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glutDisplayFunc(display);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,HEIGHT-1,0);

    glutMainLoop();
    return 0;
}

void display(){
    int i, j;

    glClearColor(0.2,0.2,0.2,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            if(map.mesh[i][j] == CELL_VISITED){
                draw_square(j*sq_size, i*sq_size, sq_size);
            }
        }
  	}

    glutSwapBuffers();

}
