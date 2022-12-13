#include "countdownstate.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>

#include "../globals.h"

#include "gamestate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

void draw_food();

// items to animate
float agent_size_a = 0;
float agent_size_va;
float quadratic_a = 0.005;
float quadratic_va;

void move_ghosts_to_base();

void CountdownState::enter()
{
    // El que sa de carregar al iniciar el joc
    stateOfGame = COUNTDOWNSTATE;

    transition_timer = 1200;

    agent_size_va = (sq_size - 5.0) / transition_timer;
    quadratic_va = (0.00007 - quadratic_a) / transition_timer;
}
void CountdownState::exitState()
{
    // El que sa d'executar al sortir de joc
    GameState::enter();
}

void CountdownState::displayFunc()
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
    glLightf(pacman.flashlight.light_id, GL_QUADRATIC_ATTENUATION, quadratic_a);

    pacman.flashlight.draw();

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        glLightf(ghost->flashlight.light_id, GL_CONSTANT_ATTENUATION, 0.08);
        glLightf(ghost->flashlight.light_id, GL_LINEAR_ATTENUATION, 0.009);
        glLightf(ghost->flashlight.light_id, GL_QUADRATIC_ATTENUATION, quadratic_a);
        ghost->flashlight.set_to_direction(ghost->direction);
        ghost->flashlight.draw();
    }

    set_material(1.0, 1.0, 1.0);
    map.draw(sq_size);

    // Draw food
    draw_food();

    // Draw agents
    pacman.draw();

    // Draw ghosts
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        ghost->draw();
    }

    // draw_edges();

    glutSwapBuffers();
}
void CountdownState::specialFunc(int key, int x, int y)
{
    // Funcio de tecles especials del joc
}
void CountdownState::keyboardFunc(unsigned char key, int x, int y)
{
    // Funcio de tecles del joc
}
void CountdownState::idleFunc()
{
    // Funcio idle del joc
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);
    long time_elapsed = t - last_t;
    transition_timer -= time_elapsed;
    if (transition_timer < 0)
    {
        GameState::enter();
    }
    else
    {
        quadratic_a += quadratic_va * time_elapsed;
        agent_size_a += agent_size_va * time_elapsed;
        pacman.agent_size = (int)agent_size_a;
        std::list<Ghost>::iterator ghost;
        for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
        {
            ghost->agent_size = (int)agent_size_a;
        }
    }
    last_t = t;
    glutPostRedisplay();
}

CountdownState::CountdownState()
{
}