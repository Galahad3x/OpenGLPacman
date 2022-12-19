#include"agent.h"
#include"map.h"

using namespace std;

int calculate_ghost_behaviour(Ghost ghost, Agent agent, Map map);
int calculate_next_ghost_move(Ghost ghost, int x, int y);
