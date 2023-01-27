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
pair<int, int> getCoordsByAgentIndex(GameAgentState state_a, int agentIndex);

int MinimaxAgent::getBestAction()
{
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    float best_score = -99999999.0;
    int best_action = 0;
    int depth = 4;

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
        // TODO agafar les coordenades be
        int foodx = (int)food->x + food->size / 2;
        int foody = (int)food->y + food->size / 2;
        pair<int, int> fCoords = make_pair(foodx / sq_size, foody / sq_size);
        current.foods_positions.push_back(fCoords);
    }

    for (int i = 0; i < 4; i++)
    {
        ("Trying action in main\n");
        if (!is_action_valid(make_pair(pacman.grid_x, pacman.grid_y), actions[i]))
        {
            ("Not valid\n");
            continue;
        }
        GameAgentState succ = getNextGameState(current, 0, actions[i]);
        float score = minimumScore(succ, 1, depth);
        if (score > best_score)
        {
            ("best_score %.5f\n", score);
            best_score = score;
            best_action = actions[i];
        }
    }
    return best_action;
}

GameAgentState getNextGameState(GameAgentState current, int agentIndex, int action)
{
    GameAgentState next;
    if (agentIndex != 0)
    {
        next.pacman_gridx = current.pacman_gridx;
        next.pacman_gridy = current.pacman_gridy;

        int ghostCount = 0;
        std::list<pair<int, int>>::iterator ghostIter;
        for (ghostIter = current.ghosts_positions.begin(); ghostIter != current.ghosts_positions.end(); ++ghostIter)
        {
            pair<int, int> gCoords;
            if (ghostCount == agentIndex - 1)
            {
                switch (action)
                {
                case GLUT_KEY_UP:
                    gCoords = make_pair(ghostIter->first, ghostIter->second - 1);
                    break;
                case GLUT_KEY_DOWN:
                    gCoords = make_pair(ghostIter->first, ghostIter->second + 1);
                    break;
                case GLUT_KEY_LEFT:
                    gCoords = make_pair(ghostIter->first - 1, ghostIter->second);
                    break;
                case GLUT_KEY_RIGHT:
                    gCoords = make_pair(ghostIter->first + 1, ghostIter->second);
                    break;
                }
            }
            else
            {
                gCoords = make_pair(ghostIter->first, ghostIter->second);
            }
            next.ghosts_positions.push_back(gCoords);
        }

        std::list<pair<int, int>>::iterator foodIter;
        for (foodIter = current.foods_positions.begin(); foodIter != current.foods_positions.end(); ++foodIter)
        {
            pair<int, int> fCoords;
            fCoords = make_pair(foodIter->first, foodIter->second);
            next.foods_positions.push_back(fCoords);
        }
    }
    else
    {
        std::list<pair<int, int>>::iterator ghostIter;
        for (ghostIter = current.ghosts_positions.begin(); ghostIter != current.ghosts_positions.end(); ++ghostIter)
        {
            pair<int, int> gCoords;
            gCoords = make_pair(ghostIter->first, ghostIter->second);
            next.ghosts_positions.push_back(gCoords);
        }

        switch (action)
        {
        case GLUT_KEY_UP:
            next.pacman_gridy = current.pacman_gridy - 1;
            next.pacman_gridx = current.pacman_gridx;
            break;
        case GLUT_KEY_DOWN:
            next.pacman_gridy = current.pacman_gridy + 1;
            next.pacman_gridx = current.pacman_gridx;
            break;
        case GLUT_KEY_LEFT:
            next.pacman_gridy = current.pacman_gridy;
            next.pacman_gridx = current.pacman_gridx - 1;
            break;
        case GLUT_KEY_RIGHT:
            next.pacman_gridy = current.pacman_gridy;
            next.pacman_gridx = current.pacman_gridx + 1;
            break;
        }

        std::list<pair<int, int>>::iterator foodIter;
        for (foodIter = current.foods_positions.begin(); foodIter != current.foods_positions.end(); ++foodIter)
        {
            pair<int, int> fCoords;
            fCoords = make_pair(foodIter->first, foodIter->second);
            if (fCoords.first == next.pacman_gridx && fCoords.second == next.pacman_gridy)
            {
                continue;
            }
            next.foods_positions.push_back(fCoords);
        }
    }
    // Ha de moure els agents i en el cas del pacman menjar el food també
    return next;
}

