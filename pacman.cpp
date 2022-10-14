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

int WIDTH = 800;
int HEIGHT = 600;

int sq_size;

void display();

Map map;

void draw_square(int x, int y, int size){
    glColor3f(0.8,0.8,0);
    glBegin(GL_QUADS);

    glVertex2i(x,y);
    glVertex2i(x,y+size);
    glVertex2i(x+size, y+size);
    glVertex2i(x+size, y);

    glEnd();
}


int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Usage: ./pacman <rows> <half_of_columns>\n");
        exit(-1);
    }

    ROWS = atoi(argv[1]);
    ROWS = (ROWS % 2 == 0 ? ROWS + 1 : ROWS) *2+1;
    COLS = atoi(argv[2]);
    COLS = (COLS % 2 == 0 ? COLS + 1 : COLS) *2+1;
    map.generate(ROWS,COLS);
     // Create a map instance map
    map.print_map();
    // CELL_VISITED constant mark road

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(150, 50);

    sq_size = HEIGHT / ROWS;
    while ((sq_size * COLS) > WIDTH){
        sq_size--;
    }

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
