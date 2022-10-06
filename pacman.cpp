#include <GL/glut.h>
#include <stdio.h>

#define COLUMNS 8
#define ROWS 8
#define WIDTH 800
#define HEIGHT 600

//-----------------------------------------------

void display();;
void keyboard(unsigned char c,int x,int y);

//-----------------------------------------------

class Maze {
    public:
        int n_rows;
        int n_cols;
};

//-----------------------------------------------
// -- MAIN PROCEDURE
//-----------------------------------------------

int main(int argc,char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

    glutMainLoop();
    return 0;
}

//------------------------------------------------------------
//------------------------------------------------------------

void display() {

}

//-----------------------------------------------
//-----------------------------------------------
void keyboard(unsigned char c,int x,int y) {

};
