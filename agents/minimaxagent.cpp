#include "minimaxagent.h"

#include <list>
#include <math.h>
#include <stdio.h>

#include "../globals.h"

int adapt_to_cam(int key);
float manhattanDistance(pair<float, float> obj1, pair<float, float> obj2);
bool is_action_valid(pair<int, int> current_grid, int action);
GameAgentState getNextGameState(GameAgentState current, int agentIndex, int action);
float minimumScore(GameAgentState state_a, int agentIndex, int depth);
float maximumScore(GameAgentState state_a, int agentIndex, int depth);
bool terminal_test(GameAgentState state_a, int depth);
float evaluationFunction(GameAgentState state_a);

int MinimaxAgent::getBestAction()
{
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    float best_score = -99999999.0;
    int best_action = 0;
    int depth = 3;

    GameAgentState current;

    current.pacman_gridx = pacman.grid_x;
    current.pacman_gridy = pacman.grid_y;

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        pair<int, int> gCoords = make_pair(ghost->grid_x, ghost->grid_y);
        current.ghosts_positions.push_back(gCoords);
    }

    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food)
    {
        pair<int, int> fCoords = make_pair(food->x, food->y);
        current.foods_positions.push_back(fCoords);
    }

    for (int i = 0; i < 4; i++)
    {
        if (!is_action_valid(make_pair(pacman.grid_x, pacman.grid_y), actions[i]))
        {
            continue;
        }
        GameAgentState succ = getNextGameState(current, 0, actions[i]);
        float score = minimumScore(succ, 1, depth);
        if (score > best_score)
        {
            best_score = score;
            best_action = actions[i];
        }
    }
    // printf("\n");
    return best_action;
}

GameAgentState getNextGameState(GameAgentState current, int agentIndex, int action)
{
    // TODO implementar canviar el state
    // Ha de moure els agents i en el cas del pacman menjar el food també
}

float minimumScore(GameAgentState state_a, int agentIndex, int depth)
{
    if (terminal_test(state_a, depth))
    {
        return evaluationFunction(state_a);
    }

    float sc = 999999999.0;
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};

    pair<int, int> current_coords = make_pair(0, 0);

    // TODO agafar les coords del agent que toqui

    for (int i = 0; i < 4; i++)
    {
        if (!is_action_valid(make_pair(current_coords.first, current_coords.second), actions[i]))
        {
            continue;
        }
        GameAgentState succ = getNextGameState(state_a, agentIndex, actions[i]);
        float score;
        if (agentIndex == state_a.ghosts_positions.size())
        {
            score = maximumScore(succ, 0, depth - 1);
        }
        else
        {
            score = minimumScore(succ, agentIndex + 1, depth);
        }
        if (score < sc)
        {
            sc = score;
        }
    }
    // printf("\n");
    return sc;
}

float maximumScore(GameAgentState state_a, int agentIndex, int depth)
{
    if (terminal_test(state_a, depth))
    {
        return evaluationFunction(state_a);
    }

    float sc = -999999999.0;
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};

    pair<int, int> current_coords = make_pair(0, 0);

    // TODO agafar les coords del agent que toqui

    for (int i = 0; i < 4; i++)
    {
        if (!is_action_valid(make_pair(current_coords.first, current_coords.second), actions[i]))
        {
            continue;
        }
        GameAgentState succ = getNextGameState(state_a, agentIndex, actions[i]);
        float score = minimumScore(succ, 1, depth);
        if (score < sc)
        {
            sc = score;
        }
    }
    return sc;
}

bool terminal_test(GameAgentState state_a, int depth)
{
    if (depth == 0)
    {
        return true;
    }
    std::list<pair<int, int>>::iterator ghost_iter;
    for (ghost_iter = state_a.ghosts_positions.begin(); ghost_iter != state_a.ghosts_positions.end(); ++ghost_iter)
    {
        pair<int, int> coords = make_pair(ghost_iter->first, ghost_iter->second);
        if (coords.first == state_a.pacman_gridx && coords.second == state_a.pacman_gridy)
        {
            return true;
        }
    }
    return false;
}

float evaluationFunction(GameAgentState state_a)
{
    // TODO implementar evaluation function
    return 0.0;
}

GameAgentState::GameAgentState()
{
}

bool is_action_valid(pair<int, int> current_grid, int action)
{
    int gridx = current_grid.first;
    int gridy = current_grid.second;
    switch (action)
    {
    case GLUT_KEY_UP:
        if (map.mesh[gridy - 1][gridx] == CELL_VISITED)
        {
            return true;
        }
        break;
    case GLUT_KEY_DOWN:
        if (map.mesh[gridy + 1][gridx] == CELL_VISITED)
        {
            return true;
        }
        break;
    case GLUT_KEY_LEFT:
        if (map.mesh[gridy][gridx - 1] == CELL_VISITED)
        {
            return true;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (map.mesh[gridy][gridx + 1] == CELL_VISITED)
        {
            return true;
        }
        break;
    }
    return false;
}

float MinimaxAgent::getScore(int action)
{
    if (!pacman.next_move_valid(action))
    {
        return -99999999.0;
    }

    float pacmanx = pacman.x + pacman.agent_size / 2;
    float pacmany = pacman.y + pacman.agent_size / 2;
    switch (action)
    {
    case GLUT_KEY_UP:
        pacmany -= sq_size;
        break;
    case GLUT_KEY_DOWN:
        pacmany += sq_size;
        break;
    case GLUT_KEY_LEFT:
        pacmanx -= sq_size;
        break;
    case GLUT_KEY_RIGHT:
        pacmanx += sq_size;
        break;
    }
    pair<float, float> pacman_coords = make_pair(pacmanx, pacmany);

    float score = 0.0;
    std::list<Food>::iterator food;
    for (food = foodList.begin(); food != foodList.end(); ++food)
    {
        score -= 1.0;
        pair<float, float> food_coords = make_pair(food->x, food->y);
        score += (1.0 / pow(manhattanDistance(pacman_coords, food_coords), 2));
    }

    std::list<Ghost>::iterator ghost;
    for (ghost = ghosts.begin(); ghost != ghosts.end(); ++ghost)
    {
        pair<float, float> obj2 = make_pair(ghost->x, ghost->y);
        score -= (1.0 / pow(manhattanDistance(pacman_coords, obj2) - sq_size, 2));
    }
    return score;
}
