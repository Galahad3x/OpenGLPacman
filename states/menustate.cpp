#include "menustate.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../globals.h"

#include "gamestate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

void MenuState::enter(){
    state = MENUSTATE;

    map.generate(ROWS,COLS);
    map.print_map();
}
void MenuState::exitState(){

}

void MenuState::displayFunc(){
    glClearColor(0.15,0.15,0.15,0.0);
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

    set_material(1.0, 1.0, 1.0);
    map.draw(sq_size);

    /**
     * Draw text  
    **/

    char *c = "Pac-Man";

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0-alpha_angle, 0.0, 0.0, 1.0);
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(-105*0.5*strlen(c), 0.0, 0.0);
        glTranslatef(0.0, 0.0, 100.0);
        glRotatef(90 - beta_angle, 1.0, 0.0 ,0.0);
        for (int i = 0; i < strlen(c); i++)
        {
            glColor3f(1.0,1.0,1.0);
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c[i]);
        }
    glPopMatrix();

    char *c2 = "Press any key to play";

    glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0-alpha_angle, 0.0, 0.0, 1.0);
        glScalef(0.2, 0.2, 0.2);
        glTranslatef(-105*0.5*strlen(c2), 0.0, 0.0);
        glTranslatef(0.0, -200.0, 200.0);
        glRotatef(90 - beta_angle, 1.0, 0.0 ,0.0);
        for (int i = 0; i < strlen(c2); i++)
        {
            glColor3f(1.0,1.0,1.0);
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c2[i]);
        }
    glPopMatrix();
    glEnable(GL_LIGHTING);


    glutSwapBuffers();
}
void MenuState::specialFunc(int key, int x, int y){
    GameState::enter();
    glutPostRedisplay();
}
void MenuState::keyboardFunc(unsigned char key, int x, int y){
    // GO TO GAME STATE
    GameState::enter();
    glutPostRedisplay();
}
void MenuState::idleFunc(){
    alpha_angle += 0.1;
    if (alpha_angle >= 360.0){
        alpha_angle -= 360;
    }
    glutPostRedisplay();
}

MenuState::MenuState(){

}