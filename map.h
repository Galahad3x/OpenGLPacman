#include <utility>
#include<list>

using namespace std;

// Map class definition (like interface)
class Map {
    public:
    int n_rows, n_cols;
    int **mesh;
    Map(int n_rows, int n_cols); // Constructor definition
    void print_map();

    private:
        int x_limit, y_limit;
        void generate_mesh();
        void init_map();
        bool is_valid(pair<int, int> position);
        vector<pair<int,int> > get_valids_neigbours(pair<int, int> position);
};