float minimumScore(GameAgentState state_a, int agentIndex, int depth)
{
    ("Entering minimumScore\n");
    if (terminal_test(state_a, depth))
    {
        return evaluationFunction(state_a);
    }

    float sc = 999999999.0;
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};

    pair<int, int> current_coords = getCoordsByAgentIndex(state_a, agentIndex);
    for (int i = 0; i < 4; i++)
    {
        ("Trying action in minimum state \t%i\n", agentIndex);
        if (!is_action_valid(make_pair(current_coords.first, current_coords.second), actions[i]))
        {
            ("Not valid\n");
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
        ("Score %.5f", score);
        if (score < sc)
        {
            ("\tIs best");
            sc = score;
        }
        ("\n");
    }
    return sc;
}

float maximumScore(GameAgentState state_a, int agentIndex, int depth)
{
    ("Entering maximum score\n");
    if (terminal_test(state_a, depth))
    {
        return evaluationFunction(state_a);
    }

    float sc = -999999999.0;
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};

    pair<int, int> current_coords = getCoordsByAgentIndex(state_a, agentIndex);

    for (int i = 0; i < 4; i++)
    {
        ("Testing action in maximum\n");
        if (!is_action_valid(make_pair(current_coords.first, current_coords.second), actions[i]))
        {
            continue;
        }
        GameAgentState succ = getNextGameState(state_a, agentIndex, actions[i]);
        float score = minimumScore(succ, 1, depth);
        ("skore %.5f", score);
        if (score > sc)
        {
            ("\tIs best");
            sc = score;
        }
        ("\n");
    }
    return sc;
}

bool terminal_test(GameAgentState state_a, int depth)
{
    if (depth == 0)
    {
        return true;
    }
    if (state_a.foods_positions.size() == 0)
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
    int food_amount = 0;
    float min_food_distance = 999999999.0;
    float food_distance = 0;
    float food_closeness = 0.0;
    std::list<pair<int, int>>::iterator food;

    if (state_a.foods_positions.size() == 0)
    {
        return 1000000000;
    }

    for (food = state_a.foods_positions.begin(); food != state_a.foods_positions.end(); ++food)
    {
        food_amount += 1;
        // pair<int, int> food_coords = make_pair(food->first, food->second);
        float food_d = manhattanDistance(make_pair(state_a.pacman_gridx, state_a.pacman_gridy), *food);
        if (food_d < min_food_distance)
        {
            min_food_distance = food_d;
        }
        food_distance += food_d;
        /*
        std::list<pair<int, int>>::iterator food2;
        food2 = state_a.foods_positions.begin();
        for (int j = 0; j < food_amount; j++)
        {
            ++food2;
        }
        for (food2 = food2; food2 != state_a.foods_positions.end(); ++food2)
        {
            food_closeness += pow(manhattanDistance(*food, *food2), 2);
        }
        */
    }

    int distance_ghosts = 0;
    std::list<pair<int, int>>::iterator ghost;
    for (ghost = state_a.ghosts_positions.begin(); ghost != state_a.ghosts_positions.end(); ++ghost)
    {
        // pair<int, int> obj2 = make_pair(ghost->first, ghost->second);
        if (manhattanDistance(make_pair(state_a.pacman_gridx, state_a.pacman_gridy), *ghost) < 2)
        {
            distance_ghosts = 1;
        }
    }
    float score = 0.0;
    score += food_amount * -200;
    // score += food_distance * -0.8;
    score += food_closeness * 0.03;
    score += (1.0 / min_food_distance) * 250;
    score += distance_ghosts * -1000;
    return score;
    // return 0.0;
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
        if (map.mesh[gridy - 1][gridx] == CELL_VISITED || map.mesh[gridy - 1][gridx] == BASE_CELL)
        {
            return true;
        }
        break;
    case GLUT_KEY_DOWN:
        if (map.mesh[gridy + 1][gridx] == CELL_VISITED || map.mesh[gridy + 1][gridx] == BASE_CELL)
        {
            return true;
        }
        break;
    case GLUT_KEY_LEFT:
        if (map.mesh[gridy][gridx - 1] == CELL_VISITED || map.mesh[gridy][gridx - 1] == BASE_CELL)
        {
            return true;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (map.mesh[gridy][gridx + 1] == CELL_VISITED || map.mesh[gridy][gridx + 1] == BASE_CELL)
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

pair<int, int> getCoordsByAgentIndex(GameAgentState state_a, int agentIndex)
{
    if (agentIndex == 0)
    {
        return make_pair(state_a.pacman_gridx, state_a.pacman_gridy);
    }
    else
    {
        int ghostCount = 0;
        std::list<pair<int, int>>::iterator ghostIter;
        for (ghostIter = state_a.ghosts_positions.begin(); ghostIter != state_a.ghosts_positions.end(); ++ghostIter)
        {
            if (ghostCount == agentIndex - 1)
            {
                return make_pair(ghostIter->first, ghostIter->second);
            }
            ghostCount++;
        }
    }
    return make_pair(0, 0);
}
