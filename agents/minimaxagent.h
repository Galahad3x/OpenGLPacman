#include <list>

using namespace std;

#ifndef MINIMAX_H
#define MINIMAX_H

class MinimaxAgent
{
public:
    static int getBestAction();
    static float getScore(int action);
};

class GameAgentState
{
public:
    int pacman_gridx;
    int pacman_gridy;

    list<pair<int, int>> ghosts_positions;
    list<pair<int, int>> foods_positions;

    GameAgentState();
};

#endif
