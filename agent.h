#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "map.h"

#define STILL 0
#define MOVING_BETWEEN 1
#define REACHED 2

using namespace std;

class Agent {
    public:
        // Position inside the grid of cells
        int grid_x, grid_y;
        int sq_size;

        // Size of the agent
        int agent_size;
        int dist;

        // Position and velocity inside the screen
        float x, y;
        float vx, vy;

        long time_remaining;
        float speed;

        int state;

        int key_flag;
        int direction;

        Map map;

        int color;

        bool is_out = true;

        bool is_autonomous = false;

        // Constructor
        Agent();
        Agent(int sq_size,int agent_size,int grid_x,int grid_y, Map map);

        void initialize(int sq_size,int agent_size,int grid_x,int grid_y, Map map);

        // Movement functions
        void set_position(float x, float y);
        void init_movement(int direction);
        void integrate(long t);
        void draw();

        void treat_input(int key_flag);

        bool next_move_valid(int key);

        void before_next_move();
};

class Ghost : public Agent{
    public:
        int random_timer = 1000;
        int timer_elapsed = 0;

        void generate_new_movement(long t);
        int get_random_direction();
};
