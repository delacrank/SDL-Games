#include "Image.h"

Image::Image()
{
    surface = NULL;
    width = 0;
    height = 0;
    frameWidth = 0;
    frameHeight = 0;
}

Image::~Image()
{

}

bool Image::load(char fileName[], Graphics* g)
{
    SDL_Surface* imageLoaded = NULL;
    imageLoaded = IMG_Load(fileName);

    if(imageLoaded != NULL)
    {
        //SDL_DisplayFormat is gone in SDL 2, we can use SDL_ConvertSurface instead
        surface = SDL_ConvertSurface(imageLoaded, g->getBackbuffer()->format, 0);
        SDL_FreeSurface(imageLoaded);

        if(surface != NULL)
        {
            Uint32 colorKey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF );

            //SDL_SRCCOLORKEY is gone in SDL 2, use SDL_TRUE instead
            SDL_SetColorKey(surface, SDL_TRUE, colorKey );
            width = surface->w;
            height = surface->h;
        }
        else
        {
            printf("Failed to load image: ");
            printf(fileName);
            printf("\n");
            return false;
        }
    }
    else
    {
        printf("Failed to load image: ");
        printf(fileName);
        printf("\n");
        return false;
    }

    return true;
}

bool Image::load(char fileName[], int aFrameWidth, int aFrameHeight, Graphics* g)
{
    if(load(fileName, g))
    {
        frameWidth = aFrameWidth;
        frameHeight = aFrameHeight;

        return true;
    }

    return false;
}

void Image::draw(int x, int y, Graphics* g)
{
    if(surface == NULL)
        return;

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface(surface, NULL, g->getBackbuffer(), &destRect);
}

void Image::draw(int x, int y, int frame, Graphics* g)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int columns = width/frameWidth;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/columns)*frameHeight;
    sourceRect.x = (frame%columns)*frameWidth;
    sourceRect.w = frameWidth;
    sourceRect.h = frameHeight;

    SDL_BlitSurface(surface, &sourceRect, g->getBackbuffer(), &destRect);
}

void Image::free()
{
    if(surface != NULL)
    {
        SDL_FreeSurface(surface);
        surface = NULL;
    }
}

int Image::getWidth()
{
    return width;
}

int Image::getHeight()
{
    return height;
}

int Image::getFrameWidth()
{
    return frameWidth;
}

int Image::getFrameHeight()
{
    return frameHeight;
}

void Image::setFrameSize(int w, int h)
{
    frameWidth = w;
    frameHeight = h;
}

bool Image::isLoaded()
{
    return (surface != NULL);
}
