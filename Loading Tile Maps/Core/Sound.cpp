#include "Sound.h"

Sound::Sound()
{
    sound = NULL;
}

Sound::~Sound()
{

}

bool Sound::load(char fileName[])
{
    sound = Mix_LoadWAV(fileName);

    if(sound == NULL)
    {
        printf("Failed to load sound: ");
        printf(fileName);
        printf("\n");
        
        return false;
    }

    return true;
}

void Sound::free()
{
    if(sound != NULL)
    {
        Mix_FreeChunk(sound);
        sound = NULL;
    }
}

int Sound::play(int loops)
{
    if(sound != NULL)
    {
        return Mix_PlayChannel(-1, sound, loops);
    }
    else
    {
        return -1;
    }
}

bool Sound::isLoaded()
{
    return (sound != NULL);
}
