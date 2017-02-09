#include <SDL2/SDL.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <SDL2_mixer/SDL_mixer.h>
    #include <iostream>
    #include <cstdlib>
    #include <stdio.h>


// Declare Global Variables============================
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const int PADDLE_WIDTH = 20;
    const int PADDLE_HEIGHT = 100;
    const int BALL_WIDTH = 20;
    const int BALL_HEIGHT = 20;
    const int BALL_MAX_SPEED = 20;

    const int PLAYER_PADDLE_X = PADDLE_WIDTH;
    const int ENEMY_PADDLE_X = SCREEN_WIDTH - PADDLE_WIDTH*2;
    
    const int PLAYER_SPEED = 10;
    const int ENEMY_SPEED = 10;
    
    const int FPS = 30;
    const int FRAME_DELAY = 1000/FPS;

    SDL_Window* Window = NULL;

    SDL_Surface *Backbuffer = NULL;
    SDL_Surface *BackgroundImage = NULL;
    SDL_Surface *BallImage = NULL;
    SDL_Surface *PlayerPaddleImage = NULL;
    SDL_Surface *EnemyPaddleImage = NULL;

    TTF_Font *GameFont = NULL;

    Mix_Chunk *BallBounceSound = NULL;
    Mix_Chunk *BallSpawnSound = NULL;
    Mix_Chunk *PlayerScoreSound = NULL;
    Mix_Chunk *EnemyScoreSound = NULL;
    
    Mix_Music *GameMusic = NULL;

    int PlayerScore;
    int EnemyScore;
    
    int BallXVel;
    int BallYVel;

    SDL_Rect PlayerPaddleRect;
    SDL_Rect EnemyPaddleRect;
    SDL_Rect BallRect;

    SDL_Surface* LoadImage(char* fileName);
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
    void DrawText(SDL_Surface* surface, char* string, int x, int y,
    TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
    bool ProgramIsRunning();
    bool LoadFiles();
    void FreeFiles();
    bool RectsOverlap();
    bool InitSDL();
    void ResetGame();
    bool InitGame();
    void UpdatePlayer();
    void UpdateAI();
    void UpdateBall();
    void RunGame();
    void DrawGame();
    void FreeGame();

    int main(int argc, char *argv[])
    {
// Check to see if Initialization successful========================
        if(!InitGame())
        {
            FreeGame();
            return 0;
        }
        
// Set frame rate & Run game functions============================
        while(ProgramIsRunning())
        {
            long int oldTime = SDL_GetTicks();
            SDL_FillRect(Backbuffer, NULL, 0);
            RunGame();
            DrawGame();
            
            int frameTime = SDL_GetTicks() - oldTime;
            
            if(frameTime < FRAME_DELAY)
                SDL_Delay(FRAME_DELAY - frameTime);
            
            SDL_UpdateWindowSurface(Window);
        }
        
        FreeGame();
        
        return 0;
    }

// Convert Images to SDL format============================
    SDL_Surface* LoadImage(char* fileName)
    {
        SDL_Surface* imageLoaded = NULL;
        SDL_Surface* processedImage = NULL;
        
        imageLoaded = SDL_LoadBMP(fileName);
        
        if(imageLoaded != NULL)
        {
            processedImage = SDL_ConvertSurface(imageLoaded, Backbuffer->format, 0);
            SDL_FreeSurface(imageLoaded);
            
            if(processedImage !=  NULL)
            {
                Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0, 0xFF);
                SDL_SetColorKey(processedImage, SDL_TRUE, colorKey);
            }
        }
        
        return processedImage;
    }

// Draw Images along x and y axises============================
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        
        SDL_BlitSurface( image, NULL, destSurface, &destRect);
    }

// Function for rendering fonts============================
    void DrawText(SDL_Surface* surface, char* string, int x, int y,
    TTF_Font* font, Uint8 r, Uint8 g, Uint8 b)
    {
        SDL_Surface* renderedText = NULL;
        SDL_Color color;
        
        color.r = r;
        color.g = g;
        color.b = b;
        
        renderedText = TTF_RenderText_Solid( font, string, color );
        
        SDL_Rect pos;
        
        pos.x = x;
        pos.y = y;
        
        SDL_BlitSurface( renderedText, NULL, surface, &pos);
        SDL_FreeSurface(renderedText);
    }

