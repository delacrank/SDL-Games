#ifndef IMAGE_H
#define IMAGE_H

#include "Graphics.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

class Image
{
private:
    SDL_Surface* surface;
    int width;
    int height;
    int frameWidth;
    int frameHeight;
public:
    Image();
    ~Image();
    bool load(char fileName[], Graphics* g);
    bool load(char fileName[], int aFrameWidth, int aFrameHeight, Graphics* g);
    void draw(int x, int y, Graphics* g);
    void draw(int x, int y, int frame, Graphics* g);
    void free();

    int getWidth();
    int getHeight();
    int getFrameWidth();
    int getFrameHeight();
    void setFrameSize(int w, int h);
    bool isLoaded();
};

#endif
