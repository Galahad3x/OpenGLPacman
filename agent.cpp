#include "agent.h"
#include "graphic.h"

using namespace std;

Agent::Agent(int sq_size,int agent_size){
    this->grid_x = 0;
    this->grid_y = 0;

    int distance = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + distance;
    this->y = this->grid_y*sq_size + distance;
    this->sq_size = sq_size;
    this->agent_size = agent_size;
}

Agent::Agent(int sq_size,int agent_size,int grid_x,int grid_y){
    this->grid_x = grid_x;
    this->grid_y = grid_y;

    int distance = sq_size / 2 - agent_size / 2;

    this->x = this->grid_x*sq_size + distance;
    this->y = this->grid_y*sq_size + distance;
    this->sq_size = sq_size;
    this->agent_size = agent_size;
}

void Agent::set_position(int x,int y) {
    this->x = x;
    this->y = y;
}

void Agent::init_movement(int destination_x,int destination_y,int duration) {
    vx = (destination_x - x)/duration;
    vy = (destination_y - y)/duration;

    time_remaining=duration;
}

void Agent::integrate(long t) {
    if(t<time_remaining) {
        x = x + vx*t;
        y = y + vy*t;
        time_remaining -= t;
    } else if(t>=time_remaining) {
        x = x + vx*time_remaining;
        y = y + vy*time_remaining;
    }
}

void Agent::draw() {
    set_3f_color(DARK_GREEN);
    draw_square(x, y, agent_size);
}
