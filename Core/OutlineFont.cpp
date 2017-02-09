#include "OutlineFont.h"

OutlineFont::OutlineFont()
{
    font = NULL;
}

OutlineFont::~OutlineFont()
{

}

bool OutlineFont::load(char fileName[], int size)
{
    font = TTF_OpenFont(fileName, size);

    if(font == NULL)
        return false;

    return true;
}

void OutlineFont::free()
{
    if(font != NULL)
    {
        TTF_CloseFont(font);
    }
}

void OutlineFont::draw(char text[], int x, int y, int r, int g, int b, Graphics* gfx)
{
    if(font == NULL)
        return;

    SDL_Surface* renderedText = NULL;

    SDL_Color color;

    color.r = r;
    color.g = g;
    color.b = b;

    renderedText = TTF_RenderText_Solid(font, text, color);

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

    SDL_BlitSurface(renderedText, NULL, gfx->getBackbuffer(), &pos );
    SDL_FreeSurface(renderedText);
}

