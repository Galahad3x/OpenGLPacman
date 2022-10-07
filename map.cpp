#include"map.h"

#include<stdlib.h>
#include<stdio.h>

#ifndef MAP
#define MAP

Map::Map(int n_rows, int n_cols) {
    this->n_rows = n_rows;
    this->n_cols = n_rows;
    this->mesh = new int*[n_rows];
    for (int i=0; i < n_rows; i++)
        this->mesh[i] = new int[n_cols];
}

void Map::print_map() {
    for (int i = 0; i < this->n_rows; i++) {
        for (int j = 0; j < this->n_cols; j++) {
            if (this->mesh[i][j] >= 0) 
                printf("0");
            else 
                printf("-");
        }
        printf("\n");
    }
}

#endif