#include "losestate.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../globals.h"

#include "gamestate.h"
#include "menustate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

void draw_food();
int adapt_to_cam(int key);

void check_collisions();
void food_collision();
void ghost_collision();
bool collides(pair<float, float> obj1, pair<float, float> obj2, float size_obj1, float size_obj2);

void move_ghosts_to_base();

void LoseState::enter()
{
    // El que sa de carregar al iniciar el joc
    stateOfGame = LOSESTATE;
}
void LoseState::exitState()
{
    // El que sa d'executar al sortir de joc
}

void LoseState::displayFunc()
{
    // Funcio display del joc
    // Set wall color as grey
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

    // Draw ghosts
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        ghost->draw();
    }

    char *c = "You lost!";

    glDisable(GL_LIGHTING);
    glPushMatrix();
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

    glutSwapBuffers();
}
void LoseState::specialFunc(int key, int x, int y)
{
    // Funcio de tecles especials del joc
    MenuState::enter();
}
void LoseState::keyboardFunc(unsigned char key, int x, int y)
{
    // Funcio de tecles del joc
    MenuState::enter();
}
void LoseState::idleFunc()
{
    // Funcio idle del joc
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

LoseState::LoseState()
{
}

long wait_timer;

void GameToLoseState::enter()
{
    // El que sa de carregar al iniciar el joc
    stateOfGame = GAMETOLOSESTATE;

    agent_size_a = sq_size - 5.0;
    agent_size_va = 0.0;

    transition_timer = 600;
    wait_timer = 1000;

    agent_size_va = -agent_size_a / transition_timer;
}
void GameToLoseState::exitState()
{
    // El que sa d'executar al sortir de joc
}

void GameToLoseState::displayFunc()
{
    // Funcio display del joc
    // Set wall color as grey
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

    pacman.agent_size = agent_size_a;
    pacman.draw();

    // Draw ghosts
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        ghost->draw();
    }

    glutSwapBuffers();
}
void GameToLoseState::specialFunc(int key, int x, int y)
{
    // Funcio de tecles especials del joc
}
void GameToLoseState::keyboardFunc(unsigned char key, int x, int y)
{
    // Funcio de tecles del joc
}
void GameToLoseState::idleFunc()
{
    // Funcio idle del joc
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);
    long time_elapsed = t - last_t;
    if (wait_timer <= 0)
    {
        if (transition_timer <= 0)
        {
            // Depen de les vides seguir o acabar
            LoseState::enter();
        }
        else
        {
            transition_timer -= time_elapsed;
            agent_size_a += agent_size_va * time_elapsed;
        }
    }
    else
    {
        wait_timer -= time_elapsed;
    }
    last_t = t;
    glutPostRedisplay();
}

GameToLoseState::GameToLoseState()
{
}