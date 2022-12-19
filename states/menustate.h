#include "state.h"

class MenuState : public State {
    public:
        static void enter();
        static void exitState();

        static void displayFunc();
        static void specialFunc(int key, int x, int y);
        static void keyboardFunc(unsigned char key, int x, int y);
        static void idleFunc();

        MenuState();
};

class MenuToGameState : public State {
    public:
        static void enter();
        static void exitState();

        static void displayFunc();
        static void specialFunc(int key, int x, int y);
        static void keyboardFunc(unsigned char key, int x, int y);
        static void idleFunc();

        MenuToGameState();
};