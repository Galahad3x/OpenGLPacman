#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include"ghost.h"

#include<stdio.h>
#include<tgmath.h>

float pythagoras(int x1, int y1, int x2, int y2){
    return sqrt(((abs(x1-x2)*abs(x1-x2))+(abs(y1-y2)*abs(y1-y2)))*1.0);
}

int calculate_ghost_behaviour(Ghost ghost, Agent agent, Map map){
    if (ghost.is_out == false && ghost.behave_state != HOUSE){
        pair<int, int> pos = map.get_exit_base_position();
        return calculate_next_ghost_move(ghost, pos.first, pos.second);
    }else if (ghost.behave_state == SCATTER){
        return calculate_next_ghost_move(ghost, ghost.corner_x, ghost.corner_y);
    }else if (ghost.behave_state == CHASE){
        return calculate_next_ghost_move(ghost, agent.grid_x, agent.grid_y);
    } else {
        return ghost.get_random_direction();
    }
    return GLUT_KEY_UP;
}

int calculate_next_ghost_move(Ghost ghost, int x, int y){
    int p = 4;
    int directions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    float scores[] = {0,0,0,0};
    for (int i = 0; i < p; i++){
        switch (directions[i]) {
            case GLUT_KEY_UP:
                scores[i] = pythagoras(ghost.grid_x, ghost.grid_y-1, x, y);
                break;
            case GLUT_KEY_DOWN:
                scores[i] = pythagoras(ghost.grid_x, ghost.grid_y+1, x, y);
                break;
            case GLUT_KEY_LEFT:
                scores[i] = pythagoras(ghost.grid_x-1, ghost.grid_y, x, y);
                break;
            case GLUT_KEY_RIGHT:
                scores[i] = pythagoras(ghost.grid_x+1, ghost.grid_y, x, y);
                break;
        }
    }
    float min_score = 999999.0;
    int best_move = -1;
    for (int i = 0; i < p; i++){
        // Canviar true per normes
        if (scores[i] < min_score && ghost.is_not_turn(directions[i]) && ghost.next_move_valid(directions[i])){
            best_move = directions[i];
            min_score = scores[i];
        }
    }
    return best_move;
}
