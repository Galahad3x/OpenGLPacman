#ifndef MAP_H
#define MAP_H

#include<utility>
#include<list>
#include<vector>

using namespace std;

#define WALL_CELL 4
#define CELL_POSSIBLE_WALL 3
#define CELL_VISITED 1
#define BASE_CELL 2

// Map class definition (like interface)
class Map {
    public:
    int n_rows, n_cols;
    int **mesh;
    Map(int n_rows, int n_cols); // Constructor definition
    Map();
    void generate(int n_rows, int n_cols);
    void print_map();
    void draw(int sq_size);

    // Returns a random start position
    pair<int, int> start_position();
    pair<int, int> base_start_position();
    pair<int, int> get_exit_base_position();

    private:
        int x_limit, y_limit;
        pair<int, int> base_exit;
        void generate_mesh();
        void dfs_generator(int x_start, int y_start);
        pair<int, int> random_moviment(pair<int, int> current_position);
        void init_map();
        pair<int, int> insert_base();
        bool is_valid(pair<int, int> position);
        bool is_valid_to_jump(pair<int, int> position);
        vector<pair<int,int> > get_valids_neigbours(pair<int, int> position);
        vector<pair<int,int> > get_positions_to_jump(pair<int, int> position);
    

};
#endif
