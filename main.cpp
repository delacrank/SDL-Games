#include "MapDemo.h"

int main(int argc, char *argv[])
{
    MapDemo game;

    if(!game.init())
    {
        game.free();
        return 0;
    }

    game.run();

    return 0;
}
