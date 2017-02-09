#include "Game.h"

Game::Game()
{
    isDone = false;
    fps = 30;
}

Game::~Game()
{

}

unsigned int Game::getTicks()
{
    return SDL_GetTicks();
}

void Game::setFPS(int f)
{
    fps = f;
}

void Game::delay(int ticks)
{
    if(ticks > 0)
        SDL_Delay(ticks);
}

bool Game::initSystem(char title[], int width, int height, bool fullscreen)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    if(!graphics.init(width, height, fullscreen))
        return false;

    graphics.setTitle(title);

    if(!audio.init())
        return false;

    input.init();

    if(TTF_Init() == -1)
        return false;

    return true;
}

void Game::freeSystem()
{
    graphics.kill();
    input.kill();
    audio.kill();
    TTF_Quit();
    SDL_Quit();
}

void Game::run()
{
    while(!isDone)
    {
        unsigned int frameStart = SDL_GetTicks();

        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                isDone = true;
                break;
            }
        }

        input.update();
        update();
        draw(getGraphics());

        getGraphics()->flip();

        int frameTime = getTicks()-frameStart;
        int delayTime = (1000/fps) - frameTime;

        delay(delayTime);
    }

    free();
	freeSystem();
}

void Game::end()
{
    isDone = true;
}

bool Game::init()
{
    return initSystem("Game", 800, 600, false);
}

void Game::free()
{
}

void Game::update()
{

}

void Game::draw(Graphics* g)
{
    g->clear(255,255,255);
}

Graphics* Game::getGraphics()
{
    return &graphics;
}

Input* Game::getInput()
{
    return &input;
}

Audio* Game::getAudio()
{
    return &audio;
}
