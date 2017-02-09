#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input
{
private:
    bool* keys;
    bool* prevKeys;

    bool mouseKeys[3];
    bool prevMouseKeys[3];

    int numKeys;
    int mouseX;
    int mouseY;
public:
    static const int MOUSE_LEFT = 1;
    static const int MOUSE_MIDDLE = 2;
    static const int MOUSE_RIGHT = 3;

    void init();
    void kill();

    void update();

    bool keyDown(int key);
    bool keyHit(int key);
    bool keyUp(int key);

    bool mouseDown(int key);
    bool mouseHit(int key);
    bool mouseUp(int key);

    int getMouseX();
    int getMouseY();

    void hideCursor(bool hide = true);
};

#endif
