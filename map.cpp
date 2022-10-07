#include"map.h"

#include<stdlib.h>
#include<stdio.h>


#ifndef MAP // This is to solve problems in the compilation time
#define MAP
/*
Implementation of the constructor and methods of the Map class
*/

// Map:: to indicate that you goint to implement the x map function or constructor
Map::Map(int n_rows, int n_cols) {
    // @anotation: this i like java but in this case it is a pointer for this reason to access to its fields its using -> operator
    this->n_rows = n_rows;
    this->n_cols = n_rows;
    this->mesh = new int*[n_rows]; // similar a malloc but more easy(c++)
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