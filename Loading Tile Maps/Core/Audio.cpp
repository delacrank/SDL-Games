#include "Audio.h"

bool Audio::init()
{
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
    {
	printf("Failed to initialize audio!\n");
        return false;
    }

    return true;
}

void Audio::kill()
{
    Mix_CloseAudio();
}

bool Audio::musicPlaying()
{
    return Mix_PlayingMusic();
}

bool Audio::musicPaused()
{
    return Mix_PausedMusic();
}

void Audio::pauseMusic()
{
    Mix_PauseMusic();
}

void Audio::resumeMusic()
{
    Mix_ResumeMusic();
}

void Audio::stopMusic()
{
    Mix_HaltMusic();
}

void Audio::stopChannel(int channel)
{
    Mix_HaltChannel(channel);
}
