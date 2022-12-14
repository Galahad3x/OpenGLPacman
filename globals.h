#include "agent.h"
#include "food.h"

extern float alpha_angle;
extern float beta_angle;
extern int radi_cam;
extern float multi;

extern int WIDTH;
extern int HEIGHT;

extern int ROWS;
extern int COLS;

extern Agent pacman;
extern list<Ghost> ghosts;
extern list<Food> foodList;

extern Map map;

extern int sq_size;

extern long last_t;

extern int stateOfGame;

extern long transition_timer;

extern float agent_size_a;
extern float agent_size_va;

extern int lives_left;

extern float observer_offset_x;
extern float observer_offset_z;