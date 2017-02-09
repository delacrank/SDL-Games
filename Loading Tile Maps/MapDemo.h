#ifndef MAPDEMO_H
#define MAPDEMO_H

#include "Core/Game.h"
#include "Map.h"

class MapDemo : public Game
{
private:
    Map map;
    Image background;
    int mapX, mapY;

    static const int SCROLL_SPEED = 10;
public:
    MapDemo();
    ~MapDemo();
    virtual bool init();
    virtual void update();
    virtual void draw(Graphics* g);
    virtual void free();
};

#endif
