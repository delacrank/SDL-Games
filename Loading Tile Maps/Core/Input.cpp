#include "Input.h"

void Input::init()
{
    //SDL_GetKeyState is gone, replace with SDL_GetKeyboardState
    Uint8* keyboard = (Uint8*)SDL_GetKeyboardState(&numKeys);

    keys = new bool[numKeys];
    prevKeys = new bool[numKeys];

    for(int i = 0; i < numKeys; i++)
    {
        keys[i] = keyboard[i];
        prevKeys[i] = false;
    }

    SDL_GetMouseState(&mouseX, &mouseY);

    for(int i = 1; i <= 3; i++)
    {
        mouseKeys[i] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(i);
        prevMouseKeys[i] = false;
    }
}

void Input::kill()
{
    delete[] keys;
    delete[] prevKeys;
}

void Input::update()
{
    //SDL_GetKeyState is gone in SDL 2 in SDL 2, replace with SDL_GetKeyboardState
    Uint8* keyboard = (Uint8*)SDL_GetKeyboardState(&numKeys);

    for(int i = 0; i < numKeys; i++)
    {

        prevKeys[i] = keys[i];
        keys[i] = keyboard[i];
    }

    SDL_GetMouseState(&mouseX, &mouseY);

    for(int i = 1; i <= 3; i++)
    {
        prevMouseKeys[i] = mouseKeys[i];
        mouseKeys[i] = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(i);
    }
}

bool Input::keyDown(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return keys[key];
}

bool Input::keyHit(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return (keys[key] && !prevKeys[key]);
}

bool Input::keyUp(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return (prevKeys[key] && !keys[key]);
}

bool Input::mouseDown(int key)
{
    if(key < 0 || key > 3)
        return false;

    return mouseKeys[key];
}

bool Input::mouseHit(int key)
{
    if(key < 0 || key > 3)
        return false;

    return (mouseKeys[key] && !prevMouseKeys[key]);
}

bool Input::mouseUp(int key)
{
    if(key < 0 || key > 3)
        return false;

    return (prevMouseKeys[key] && !mouseKeys[key]);
}

int Input::getMouseX()
{
    return mouseX;
}

int Input::getMouseY()
{
    return mouseY;
}

void Input::hideCursor(bool hide)
{
    if(hide)
        SDL_ShowCursor(SDL_DISABLE);
    else
        SDL_ShowCursor(SDL_ENABLE);
}
