#include "RasterFont.h"

bool RasterFont::load(char fileName[], Graphics* g)
{
    if(!image.load(fileName, g))
        return false;

    charSize = image.getWidth()/NUM_COLUMNS;

    image.setFrameSize(charSize, charSize);

    return true;
}

void RasterFont::draw(char text[], int x, int y, Graphics* g)
{
    if(!image.isLoaded())
        return;

    for(int i = 0; i < strlen(text); i++)
    {
        image.draw(x+i*charSize, y, text[i]-START_CHAR, g);
    }
}

void RasterFont::free()
{
    image.free();
}
