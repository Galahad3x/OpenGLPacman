#include"map.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stack>
#include<vector>
#include<list>
#include<utility>
#include<iostream>
#include <algorithm>
#include"graphic.h"

using namespace std;


/*
***************************************************************
                    AUXILIAR FUNCTIONS
***************************************************************
*/

void apply_moviment(pair<int, int> src_position, pair<int, int> dst_position, int **mesh) {
    int x_src = src_position.first;
    int y_src = src_position.second;
    int x_dst = dst_position.first;
    int y_dst = dst_position.second;

    // caculate the middle wall position
    int x_wall = (x_src + x_dst) / 2;
    int y_wall = (y_src + y_dst) / 2;

    mesh[y_wall][x_wall] = CELL_VISITED;
}


void duplicate(int width, int height, int **mesh) {
    int x_end = width / 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < x_end; j++) {
            mesh[i][width - 1 - j] = mesh[i][j];
        }
    }
}

void remove_neighbour(vector<pair<int ,int> > &from,  pair<int ,int> neighbour) {
    vector<pair<int, int> >::iterator it = find(from.begin(), from.end(), neighbour);
    from.erase(it);
}

/*
***************************************************************
Implementation of the constructor and methods of the Map class
***************************************************************
*/
Map::Map(int n_rows, int n_cols) {
   generate(n_rows, n_cols);
}

Map::Map(){}

void Map::generate(int n_rows, int n_cols){
    this->n_rows = n_rows;
    this->n_cols = n_cols;

    this->x_limit = n_cols / 2 + 1; //(update limit when generates simetric map)
    this->y_limit = n_rows - 1; //(update limit when generates simetric map)

    generate_mesh();
}

void Map::print_map() {
    for (int i = 0; i < this->n_rows; i++) {
        for (int j = 0; j < this->n_cols; j++) {
            if (this->mesh[i][j] == CELL_VISITED)
                printf("-");
            else
                printf("0");
        }
        printf("\n");
    }
}

void Map::draw(int sq_size) {
    set_3f_color(COOL_BLUE);
    // Print corridor colors
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            if(this->mesh[i][j] == CELL_VISITED || this->mesh[i][j] == BASE_CELL){
                set_3f_color(GREEN_SHEEN);
                draw_square(j*sq_size, i*sq_size, sq_size);
            }else{
                set_3f_color(BLUE_CRAYOLA);
                draw_prism(j*sq_size, 0, i*sq_size, sq_size, sq_size-11, sq_size);
            }
        }
  	}
}

pair<int, int> Map::start_position(){
    int start_x = -1;
    int start_y = -1;
    while (true) {
        start_x = rand() % n_rows;
        start_y = rand() % n_cols;
        if (this->mesh[start_x][start_y] == CELL_VISITED){
            return make_pair(start_y, start_x);
        }
    }
}


pair<int, int> Map::base_start_position(){
    int start_x = -1;
    int start_y = -1;
    while (true) {
        start_x = rand() % n_rows;
        start_y = rand() % n_cols;
        if (this->mesh[start_x][start_y] == BASE_CELL){
            return make_pair(start_y, start_x);
        }
    }
}



void Map::init_map() {
    this->mesh = new int*[n_rows]; // similar a malloc but more easy(c++)
    for (int i=0; i < n_rows; i++) {
        this->mesh[i] = new int[n_cols];
        memset(mesh[i], 0, n_cols * sizeof(int));
    }
}

pair<int, int> Map::insert_base() {
    int base_width = 8;
    int base_height = 6;
    int x_mid = n_cols /2;
    int y_mid = n_rows /2;

    // calculate base limits
    int x_start = x_mid - (base_width) / 2 - 1;
    int y_start = y_mid - (base_height) / 2;
    int y_end = y_mid + base_height / 2;

    for (int i = y_start; i <= y_end; i++) {
        // put a wall in the first position to build a wall column base
        mesh[i][x_start] = WALL_CELL;
        // fill the base positions
        int fill_value = (i == y_start || i == y_end) ? WALL_CELL : BASE_CELL;
        for(int j = x_start + 1; j < x_mid; j++) {
            mesh[i][j] = fill_value;
        }
        mesh[i][x_mid] = fill_value;
    }
    // mark the base exit position as visited
    mesh[y_start][x_mid] = CELL_VISITED;
    return make_pair(x_mid, y_start);
}

void Map::generate_mesh() {
    init_map();

    pair<int, int> start_position = insert_base();
    dfs_generator(start_position.first, start_position.second-1);
    duplicate(n_cols, n_rows, mesh);
}

