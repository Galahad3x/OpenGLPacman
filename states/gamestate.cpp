#include "gamestate.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>

#include "../globals.h"

#include "losestate.h"
#include "winstate.h"

#include "../graphic.h"
#include "../lighting.h"
#include "../map.h"
#include "../ghost.h"

#include "../agents/reflexagent.h"
#include "../agents/minimaxagent.h"

void draw_food();
int adapt_to_cam(int key);

void check_collisions();
int food_collision();
int ghost_collision();
bool collides(pair<float, float> obj1, pair<float, float> obj2, float size_obj1, float size_obj2);

void move_ghosts_to_base();

float quadratic;

void GameState::enter()
{
    // El que sa de carregar al iniciar el joc
    stateOfGame = GAMESTATE;

    // alpha_angle = 45.0;
    // beta_angle = 45.0;

    quadratic = 0.00009;
}
void GameState::exitState()
{
    // El que sa d'executar al sortir de joc
}

void GameState::displayFunc()
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
    glOrtho(-WIDTH * multi, WIDTH * multi, -HEIGHT * multi, HEIGHT * multi, 0, 2000);

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
    glLightf(pacman.flashlight.light_id, GL_QUADRATIC_ATTENUATION, quadratic);

    pacman.flashlight.draw();

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        glLightf(ghost->flashlight.light_id, GL_CONSTANT_ATTENUATION, 0.08);
        glLightf(ghost->flashlight.light_id, GL_LINEAR_ATTENUATION, 0.009);
        glLightf(ghost->flashlight.light_id, GL_QUADRATIC_ATTENUATION, quadratic);
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

    // Lives left
    for (int l = 0; l < lives_left; l++)
    {
        set_material_id(FULVOUS_MATERIAL);
        draw_sphere(pacman.agent_size / 2.0, sq_size * map.n_cols + sq_size, 0, sq_size * map.n_rows - (sq_size + 8) * l);
    }

    // draw_edges();

    glutSwapBuffers();
}
void GameState::specialFunc(int key, int x, int y)
{
    // Funcio de tecles especials del joc
    switch (key)
    {
    case GLUT_KEY_F1:
        exit(0);
        break;
    default:
        if (autonomous_agent == 0)
        {
            pacman.treat_input(adapt_to_cam(key));
        }
        break;
    }
    glutPostRedisplay();
}
void GameState::keyboardFunc(unsigned char key, int x, int y)
{
    // Funcio de tecles del joc
    switch (key)
    {
    case 'a':
        alpha_angle += 0.5;
        if (alpha_angle >= 360.0)
        {
            alpha_angle -= 360;
        }
        break;
    case 'd':
        alpha_angle -= 0.5;
        if (alpha_angle <= 0.0)
        {
            alpha_angle += 360;
        }
        break;
    case 'w':
        if (beta_angle < 80.0)
        {
            beta_angle += 0.5;
        }
        break;
    case 's':
        if (beta_angle > -80.0)
        {
            beta_angle -= 0.5;
        }
        break;
    case 'e':
        if (multi > 0.45)
        {
            multi -= 0.05;
        }
        break;
    case 'q':
        if (multi < 0.75)
        {
            multi += 0.05;
        }
        break;
    }
}
void GameState::idleFunc()
{
    // Funcio idle del joc
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);

    check_collisions();

    pacman.integrate(t - last_t);

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        int movement = calculate_ghost_behaviour(*ghost, pacman, map);
        ghost->treat_input(movement);
        ghost->integrate(t - last_t);
        ghost->integrate_timer(t - last_t);
    }
    last_t = t;

    /*
    if (autonomous_agent == 1)
    {
        pacman.treat_input(ReflexAgent::getBestAction());
    }
    else if (autonomous_agent == 2)
    {
        pacman.treat_input(MinimaxAgent::getBestAction());
    }
    */

    glutPostRedisplay();
}

GameState::GameState()
{
}

void draw_food()
{
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food)
    {
        food->draw();
    }
}

int adapt_to_cam(int key)
{
    if (alpha_angle >= 315 || alpha_angle < 45)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            return GLUT_KEY_LEFT;
        case GLUT_KEY_DOWN:
            return GLUT_KEY_RIGHT;
        case GLUT_KEY_LEFT:
            return GLUT_KEY_DOWN;
        case GLUT_KEY_RIGHT:
            return GLUT_KEY_UP;
        }
    }
    else if (alpha_angle < 135)
    {
        return key;
    }
    else if (alpha_angle < 225)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            return GLUT_KEY_RIGHT;
        case GLUT_KEY_DOWN:
            return GLUT_KEY_LEFT;
        case GLUT_KEY_LEFT:
            return GLUT_KEY_UP;
        case GLUT_KEY_RIGHT:
            return GLUT_KEY_DOWN;
        }
    }
    else
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            return GLUT_KEY_DOWN;
        case GLUT_KEY_DOWN:
            return GLUT_KEY_UP;
        case GLUT_KEY_LEFT:
            return GLUT_KEY_RIGHT;
        case GLUT_KEY_RIGHT:
            return GLUT_KEY_LEFT;
        }
    }
    return key;
}

void check_collisions()
{
    if (food_collision() == 1)
    {
        WinState::enter();
    }
    if (ghost_collision() == 1)
    {
        GameToLoseState::enter();
    }
}

int food_collision()
{
    Food *food_to_remove = 0;
    float dist = sq_size / 2;
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food)
    {
        pair<float, float> obj1 = make_pair(pacman.x + pacman.agent_size / 2, pacman.y + pacman.agent_size / 2);
        pair<float, float> obj2 = make_pair(food->x, food->y);
        if (collides(obj1, obj2, pacman.agent_size, food->size * 2))
        {
            food_to_remove = &(*food);
        }
    }
    if (food_to_remove != 0)
    {
        foodList.remove(*food_to_remove);
    }
    if (foodList.size() == 0)
    {
        return 1;
    }
    return 0;
}

int ghost_collision()
{
    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        pair<float, float> obj1 = make_pair(pacman.x + pacman.agent_size / 2, pacman.y + pacman.agent_size / 2);
        pair<float, float> obj2 = make_pair(ghost->x, ghost->y);
        if (collides(obj1, obj2, pacman.agent_size, pacman.agent_size))
        {
            return 1;
        }
    }
    return 0;
}

bool collides(pair<float, float> obj1, pair<float, float> obj2, float size_obj1, float size_obj2)
{
    float dist = sq_size / 5;
    float dx = pow(obj1.first - obj2.first, 2);
    float dy = pow(obj1.second - obj2.second, 2);
    float distance = sqrt(dx + dy);
    return distance <= ((size_obj1 / 2.0) + (size_obj2 / 2.0) - 3);
}

void move_ghosts_to_base()
{
    std::list<Ghost>::iterator ghost;
    int counter = 0;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        pair<int, int> start_positions = map.base_start_position();
        ghost->is_out = false;
        ghost->initialize(sq_size, sq_size - 7, start_positions.first, start_positions.second, map);
        ghost->initialize_autonomous(counter);
        counter++;
    }
}
