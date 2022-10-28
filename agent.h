using namespace std;

class Agent {
    public:
        // Position inside the grid of cells
        int grid_x, grid_y;
        int sq_size;

        // Size of the agent
        int agent_size;

        // Position and velocity inside the screen
        int x, y;
        int vx, vy;

        long time_remaining;

        // Constructor
        Agent(int sq_size,int agent_size);
        Agent(int sq_size,int agent_size,int grid_x,int grid_y);

        // Movement functions
        void set_position(int x, int y);
        void init_movement(int destination_x, int destination_y, int duration);
        void integrate(long t);
        void draw();
};
