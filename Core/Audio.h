#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <stdio.h>

class Audio
{
private:
public:
    bool init();
    void kill();

    bool musicPlaying();
    bool musicPaused();
    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    void stopChannel(int channel);
};

#endif
