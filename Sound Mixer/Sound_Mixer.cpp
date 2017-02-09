#include <SDL2/SDL.h>
    #include <SDL2_mixer/SDL_mixer.h>
    #include <iostream>

    Mix_Music *Music = NULL;

    SDL_Surface* Background = NULL;
    SDL_Surface* Backbuffer = NULL;

    SDL_Window* Window = NULL;

    bool LoadFiles();
    void FreeFiles();

    SDL_Surface* LoadImage(char* fileName);
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
    bool ProgramIsRunning();

    int main(int argc, char* argcs[])
    {
// Initialize Everything================================= 
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "Error" << std::endl;
            SDL_Quit();
            return 0;
        }
    
        Window = SDL_CreateWindow("interesting",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             800, 600, SDL_WINDOW_SHOWN);
    
        Backbuffer = SDL_GetWindowSurface(Window);
    
// Initialize Audio================================= 
        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            std::cout << "Error" << std::endl;
            SDL_DestroyWindow(Window);
            SDL_Quit();
            return 0;
        }
    
        if(!LoadFiles())
        {   
            std::cout << "Error" << std::endl;
            SDL_DestroyWindow(Window);
            FreeFiles();
            Mix_CloseAudio();
            SDL_Quit();
            return 0;
        }
    
        while(ProgramIsRunning())
        {
            DrawImage(Background, Backbuffer, 0, 0);
            SDL_Delay(20);
            SDL_UpdateWindowSurface(Window);
        }
    
        FreeFiles();
        Mix_CloseAudio();
        SDL_Quit();
    
        return 1;
    }

// Load our sound track and background =========================== 
    bool LoadFiles()
    {
        Background = LoadImage("graphics/background5.bmp");
    
        if(Background == NULL)
            return false;
    
// Here we use 'Mix_LoadMus()' for our audio file ====================== 
        Music = Mix_LoadMUS("audio/happy_song.ogg");
    
    
        if(Music == NULL)
            return false;
    
        return true;
    }

    void FreeFiles()
    {
        if(Background != NULL)
        {
            SDL_FreeSurface(Background);
            Background = NULL;
        }
    
        if(Music != NULL)
        {
// We use 'Mix_FreeMusic()' to free up these files ==================== 
            Mix_FreeMusic(Music);
            Music = NULL;
        }
    }

    SDL_Surface* LoadImage(char* fileName)
    {
        SDL_Surface* imageLoaded = NULL;
        SDL_Surface* processedImage = NULL;
    
        imageLoaded = SDL_LoadBMP(fileName);
    
        if(imageLoaded != NULL)
        {
            processedImage = SDL_ConvertSurface(imageLoaded, Backbuffer->format, 0);
            SDL_FreeSurface(imageLoaded);
        
            if(processedImage != NULL)
            {
                Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0, 0xFF);
                SDL_SetColorKey(processedImage, SDL_TRUE, colorKey);
            }
        }
        return processedImage;
    }

    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
    
        SDL_BlitSurface(image, NULL, destSurface, &destRect);
    }

// Event Handling ================================= 
    bool ProgramIsRunning()
    {
        SDL_Event event;
    
        bool running = true;
    
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        
            if(event.type = SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    
                    case SDLK_BACKSPACE:
// Here we use 'Mix_HaltMusic()' ============================= 
                        Mix_HaltMusic();
                        break;
                    
                    case SDLK_SPACE:
                        if(!Mix_PlayingMusic())
                        {
// 'Mix_PlayMusic()' starts our sound =========================== 
                            Mix_PlayMusic(Music, -1);
                        }
                        else
                        {
                            if(Mix_PausedMusic())
                            {
                                Mix_ResumeMusic();
                            }
                            else
                            {
                                Mix_PauseMusic();
                            }
                        }
                        break;
                    
                    default:
                        break;
                }
            }
        }
    
        return running;
    }