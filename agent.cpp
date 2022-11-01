#include "agent.h"
#include "graphic.h"
//#include "map.h"
#include <stdio.h>

using namespace std;

void Agent::initialize(int sq_size, int agent_size, Map map){
    this->state = STILL;
    this->speed = 400.0;

    this->grid_x = 0;
    this->grid_y = 0;

    this->dist = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + this->dist;
    this->y = this->grid_y*sq_size + this->dist;
    this->sq_size = sq_size;
    this->agent_size = agent_size;

    this->map = map;
}

void Agent::initialize(int sq_size,int agent_size,int grid_x,int grid_y, Map map){
    this->state = STILL;
    this->speed = 400.0;

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

Agent::Agent(int sq_size,int agent_size, Map map){
    this->initialize(sq_size, agent_size, map);
}

Agent::Agent(int sq_size,int agent_size,int grid_x,int grid_y, Map map){
    this->initialize(sq_size, agent_size, grid_x, grid_y, map);
}

void Agent::set_position(float x,float y) {
    this->x = x;
    this->y = y;
}

void Agent::init_movement() {
    this->state = MOVING_BETWEEN;

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

            // Init new movement if possible
            // TODO change true by valid comprovation
            if (this->next_move_valid()){
                this->state = STILL;
                this->treat_input(this->key_flag);
            }else{
                this->state = STILL;
                this->key_flag = -1;
            }
        }
    }
}

void Agent::treat_input(int key_flag){
    if (this->state == STILL){
        int original_flag = this->key_flag;
        this->key_flag = key_flag;
        if (this->next_move_valid()){
            switch (key_flag) {
                case GLUT_KEY_UP:
                    this->grid_y--;
                    this->init_movement();
                    break;
                case GLUT_KEY_DOWN:
                    this->grid_y++;
                    this->init_movement();
                    break;
                case GLUT_KEY_LEFT:
                    this->grid_x--;
                    this->init_movement();
                    break;
                case GLUT_KEY_RIGHT:
                    this->grid_x++;
                    this->init_movement();
                    break;
            }
        } else {
            this->key_flag = original_flag;
            switch (this->key_flag) {
                case GLUT_KEY_UP:
                    this->grid_y--;
                    this->init_movement();
                    break;
                case GLUT_KEY_DOWN:
                    this->grid_y++;
                    this->init_movement();
                    break;
                case GLUT_KEY_LEFT:
                    this->grid_x--;
                    this->init_movement();
                    break;
                case GLUT_KEY_RIGHT:
                    this->grid_x++;
                    this->init_movement();
                    break;
                default:
                    if (original_flag == this->key_flag){
                        this->key_flag = key_flag;
                    }
                    break;
            }
        }
    } else if(this->state == MOVING_BETWEEN) {
        this->key_flag = key_flag;
    }
}

void Agent::draw() {
    set_3f_color(ORANGE);
    draw_square((int) x, (int) y, agent_size);
}

bool Agent::next_move_valid(){
    switch (this->key_flag) {
        case GLUT_KEY_UP:
            if (map.mesh[this->grid_y-1][this->grid_x] == CELL_VISITED){
                return true;
            }
            break;
        case GLUT_KEY_DOWN:
            if (map.mesh[this->grid_y+1][this->grid_x] == CELL_VISITED){
                return true;
            }
            break;
        case GLUT_KEY_LEFT:
            if (map.mesh[this->grid_y][this->grid_x-1] == CELL_VISITED){
                return true;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (map.mesh[this->grid_y][this->grid_x+1] == CELL_VISITED){
                return true;
            }
            break;
    }
    return false;
}
