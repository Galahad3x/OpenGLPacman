#include "agent.h"
#include "graphic.h"
#include <stdio.h>

using namespace std;

void Agent::initialize(int sq_size, int agent_size){
    this->state = STILL;
    this->speed = 1000.0;

    this->grid_x = 0;
    this->grid_y = 0;

    this->dist = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + this->dist;
    this->y = this->grid_y*sq_size + this->dist;
    this->sq_size = sq_size;
    this->agent_size = agent_size;
}

void Agent::initialize(int sq_size,int agent_size,int grid_x,int grid_y){
    this->state = STILL;
    this->speed = 1000.0;

    this->grid_x = grid_x;
    this->grid_y = grid_y;

    this->dist = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + this->dist;
    this->y = this->grid_y*sq_size + this->dist;
    this->sq_size = sq_size;
    this->agent_size = agent_size;
}

Agent::Agent(){}

Agent::Agent(int sq_size,int agent_size){
    this->initialize(sq_size, agent_size);
}

Agent::Agent(int sq_size,int agent_size,int grid_x,int grid_y){
    this->initialize(sq_size, agent_size, grid_x, grid_y);
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

            this->state = STILL;
        }
    }
}

void Agent::draw() {
    set_3f_color(DARK_GREEN);
    draw_square((int) x, (int) y, agent_size);
}
