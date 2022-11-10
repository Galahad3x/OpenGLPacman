#include "agent.h"
#include "graphic.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void Agent::initialize(int sq_size,int agent_size,int grid_x,int grid_y, Map map){
    this->state = STILL;
    this->speed = 250.0;

    this->grid_x = grid_x;
    this->grid_y = grid_y;

    this->dist = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + this->dist;
    this->y = this->grid_y*sq_size + this->dist;
    this->sq_size = sq_size;
    this->agent_size = agent_size;

    this->map = map;
}

Agent::Agent(){}

Agent::Agent(int sq_size,int agent_size,int grid_x,int grid_y, Map map){
    this->initialize(sq_size, agent_size, grid_x, grid_y, map);
}

void Agent::set_position(float x,float y) {
    this->x = x;
    this->y = y;
}

void Agent::draw() {
    set_3f_color(this->color);
    //set_raised(1);
    //draw_square((int) x, (int) y, agent_size);
    //draw_prism(x,0,y,agent_size, agent_size, agent_size);
    //set_raised(0);

    set_3f_color(this->color);
    draw_sphere(agent_size / 2, (int) x + agent_size/2, agent_size/2,(int) y + agent_size/2);
}

void Agent::init_movement(int direction) {
    this->state = MOVING_BETWEEN;

    switch (direction) {
        case GLUT_KEY_UP:
            this->grid_y--;
            break;
        case GLUT_KEY_DOWN:
            this->grid_y++;
            break;
        case GLUT_KEY_LEFT:
            this->grid_x--;
            break;
        case GLUT_KEY_RIGHT:
            this->grid_x++;
            break;
    }

    int destination_x = this->grid_x*this->sq_size + this->dist;
    int destination_y = this->grid_y*this->sq_size + this->dist;

    int dx = destination_x - x;
    int dy = destination_y - y;

    vx = dx/this->speed;
    vy = dy/this->speed;

    time_remaining = this->speed;
}

void Agent::integrate(long t) {
    if (this->state == MOVING_BETWEEN){
        if(t < time_remaining) {
            this->x = x + vx*t;
            this->y = y + vy*t;
            time_remaining -= t;
        } else if(t >= time_remaining) {
            this->x = x + vx*time_remaining;
            this->y = y + vy*time_remaining;

            if (this->next_move_valid(this->key_flag)){
                this->direction = this->key_flag;
                this->init_movement(this->key_flag);
            }else if(this->next_move_valid(this->direction)){
                this->init_movement(this->direction);
            }else{
                this->state = STILL;
                this->direction = -1;
                this->key_flag = -1;
            }
        }
    }
}

void Agent::treat_input(int key_flag){
    if (this->state == STILL){
        if (this->next_move_valid(key_flag)){
            this->direction = key_flag;
            this->key_flag = key_flag;
            this->init_movement(key_flag);
        }
    } else if(this->state == MOVING_BETWEEN) {
        this->key_flag = key_flag;
    }
}

bool Agent::next_move_valid(int key){
    switch (key) {
        case GLUT_KEY_UP:
            if (map.mesh[this->grid_y-1][this->grid_x] == CELL_VISITED){
                this->is_out = true;
                return true;
            } else if (!this->is_out && map.mesh[this->grid_y-1][this->grid_x] == BASE_CELL){
                return true;
            }
            break;
        case GLUT_KEY_DOWN:
            if (map.mesh[this->grid_y+1][this->grid_x] == CELL_VISITED){
                this->is_out = true;
                return true;
            } else if (!this->is_out && map.mesh[this->grid_y+1][this->grid_x] == BASE_CELL){
                return true;
            }
            break;
        case GLUT_KEY_LEFT:
            if (map.mesh[this->grid_y][this->grid_x-1] == CELL_VISITED){
                this->is_out = true;
                return true;
            } else if (!this->is_out && map.mesh[this->grid_y][this->grid_x-1] == BASE_CELL){
                return true;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (map.mesh[this->grid_y][this->grid_x+1] == CELL_VISITED){
                this->is_out = true;
                return true;
            } else if (!this->is_out && map.mesh[this->grid_y][this->grid_x+1] == BASE_CELL){
                return true;
            }
            break;
    }
    return false;
}

void Ghost::generate_new_movement(long t){
    if (t + this->timer_elapsed > this->random_timer){
        this->timer_elapsed = 0;
        int new_direction = this->get_random_direction();
        this->treat_input(new_direction);
    }else{
        this->timer_elapsed += t;
    }
}

int Ghost::get_random_direction(){
    int directions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    return directions[rand() % 4];
}

bool Ghost::is_not_turn(int direction){
    switch (direction) {
        case GLUT_KEY_UP:
            return this->direction != GLUT_KEY_DOWN;
        case GLUT_KEY_DOWN:
            return this->direction != GLUT_KEY_UP;
        case GLUT_KEY_LEFT:
            return this->direction != GLUT_KEY_RIGHT;
        case GLUT_KEY_RIGHT:
            return this->direction != GLUT_KEY_LEFT;
    }
    return false;
}

void Ghost::integrate_timer(long t){
    switch (this->behave_state) {
        case HOUSE:
            if (this->timer + t > exit_timer){
                this->behave_state = CHASE;
                this->timer = 0;
            }
            break;
        case SCATTER:
            if (this->timer + t > scatter_timer){
                this->behave_state = CHASE;
                this->direction = inverse_direction(this->direction);
                this->timer = 0;
            }
            break;
        case CHASE:
            if (this->timer + t > chase_timer){
                this->behave_state = SCATTER;
                this->direction = inverse_direction(this->direction);
                int xs[] = {0,map.n_cols};
                int ys[] = {0,map.n_rows};
                this->corner_x = xs[rand() % 2];
                this->corner_y = ys[rand() % 2];
                this->timer = 0;
            }
            break;
    }
    this->timer += t;
}

void Ghost::initialize_autonomous(int i){
    this->color = RED_CRAYOLA;
    this->is_out = false;
    this->behave_state = HOUSE;
    this->is_autonomous = true;
    this->timer = 0;
    this->exit_timer = 10000 * i;
    this->chase_timer = 10000;
    this->scatter_timer = 8000;
}

int inverse_direction(int direction){
    switch (direction) {
        case GLUT_KEY_UP:
            return GLUT_KEY_DOWN;
        case GLUT_KEY_DOWN:
            return GLUT_KEY_UP;
        case GLUT_KEY_LEFT:
            return GLUT_KEY_RIGHT;
        case GLUT_KEY_RIGHT:
            return GLUT_KEY_LEFT;
    }
    return GLUT_KEY_UP;
}