void Map::dfs_generator(int x_start, int y_start) {
    stack< pair<int,int> > stack;

    // Set start position
    pair<int, int> start_position = make_pair(x_start, y_start);
    mesh[start_position.second][start_position.first] = CELL_VISITED;
    stack.push(start_position);

    // apply dfs while the stack is no empty
    pair<int, int> current_position;
    pair<int, int> last_position;
    while (!stack.empty()) {
        current_position = stack.top();

        int x = current_position.first;
        int y = current_position.second;
        if (mesh[y][x] == CELL_POSSIBLE_WALL) {
            // find the valids position to jump
            vector<pair<int, int> > positions_to_jump = get_positions_to_jump(current_position);
            // remove the previus position of the list of valid position to jump
            remove_neighbour(positions_to_jump, last_position);
            // check if has valid positions to jump
            if(positions_to_jump.size()!=0) {
                pair<int, int> next_position = positions_to_jump[rand() % positions_to_jump.size()]; // select a random neighbour
                apply_moviment(current_position, next_position, mesh);
                mesh[current_position.second][current_position.first] = CELL_VISITED;
            } else {
                // backtraking if is imposible continue for this position
                mesh[current_position.second][current_position.first] = 0;
                stack.pop();
            }
        } else {
                pair<int, int> next_position = random_moviment(current_position);
                last_position = current_position;
                //check if have next position
                if(next_position.first==-1)
                    stack.pop();
                 else
                    stack.push(next_position);
        }
    }
    mesh[start_position.second + 1][start_position.first] = BASE_CELL;
    this->base_exit = make_pair(start_position.first, start_position.second - 1);
}


pair<int, int> Map::random_moviment(pair<int, int> current_position) {
    vector<pair<int, int> > neighbours = get_valids_neigbours(current_position);
    if(!neighbours.empty()) {
        pair<int, int> next_position = neighbours[rand() % neighbours.size()]; // select a random neighbour
        apply_moviment(current_position, next_position, mesh);
        vector<pair<int, int> > next_neighbours = get_valids_neigbours(next_position);
        mesh[next_position.second][next_position.first] = CELL_VISITED;

        // check if the next position have valid neighbours
        if (next_neighbours.size() == 0)  {
            // mark as possible wall to process the problem in the next iteration
            mesh[next_position.second][next_position.first] = CELL_POSSIBLE_WALL;
        }
        return next_position;
    }

    // return invalid position the current position don't have a valid neighbour
    return make_pair(-1,-1);
}

bool Map::is_valid(pair<int, int> position) {
    int x = position.first;
    int y = position.second;

    // check cords of the position
    if (x <= 0 || x >= x_limit)
        return false;

    if (y <= 0 || y >= y_limit)
        return false;

    return mesh[y][x] != CELL_VISITED && mesh[y][x] != WALL_CELL && mesh[y][x] != BASE_CELL;
}

vector<pair<int,int> > Map::get_valids_neigbours(pair<int, int> position) {
    int x = position.first;
    int y = position.second;

    pair<int, int> right = make_pair(x - 2, y);
    pair<int, int> left = make_pair(x + 2, y);
    pair<int, int> top = make_pair(x, y - 2);
    pair<int, int> botton = make_pair(x, y + 2);

    // get valids neigbours
    vector< pair<int,int> > valids_neigbours;
    if (is_valid(right))
        valids_neigbours.push_back(right);

    if (is_valid(left))
        valids_neigbours.push_back(left);


    if (is_valid(top))
        valids_neigbours.push_back(top);


    if (is_valid(botton))
        valids_neigbours.push_back(botton);

    return valids_neigbours;
}


bool Map::is_valid_to_jump(pair<int, int> current_position) {
    int x = current_position.first;
    int y = current_position.second;

    // check cords of the position
    if (x <= 0 || x > x_limit)
        return false;

    if (y <= 0 || y > y_limit)
        return false;

    return mesh[y][x] != WALL_CELL && mesh[y][x] != BASE_CELL;

}

vector<pair<int, int> > Map::get_positions_to_jump(pair<int, int> current_position) {
    int x = current_position.first;
    int y = current_position.second;

    // generate all neighbours
    pair<int, int> right = make_pair(x - 2, y);
    pair<int, int> left = make_pair(x + 2, y);
    pair<int, int> top = make_pair(x, y - 2);
    pair<int, int> botton = make_pair(x, y + 2);

    // get valids neighbours
    vector< pair<int,int> > valids_neigbours;
    if (is_valid_to_jump(right))
        valids_neigbours.push_back(right);

    if (is_valid_to_jump(left))
        valids_neigbours.push_back(left);


    if (is_valid_to_jump(top))
        valids_neigbours.push_back(top);


    if (is_valid_to_jump(botton))
        valids_neigbours.push_back(botton);

    return valids_neigbours;
}

pair<int, int> Map::get_exit_base_position() {
    return this->base_exit;
}