// Keep Window open unless esc is pressed=====================
    bool ProgramIsRunning()
    {
        SDL_Event event;
        bool running = true;
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
            
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }
        
        return running;
    }

// Load Images, Font and Sound files============================
    bool LoadFiles()
    {
        BackgroundImage = LoadImage("graphics/background.bmp");
        BallImage = LoadImage("graphics/ball.bmp");
        PlayerPaddleImage = LoadImage("graphics/player.bmp");
        EnemyPaddleImage = LoadImage("graphics/enemy.bmp");
        
        if(BackgroundImage == NULL)
            return false;
        if(BallImage == NULL)
            return false;
        if(PlayerPaddleImage == NULL)
            return false;
        
        GameFont = TTF_OpenFont("graphics/alfphabet.ttf", 30);
        
        BallBounceSound = Mix_LoadWAV("audio/ballBounce.wav");
        BallSpawnSound = Mix_LoadWAV("audio/ballSpawn.wav");
        PlayerScoreSound = Mix_LoadWAV("audio/playerScore.wav");
        EnemyScoreSound = Mix_LoadWAV("audio/enemyScore.wav");
        
        if(BallBounceSound == NULL)
            return false;
        if(BallSpawnSound == NULL)
            return false;
        if(PlayerScoreSound == NULL)
            return false;
        if(EnemyScoreSound == NULL)
            return false;
        
        GameMusic = Mix_LoadMUS("audio/song.ogg");
        
        if(GameMusic == NULL)
            return false;
        
        return true;
    }

// Release files from memory============================
    void FreeFiles()
    {
        SDL_FreeSurface(BackgroundImage);
        SDL_FreeSurface(BallImage);
        SDL_FreeSurface(PlayerPaddleImage);
        SDL_FreeSurface(EnemyPaddleImage);
        
        TTF_CloseFont(GameFont);
        
        Mix_FreeChunk(BallBounceSound);
        Mix_FreeChunk(BallSpawnSound);
        Mix_FreeChunk(PlayerScoreSound);
        Mix_FreeChunk(EnemyScoreSound);
        
        Mix_FreeMusic(GameMusic);
    }

// Collision dectection for ball and paddle=======================
    bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
    {
        if(rect1.x >= rect2.x + rect2.w)
            return false;
        
        if(rect1.y >= rect2.y + rect2.h)
            return false;
        
        if(rect2.x >= rect1.x + rect1.w)
            return false;
        
        if(rect2.y >= rect1.y + rect1.h)
            return false;
        
        return true;
    }

// Initialize SDL, Sound, and Font libraries=========================
    bool InitSDL()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
            return false;
        
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            return 0;
        }
        
        Window = SDL_CreateWindow("Interesting",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 800, 600, SDL_WINDOW_SHOWN);
        
        Backbuffer = SDL_GetWindowSurface(Window);
        
        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            return 0;
        }
        
        if(TTF_Init() == -1)
        {
            return 0;
        }
        
        if(Backbuffer == NULL)
            return false;
        
        return true;
    }

// Set Paddles and Ball in correct location======================
    void ResetGame()
    {
        PlayerPaddleRect.x = PLAYER_PADDLE_X;
        PlayerPaddleRect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
        PlayerPaddleRect.w = PADDLE_WIDTH;
        PlayerPaddleRect.h = PADDLE_HEIGHT;
        
        EnemyPaddleRect.x = ENEMY_PADDLE_X;
        EnemyPaddleRect.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
        EnemyPaddleRect.w = PADDLE_WIDTH;
        EnemyPaddleRect.h = PADDLE_HEIGHT;
        
        BallRect.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
        BallRect.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
        BallRect.w = BALL_WIDTH;
        BallRect.h = BALL_HEIGHT;
        
        BallXVel = rand()%BALL_MAX_SPEED + 1;
        
        BallYVel = (rand()%BALL_MAX_SPEED*2 + 1) - BALL_MAX_SPEED;
        
        if(rand()%2 == 0)
            BallXVel *= -1;
        
        Mix_PlayChannel(-1, BallSpawnSound, 0);
    }

// Set Music, Scores, Files, and Initialization==================
    bool InitGame()
    {
        if(!InitSDL())
            return false;
        if(!LoadFiles())
            return false;
        
        PlayerScore = 0;
        EnemyScore = 0;
        
        ResetGame();
        Mix_PlayMusic(GameMusic, -1);
        
        return true;
    }

