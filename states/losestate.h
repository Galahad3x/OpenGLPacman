#include "state.h"

class LoseState : public State
{
public:
    static void enter();
    static void exitState();

    static void displayFunc();
    static void specialFunc(int key, int x, int y);
    static void keyboardFunc(unsigned char key, int x, int y);
    static void idleFunc();

    LoseState();
};

class GameToLoseState : public State
{
public:
    static void enter();
    static void exitState();

    static void displayFunc();
    static void specialFunc(int key, int x, int y);
    static void keyboardFunc(unsigned char key, int x, int y);
    static void idleFunc();

    GameToLoseState();
};