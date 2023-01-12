#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <list>
#include <tgmath.h>
#include "graphic.h"
#include "map.h"
#include "agent.h"
#include "food.h"
#include "ghost.h"
#include "texture.h"
#include "lighting.h"

#include "states/gamestate.h"
#include "states/menustate.h"
#include "states/countdownstate.h"
#include "states/losestate.h"
#include "states/winstate.h"

//-------------------------
// OpenGL functions
void display();
void special_input(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void idle();

// 3D Special functions
void draw_edges();

float alpha_angle = 45.0;
float beta_angle = 30.0;
int radi_cam = 450;
float multi = 0.65;
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

Agent pacman;
list<Ghost> ghosts;
list<Food> foodList;

Map map;

int stateOfGame;

long transition_timer;

float agent_size_a;
float agent_size_va;

int lives_left;

void put_food();

float observer_offset_x;
float observer_offset_z;

int autonomous_agent;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: ./pacman <half_of_rows> <half_of_columns> [autonomous_agent]\n");
        exit(-1);
    }

    if (argc > 3)
    {
        autonomous_agent = atoi(argv[3]);
    }
    else
    {
        autonomous_agent = 0;
    }
    printf("%i\n", autonomous_agent);
    srand(clock());

    // Calculate the number of rows and cols
    ROWS = atoi(argv[1]);
    ROWS = (ROWS % 2 == 0 ? ROWS + 1 : ROWS) * 2 + 1;
    COLS = atoi(argv[2]);
    COLS = (COLS % 2 == 0 ? COLS + 1 : COLS) * 2 + 1;
    if (ROWS < MIN_ROWS_COLS || COLS < MIN_ROWS_COLS)
    {
        printf("The number of half_of_columns and half_of_rows must be greater Than or Equal to %d\n", MIN_ROWS_COLS);
        exit(0);
    }

    MenuState::enter();

    // init the windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);

    sq_size = min(MAX_WIDTH / COLS, MAX_HEIGHT / ROWS);

    WIDTH = sq_size * COLS;
    HEIGHT = sq_size * ROWS;
    set_offset(0);
    set_light_offset(0);

    observer_offset_x = (sq_size * map.n_cols) / 2.0;
    observer_offset_z = (sq_size * map.n_rows) / 2.0;

    // Generar fantasmes aqui

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glutDisplayFunc(display);
    glutSpecialFunc(special_input);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH - 1, HEIGHT - 1, 0);

    /*--------Loading textures----*/
    glBindTexture(GL_TEXTURE_2D, GRASS);
    LoadTexture("assets/gespa.jpg", 64);
    glBindTexture(GL_TEXTURE_2D, COBBLESTONE);
    LoadTexture("assets/cobble2.jpg", 64);
    /*-----------------------------*/
    glutMainLoop();

    return 0;
}

void display()
{
    switch (stateOfGame)
    {
    case MENUSTATE:
        MenuState::displayFunc();
        break;
    case GAMESTATE:
        GameState::displayFunc();
        break;
    case MENUTOGAMESTATE:
        MenuToGameState::displayFunc();
        break;
    case COUNTDOWNSTATE:
        CountdownState::displayFunc();
        break;
    case LOSESTATE:
        LoseState::displayFunc();
        break;
    case GAMETOLOSESTATE:
        GameToLoseState::displayFunc();
        break;
    case WINSTATE:
        WinState::displayFunc();
        break;
    default:
        break;
    }
}

void idle()
{
    switch (stateOfGame)
    {
    case MENUSTATE:
        MenuState::idleFunc();
        break;
    case GAMESTATE:
        GameState::idleFunc();
        break;
    case MENUTOGAMESTATE:
        MenuToGameState::idleFunc();
        break;
    case COUNTDOWNSTATE:
        CountdownState::idleFunc();
        break;
    case LOSESTATE:
        LoseState::idleFunc();
        break;
    case GAMETOLOSESTATE:
        GameToLoseState::idleFunc();
        break;
    case WINSTATE:
        WinState::idleFunc();
        break;
    default:
        break;
    }
}

void special_input(int key, int x, int y)
{
    switch (stateOfGame)
    {
    case MENUSTATE:
        MenuState::specialFunc(key, x, y);
        break;
    case GAMESTATE:
        GameState::specialFunc(key, x, y);
        break;
    case MENUTOGAMESTATE:
        MenuToGameState::specialFunc(key, x, y);
        break;
    case COUNTDOWNSTATE:
        CountdownState::specialFunc(key, x, y);
        break;
    case LOSESTATE:
        LoseState::specialFunc(key, x, y);
        break;
    case GAMETOLOSESTATE:
        GameToLoseState::specialFunc(key, x, y);
        break;
    case WINSTATE:
        WinState::specialFunc(key, x, y);
        break;
    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (stateOfGame)
    {
    case MENUSTATE:
        MenuState::keyboardFunc(key, x, y);
        break;
    case GAMESTATE:
        GameState::keyboardFunc(key, x, y);
        break;
    case MENUTOGAMESTATE:
        MenuToGameState::keyboardFunc(key, x, y);
        break;
    case COUNTDOWNSTATE:
        CountdownState::specialFunc(key, x, y);
        break;
    case LOSESTATE:
        LoseState::specialFunc(key, x, y);
        break;
    case GAMETOLOSESTATE:
        GameToLoseState::specialFunc(key, x, y);
        break;
    case WINSTATE:
        WinState::specialFunc(key, x, y);
        break;
    default:
        break;
    }
}

void put_food()
{
    // calculate food size
    float food_size = sq_size / 4;
    food_size = ((int)food_size % 2 == 0) ? food_size + 1 : food_size;
    food_size = 0;
    foodList.clear();
    for (int y = 0; y < map.n_rows; y++)
    {
        for (int x = 0; x < map.n_cols; x++)
        {
            if (map.mesh[y][x] == CELL_VISITED)
            {
                // calculate cell  position
                float cell_origin_x = x * sq_size;
                float cell_origin_y = y * sq_size;
                // calculate cell center
                float center_d = sq_size / 2;
                float food_d = food_size / 2;
                // Calculate food cosition
                float food_x = cell_origin_x + center_d - food_d;
                float food_y = cell_origin_y + center_d - food_d;
                foodList.push_back(Food(food_x, food_y, food_size));
            }
        }
    }
}

void draw_edges()
{
    for (int i = 0; i < 2000; i++)
    {
        glColor3f(0, 0, 1);
        glBegin(GL_QUADS);
        glVertex3i(i, 0, 0);
        glVertex3i(i, 5, 0);
        glVertex3i(i + 5, 5, 0);
        glVertex3i(0, 5, 0);
        glEnd();
        set_material(0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3i(0, i, 0);
        glVertex3i(0, i, 5);
        glVertex3i(0, i + 5, 5);
        glVertex3i(0, 0, 5);
        glEnd();
        set_material(0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        glVertex3i(0, 0, i);
        glVertex3i(5, 0, i);
        glVertex3i(5, 0, i + 5);
        glVertex3i(5, 0, 0);
        glEnd();
    }
}
