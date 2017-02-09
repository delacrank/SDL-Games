#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

class Graphics
{
private:
    SDL_Surface* backbuffer;
    SDL_Window* window;              //This is a handle to a window in SDL 2
    int width;
    int height;
public:
    bool init(int aWidth, int aHeight, bool aFullscreen);
    void kill();                     //This function was added to destroy windows
    void setTitle(char* title);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawRect(int x, int y, int width, int height, int r, int g, int b);
    void fillRect(int x, int y, int width, int height, int r, int g, int b);
    void clear(int r, int g, int b);
    void flip();
    int getWidth();
    int getHeight();
    SDL_Surface* getBackbuffer();
};

#endif
