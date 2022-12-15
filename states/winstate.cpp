#include "winstate.h"

#include <stdio.h>
#include <string.h>

#include "../globals.h"

#include "gamestate.h"
#include "menustate.h"
#include "countdownstate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

void draw_food();

void WinState::enter()
{
    stateOfGame = WINSTATE;
}
void WinState::exitState()
{
}

void WinState::displayFunc()
{
    glClearColor(0.15, 0.15, 0.15, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    positionObserver(alpha_angle, beta_angle, radi_cam);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH * multi, WIDTH * multi, -HEIGHT * multi, HEIGHT * multi, 10, 2000);

    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    //--------Ambient light---------
    set_directional_light(GL_LIGHT0, 0, 0, 0);
    set_lighting_color(GL_LIGHT0, GL_AMBIENT, AMBIENT_LIGHT);
    // glEnable(GL_LIGHT0);
    //------------------------------

    pacman.flashlight.set_to_direction(pacman.direction);

    glLightf(pacman.flashlight.light_id, GL_CONSTANT_ATTENUATION, 0.1);
    // glLightf(this->light_id,GL_LINEAR_ATTENUATION,0.009);
    glLightf(pacman.flashlight.light_id, GL_QUADRATIC_ATTENUATION, 0.00009);

    pacman.flashlight.draw();

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        glLightf(ghost->flashlight.light_id, GL_CONSTANT_ATTENUATION, 0.08);
        glLightf(ghost->flashlight.light_id, GL_LINEAR_ATTENUATION, 0.009);
        glLightf(ghost->flashlight.light_id, GL_QUADRATIC_ATTENUATION, 0.00009);
        ghost->flashlight.set_to_direction(ghost->direction);
        ghost->flashlight.draw();
    }

    set_material(1.0, 1.0, 1.0);
    map.draw(sq_size);

    // Draw food
    draw_food();

    pacman.draw();

    // Draw ghosts
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        ghost->draw();
    }

    char *c = "You win!";

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(observer_offset_x, 0, observer_offset_z);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(90.0 - alpha_angle, 0.0, 0.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    glTranslatef(-105 * 0.5 * strlen(c), 0.0, 0.0);
    glTranslatef(0.0, 0.0, 100.0);
    glRotatef(90 - beta_angle, 1.0, 0.0, 0.0);
    for (int i = 0; i < strlen(c); i++)
    {
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c[i]);
    }
    glPopMatrix();

    char *c2 = "Press any key to restart";

    glPushMatrix();
    glTranslatef(observer_offset_x, 0, observer_offset_z);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(90.0 - alpha_angle, 0.0, 0.0, 1.0);
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(-105 * 0.5 * strlen(c2), 0.0, 0.0);
    glTranslatef(0.0, -200.0, 200.0);
    glRotatef(90 - beta_angle, 1.0, 0.0, 0.0);
    for (int i = 0; i < strlen(c2); i++)
    {
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c2[i]);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);

    for (int l = 0; l < lives_left; l++)
    {
        set_material_id(FULVOUS_MATERIAL);
        draw_sphere(pacman.agent_size / 2.0, sq_size * map.n_cols + sq_size, 0, sq_size * map.n_rows - (sq_size + 8) * l);
    }

    glutSwapBuffers();
}
void WinState::specialFunc(int key, int x, int y)
{
    MenuState::enter();
}
void WinState::keyboardFunc(unsigned char key, int x, int y)
{
    MenuState::enter();
}
void WinState::idleFunc()
{
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);
    alpha_angle += 0.1;
    if (alpha_angle >= 360.0)
    {
        alpha_angle -= 360;
    }
    last_t = t;
    glutPostRedisplay();
}

WinState::WinState()
{
}