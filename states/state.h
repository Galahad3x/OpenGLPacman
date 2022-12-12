#define MENUSTATE 0
#define GAMESTATE 1

#ifndef STATE_H
#define STATE_H

class State {
    public:
        void enter();
        void exitState();

        void displayFunc();
        void specialFunc(int key, int x, int y);
        void keyboardFunc(unsigned char key, int x, int y);
        void idleFunc();
};

#endif