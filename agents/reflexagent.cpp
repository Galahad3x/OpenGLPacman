#include "reflexagent.h"

#include <math.h>
#include <stdio.h>

#include "../globals.h"

int adapt_to_cam(int key);
float manhattanDistance(pair<float, float> obj1, pair<float, float> obj2);

int ReflexAgent::getBestAction()
{
    int actions[] = {GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT};
    float best_score = -99999999.0;
    int best_action = 0;
    for (int i = 0; i < 4; i++)
    {
        float score = ReflexAgent::getScore(actions[i]);
        printf("KEY: %i SCORE: %.5f\t", actions[i], score);
        if (score > best_score)
        {
            best_score = score;
            best_action = actions[i];
        }
    }
    printf("\n");
    return best_action;
}

float ReflexAgent::getScore(int action)
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
    return score;
}

float manhattanDistance(pair<float, float> obj1, pair<float, float> obj2)
{
    float dx = pow(obj1.first - obj2.first, 2);
    float dy = pow(obj1.second - obj2.second, 2);
    return sqrt(dx + dy);
}
