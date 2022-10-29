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
        int x, y;
        int vx, vy;

        // Ms to wait before moving 1 pixel in the direction;
        int vut_x, vut_y;
        int dx, dy;

        long time_remaining;

        int state;

        int speed;

        // Constructor
        Agent();
        Agent(int sq_size,int agent_size);
        Agent(int sq_size,int agent_size,int grid_x,int grid_y);

        void initialize(int sq_size,int agent_size);
        void initialize(int sq_size,int agent_size,int grid_x,int grid_y);

        // Movement functions
        void set_position(int x, int y);
        void init_movement();
        void integrate(long t);
        void draw();
};