// Update Player Movements============================
    void UpdatePlayer()
    {
        Uint8 *keys = (Uint8*)SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_UP])
            PlayerPaddleRect.y -= PLAYER_SPEED;
        
        if(keys[SDL_SCANCODE_DOWN])
            PlayerPaddleRect.y += PLAYER_SPEED;
        
        if(PlayerPaddleRect.y < 0)
            PlayerPaddleRect.y = 0;
        
        if(PlayerPaddleRect.y > SCREEN_HEIGHT-PlayerPaddleRect.h)
            PlayerPaddleRect.y = SCREEN_HEIGHT-PlayerPaddleRect.h;
        
       }

// Update Enemy Artifical Intelligence============================
    void UpdateAI()
    {
        if((EnemyPaddleRect.y + EnemyPaddleRect.h/2) > (BallRect.y+BallRect.h/2))
            EnemyPaddleRect.y -= ENEMY_SPEED;
        
        if((EnemyPaddleRect.y + EnemyPaddleRect.h/2) < (BallRect.y+BallRect.h/2))
            EnemyPaddleRect.y += ENEMY_SPEED;
        
        if(EnemyPaddleRect.y < 0)
            EnemyPaddleRect.y = 0;
        
        if(EnemyPaddleRect.y > SCREEN_HEIGHT-EnemyPaddleRect.h)
            EnemyPaddleRect.y = SCREEN_HEIGHT-EnemyPaddleRect.h;
    } 
    
// Determine the Physics for the ball & Collision reactions============
    void UpdateBall()
    {
        BallRect.x += BallXVel;
        BallRect.y += BallYVel;
        
        if(RectsOverlap(BallRect, PlayerPaddleRect))
        {
            BallXVel = rand()%BALL_MAX_SPEED + 1;
            Mix_PlayChannel(-1, BallBounceSound, 0);
        }
        
        if(RectsOverlap(BallRect, EnemyPaddleRect))
        {
            BallXVel = (rand()%BALL_MAX_SPEED + 1) * -1;
            Mix_PlayChannel(-1, BallBounceSound, 0);
        }
        
        if(BallRect.y < 0)
        {
            BallRect.y = 0;
            BallYVel = rand()%BALL_MAX_SPEED + 1;
            Mix_PlayChannel(-1, BallBounceSound, 0);
        }
            
        if(BallRect.y > SCREEN_HEIGHT - BallRect.h)
        {
            BallRect.y = SCREEN_HEIGHT - BallRect.h;
            BallYVel = (rand()%BALL_MAX_SPEED + 1)* -1;
            Mix_PlayChannel(-1, BallBounceSound, 0);
        }
        
        if(BallRect.x > SCREEN_WIDTH)
        {
            PlayerScore++;
            Mix_PlayChannel(-1, PlayerScoreSound, 0);
            ResetGame();
        }
        
        if(BallRect.x < 0-BallRect.h)
        {
            EnemyScore++;
            Mix_PlayChannel(-1, EnemyScoreSound, 0);
            ResetGame();
        }
    }

// Call the Player, Artifical Intelligence and Ball Physics functions=========
    void RunGame()
    {
        UpdatePlayer();
        UpdateAI();
        UpdateBall();
    }

// Render Images & Text to the screen in correct locations===============
    void DrawGame()
    {
        DrawImage(BackgroundImage, Backbuffer, 0, 0);
        DrawImage(BallImage, Backbuffer, BallRect.x, BallRect.y);
        DrawImage(PlayerPaddleImage, Backbuffer, PlayerPaddleRect.x, PlayerPaddleRect.y);
        DrawImage(EnemyPaddleImage, Backbuffer, EnemyPaddleRect.x, EnemyPaddleRect.y);
        
        char playerHUD[64];
        char enemyHUD[64];
        
        sprintf(playerHUD, "Player Score: %d", PlayerScore);
        sprintf(enemyHUD, "Enemy Score: %d", EnemyScore);
        
        DrawText(Backbuffer, playerHUD, 0, 1, GameFont, 64, 64, 64);
        DrawText(Backbuffer, enemyHUD, 0, 30, GameFont, 64, 64, 64);
    }

// Garbage collection & Quit: SDL, Audio,  and font libs=================
    void FreeGame()
    {
        SDL_DestroyWindow(Window);
        Mix_HaltMusic();
        FreeFiles();
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
    }