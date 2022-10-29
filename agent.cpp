#include "agent.h"
#include "graphic.h"
#include <stdio.h>

using namespace std;

void Agent::initialize(int sq_size, int agent_size){
    this->state = STILL;
    this->speed = 1000;

    this->grid_x = 0;
    this->grid_y = 0;

    int distance = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + distance;
    this->y = this->grid_y*sq_size + distance;
    this->sq_size = sq_size;
    this->agent_size = agent_size;
}

void Agent::initialize(int sq_size,int agent_size,int grid_x,int grid_y){
    this->state = STILL;
    this->speed = 1000;

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

void Agent::set_position(int x,int y) {
    this->x = x;
    this->y = y;
}

void Agent::init_movement() {
    this->state = MOVING_BETWEEN;

    int destination_x = this->grid_x*this->sq_size + this->dist;
    int destination_y = this->grid_y*this->sq_size + this->dist;

    dx = destination_x - x;
    dy = destination_y - y;

    if (this->speed < dx || this->speed < dy){
        printf("VELOC_METHOD\n");
        vx = dx/this->speed;
        vy = dy/this->speed;
        vut_x = -1;
        vut_y = -1;
    }else{
        vut_x = this->speed/dx;
        vut_y = this->speed/dy;
        printf("VUTX %i VUTY %i\n", vut_x, vut_y);
        vx = 0;
        vy = 0;
    }

    time_remaining = this->speed;
}

void Agent::integrate(long t) {
    if (vut_x >= 0 || vut_y >= 0){
        if (t >= vut_x){
            if (dx > 0){
                x++;
                dx--;
            }else if (dx < 0){
                x--;
                dx++;
            }else{
                vut_x = -1;
            }
        }
        if (t >= vut_y){
            if (dy > 0){
                y++;
                dy--;
            }else if (dy < 0){
                y--;
                dy++;
            }else{
                vut_y = -1;
            }
        }
    } else if (vx == 0 && vy == 0){
        this->state = STILL;
    } else {
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
    printf("AGENT GRIDX %i GRIDY %i X %i Y %i\n", grid_x, grid_y, x, y);
    set_3f_color(DARK_GREEN);
    draw_square(x, y, agent_size);
}
