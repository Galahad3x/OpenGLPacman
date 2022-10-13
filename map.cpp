#include"map.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stack>
#include<vector>
#include<list>
#include <utility>
#include<iostream>

using namespace std;

#define WALL_CELL 3
#define CELL_POSIBLE_BLOCK 2
#define CELL_VISITED 1



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


/*
Implementation of the constructor and methods of the Map class
*/
// Map:: to indicate that you goint to implement the x map function or constructor
Map::Map(int n_rows, int n_cols) {
    // @anotation: this i like java but in this case it is a pointer for this reason to access to its fields its using -> operator
    this->n_rows = n_rows;
    this->n_cols = n_cols;

    this->x_limit = n_cols;  //(update limit when generates simetric map)
    this->y_limit = n_rows - 1;  //(update limit when generates simetric map)

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

void Map::init_map() {
    this->mesh = new int*[n_rows]; // similar a malloc but more easy(c++)
    for (int i=0; i < n_rows; i++) {
        this->mesh[i] = new int[n_cols];
        memset(mesh[i], 0, n_cols * sizeof(int));
    }
}

void Map::generate_mesh() {
    init_map();
    srand(clock());
    stack< pair<int,int> > stack;
    
    // Set start position
    pair<int, int> start_position = make_pair(x_limit-1, y_limit-1);
    mesh[start_position.second][start_position.first] = CELL_VISITED;
    stack.push(start_position);
    
    // apply dfs while the stack is no empty
    pair<int, int> current_position;
    while (!stack.empty()) {
        current_position = stack.top();
        vector<pair<int, int> > neighbours = get_valids_neigbours(current_position);
        if(!neighbours.empty()) {
            pair<int, int> next_position = neighbours[rand() % neighbours.size()]; // select a random neighbour
            apply_moviment(current_position, next_position, mesh);
            mesh[next_position.second][next_position.first] = CELL_VISITED;
            stack.push(next_position);
        } else {
            stack.pop();
        }
    }
}

bool Map::is_valid(pair<int, int> position) {
    int x = position.first;
    int y = position.second;
    
    // check cords of the position
    if (x <= 0 || x >= x_limit) 
        return false;

    if (y <= 0 || y >= y_limit) 
        return false;

    return mesh[y][x] != CELL_VISITED && mesh[y][x] != WALL_CELL;
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