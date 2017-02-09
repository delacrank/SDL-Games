#include "MapDemo.h"

MapDemo::MapDemo()
{
    mapX = mapY = 0;
}

MapDemo::~MapDemo()
{

}

bool MapDemo::init()
{
    if(!initSystem("Map Demo 3", 800, 600, false))
        return false;

    //Since we're using SDL 2, we need pointers to a Graphics instance in order
    //to load images
    if(!map.load("graphics/platformer.map", "graphics/tiles.bmp", getGraphics()))
        return false;

    if(!background.load("graphics/background.bmp", getGraphics()))
        return false;

    return true;
}

void MapDemo::update()
{
    Input* in = getInput();

    //Since we're using SDL 2, we have to use SDL_SCANCODE_(KEY) instead of SDLK_(KEY)
    if(in->keyDown(SDL_SCANCODE_UP))
        mapY -= SCROLL_SPEED;

    if(in->keyDown(SDL_SCANCODE_DOWN))
        mapY += SCROLL_SPEED;

    if(in->keyDown(SDL_SCANCODE_LEFT))
        mapX -= SCROLL_SPEED;

    if(in->keyDown(SDL_SCANCODE_RIGHT))
        mapX += SCROLL_SPEED;
}

void MapDemo::draw(Graphics* g)
{
    background.draw(0,0, g);
    map.draw(mapX, mapY, g);
}

void MapDemo::free()
{
    map.free();
    background.free();
    freeSystem();
}
