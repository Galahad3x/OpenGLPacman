#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include <algorithm>
#include<time.h>
#include<list>
#include<tgmath.h>
#include"graphic.h"
#include"map.h"
#include"agent.h"
#include "food.h"



//-------------------------
// OpenGL functions
void display();
void special_input(int key, int x, int y);
void idle();
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


// Map object, not initialized
Map map;

void put_food();
void draw_food();
void check_collisions();
void food_collision();
bool have_collision(pair<float, float> obj1, pair<float, float> obj2);
void move_ghosts_to_base();

int calculate_ghost_behaviour(Ghost ghost, Agent agent);
int calculate_next_ghost_move(Ghost ghost, int x, int y);

int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Usage: ./pacman <half_of_rows> <half_of_columns>\n");
        exit(-1);
    }

    srand(clock());

    // Calculate the number of rows and cols
    ROWS = atoi(argv[1]);
    ROWS = (ROWS % 2 == 0 ? ROWS + 1 : ROWS) * 2+1;
    COLS = atoi(argv[2]);
    COLS = (COLS % 2 == 0 ? COLS + 1 : COLS) * 2 + 1;
    if(ROWS < MIN_ROWS_COLS || COLS <  MIN_ROWS_COLS) {
        printf("The number of half_of_columns and half_of_rows must be greater Than or Equal to %d\n", MIN_ROWS_COLS);
        exit(0);
    }


    map.generate(ROWS,COLS);
    map.print_map();

    // init the windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50,50);

    sq_size = min(MAX_WIDTH / COLS, MAX_HEIGHT / ROWS);

    WIDTH = sq_size * COLS;
    HEIGHT = sq_size * ROWS;
    // Generar fantasmes aqui

    pair<int, int> start_positions = map.start_position();
    pacman.initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
    pacman.color = ORANGE;

    // calculate number of ghosts
    int n_ghosts =  max(COLS, ROWS) / 5;
    for(int i = 0; i < n_ghosts; i++){
        pair<int, int> start_positions = map.base_start_position();
        Ghost ghost;
        ghost.initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
        ghost.color = RED;
        ghost.is_out = false;
        ghost.is_autonomous = true;
        ghost.timer = 0;
        ghost.exit_timer = 10000 * i;
        ghost.chase_timer = 10000;
        ghost.scatter_timer = 8000;
        ghosts.push_back(ghost);
    }
    // put food
    put_food();

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac-Man");

    glutDisplayFunc(display);
    glutSpecialFunc(special_input);
    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,WIDTH-1,HEIGHT-1,0);

    glutMainLoop();
    return 0;
}

void display(){
    int i, j;

    // Set wall color as grey
    glClearColor(0.2,0.2,0.2,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    map.draw(sq_size);
    // Draw food
    draw_food();

    // Draw agents
    pacman.draw();

    // Draw ghosts
    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        ghost->draw();
    }

    glutSwapBuffers();

}


void put_food() {
    // calculate food size
    float food_size = sq_size/4;
    food_size = ((int)food_size % 2 == 0) ? food_size +1: food_size;
    for (int y=0; y < map.n_rows; y++) {
        for (int x=0; x < map.n_cols; x++) {
            if(map.mesh[y][x] == CELL_VISITED){
                // calculate cell  position
                float cell_origin_x = x * sq_size;
                float cell_origin_y = y * sq_size;
                // calculate cell center
                float center_d = sq_size / 2;
                float food_d = food_size /2;
                // Calculate food cosition
                float food_x = cell_origin_x + center_d - food_d;
                float food_y = cell_origin_y + center_d - food_d;
                foodList.push_back(Food(food_x, food_y, food_size));
            }
        }
    }
}

void draw_food() {
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food){
        food->draw();
    }
}

void move_ghosts_to_base() {
    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        pair<int, int> start_positions = map.base_start_position();
        ghost->is_out = false;
        ghost->initialize(sq_size, sq_size-7, start_positions.first, start_positions.second, map);
    }
}

