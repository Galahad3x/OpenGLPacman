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
#include "countdownstate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

void draw_food();

void put_food();

float food_size_a = 0;

void MenuState::enter()
{
    stateOfGame = MENUSTATE;

    lives_left = 3;

    map.generate(ROWS, COLS);
    map.print_map();
}
void MenuState::exitState()
{
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food)
    {
        food->size = 0;
    }
    food_size_a = 0;
}

void MenuState::displayFunc()
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

    set_material(1.0, 1.0, 1.0);
    map.draw(sq_size);

    /**
     * Draw text
     **/

    char *c = "Pac-Man";

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

    char *c2 = "Press any key to play";

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

    glutSwapBuffers();
}
void MenuState::specialFunc(int key, int x, int y)
{
    MenuState::exitState();
    MenuToGameState::enter();
}
void MenuState::keyboardFunc(unsigned char key, int x, int y)
{
    // GO TO GAME STATE
    MenuState::exitState();
    MenuToGameState::enter();
}
void MenuState::idleFunc()
{
    alpha_angle += 0.1;
    if (alpha_angle >= 360.0)
    {
        alpha_angle -= 360;
    }
    glutPostRedisplay();
}

MenuState::MenuState()
{
}

float va = 0.0;
float vb = 0.0;

float food_size_va;

void MenuToGameState::enter()
{
    stateOfGame = MENUTOGAMESTATE;
    transition_timer = 1000;

    last_t = glutGet(GLUT_ELAPSED_TIME);

    va = -(alpha_angle - 45) / transition_timer;
    vb = -(beta_angle - 45) / transition_timer;

    float food_size = sq_size / 4;
    food_size = ((int)food_size % 2 == 0) ? food_size + 1 : food_size;

    food_size_va = food_size / transition_timer;

    pair<int, int> start_positions = map.start_position();
    pacman.initialize(sq_size, sq_size - 5, start_positions.first, start_positions.second, map);
    pacman.agent_size = 0;
    pacman.color = FULVOUS_MATERIAL;
    pacman.flashlight = Flashlight();
    pacman.flashlight.light_id = GL_LIGHT1;
    pacman.flashlight.color = WHITE_LIGHT;
    pacman.flashlight.set_direction(-1, 0, 0);
    pacman.flashlight.set_position(pacman.x, sq_size, pacman.y);
    set_lighting_color(pacman.flashlight.light_id, GL_AMBIENT, ZEROS_LIGHT);

    // calculate number of ghosts
    int n_ghosts = max(COLS, ROWS) / 5;
    // ficar llista de ghosts a 0
    ghosts.clear();
    // int n_ghosts = 1;
    for (int i = 0; i < n_ghosts; i++)
    {
        pair<int, int> start_positions = map.base_start_position();
        Ghost ghost;
        ghost.initialize(sq_size, sq_size - 5, start_positions.first, start_positions.second, map);
        ghost.agent_size = 0;
        ghost.initialize_autonomous(i);
        ghost.flashlight = Flashlight();
        ghost.flashlight.light_id = GL_LIGHT2 + i;
        ghost.flashlight.color = SAGE_LIGHT;
        ghost.flashlight.set_direction(-1, 0, 0);
        ghost.flashlight.set_position(ghost.x, sq_size, ghost.y);
        set_lighting_color(ghost.flashlight.light_id, GL_AMBIENT, ZEROS_LIGHT);
        ghosts.push_back(ghost);
    }
    // put food
    put_food();
}
void MenuToGameState::exitState()
{
    CountdownState::enter();
}

void MenuToGameState::displayFunc()
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

    set_material(1.0, 1.0, 1.0);
    map.draw(sq_size);

    draw_food();

    glutSwapBuffers();
}
void MenuToGameState::specialFunc(int key, int x, int y)
{
    glutPostRedisplay();
}
void MenuToGameState::keyboardFunc(unsigned char key, int x, int y)
{
    glutPostRedisplay();
}
void MenuToGameState::idleFunc()
{
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);
    long time_elapsed = t - last_t;
    last_t = t;
    transition_timer -= time_elapsed;
    if (transition_timer > 0)
    {
        alpha_angle += va * time_elapsed;
        beta_angle += vb * time_elapsed;
        food_size_a += food_size_va * time_elapsed;
        std::list<Food>::iterator food;
        for (food = foodList.begin(); food != foodList.end(); ++food)
        {
            food->size = (int)food_size_a;
        }
    }
    else
    {
        MenuToGameState::exitState();
    }
    glutPostRedisplay();
}

MenuToGameState::MenuToGameState()
{
}