void food_collision() {
    Food *food_to_remove = 0;
    float dist = sq_size / 2;
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food){
        pair<float, float> obj1 = make_pair(pacman.x, pacman.y);
        pair<float, float> obj2 = make_pair(food->x, food->y);
        if (have_collision(obj1, obj2)) {
            food_to_remove = &(*food);
        }
    }
    if (food_to_remove != 0){
        foodList.remove(*food_to_remove);
    }
}


void ghost_collision() {
    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        float dx = abs(ghost->x - pacman.x);
        float dy = abs(ghost->y - pacman.y);
        pair<float, float> obj1 = make_pair(pacman.x, pacman.y);
        pair<float, float> obj2 = make_pair(ghost->x, ghost->y);
        if (have_collision(obj1, obj2)) {
            move_ghosts_to_base();
        }
    }
}


void check_collisions() {
    food_collision();
    ghost_collision();
}

bool have_collision(pair<float, float> obj1, pair<float, float> obj2) {
    float dist = sq_size/2;
    float dx = abs(obj1.first - obj2.first);
    float dy = abs(obj1.second - obj2.second);
    return dx  +  dy <= dist;
}


void idle() {
    long t;
    t = glutGet(GLUT_ELAPSED_TIME);

    check_collisions();

    pacman.integrate(t-last_t);

    std::list<Ghost>::iterator ghost;
    for(ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost){
        int movement = calculate_ghost_behaviour(*ghost, pacman);
        ghost->treat_input(movement);
        ghost->integrate(t-last_t);
        ghost->integrate_timer(t-last_t);
        //ghost->generate_new_movement(t-last_t);
    }
    last_t = t;
    glutPostRedisplay();
}

void special_input(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_F1:
            exit(0);
            break;
        default:
            pacman.treat_input(key);
            break;
    }
    glutPostRedisplay();
}

float pythagoras(int x1, int y1, int x2, int y2){
    return sqrt(((abs(x1-x2)*abs(x1-x2))+(abs(y1-y2)*abs(y1-y2)))*1.0);
}

int calculate_ghost_behaviour(Ghost ghost, Agent agent){
    if (ghost.is_out == false && ghost.behave_state != HOUSE){
        pair<int, int> pos = map.get_exit_base_position();
        return calculate_next_ghost_move(ghost, pos.first, pos.second);
    }else if (ghost.behave_state == SCATTER){
        return calculate_next_ghost_move(ghost, ghost.corner_x, ghost.corner_y);
    }else if (ghost.behave_state == CHASE){
        return calculate_next_ghost_move(ghost, pacman.grid_x, pacman.grid_y);
    } else {
        return ghost.get_random_direction();
    }
    return GLUT_KEY_UP;
}

int calculate_next_ghost_move(Ghost ghost, int x, int y){
    int p = 4;
    int directions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    float scores[] = {0,0,0,0};
    for (int i = 0; i < p; i++){
        switch (directions[i]) {
            case GLUT_KEY_UP:
                scores[i] = pythagoras(ghost.grid_x, ghost.grid_y-1, x, y);
                break;
            case GLUT_KEY_DOWN:
                scores[i] = pythagoras(ghost.grid_x, ghost.grid_y+1, x, y);
                break;
            case GLUT_KEY_LEFT:
                scores[i] = pythagoras(ghost.grid_x-1, ghost.grid_y, x, y);
                break;
            case GLUT_KEY_RIGHT:
                scores[i] = pythagoras(ghost.grid_x+1, ghost.grid_y, x, y);
                break;
        }
    }
    float min_score = 999999.0;
    int best_move = -1;
    for (int i = 0; i < p; i++){
        // Canviar true per normes
        if (scores[i] < min_score && ghost.is_not_turn(directions[i]) && ghost.next_move_valid(directions[i])){
            best_move = directions[i];
            min_score = scores[i];
        }
    }
    return best_move;
}
