#include <SDL2/SDL.h>
    #include <SDL2_mixer/SDL_mixer.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <stdio.h>

// Declare global variables ======================= 
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int PADDLE_WIDTH =100;
    const int PADDLE_HEIGHT = 20;
    const int PADDLE_Y = 550;
    const int BALL_WIDTH = 20;
    const int BALL_HEIGHT = 20;
    const int BALL_SPEED = 10;

    const int PLAYER_SPEED = 10;
    const int FPS = 30;
    const int FRAME_DELAY = 1000/FPS;

    const int GAMEAREA_X1 = 20;
    const int GAMEAREA_Y1 = 20;
    const int GAMEAREA_X2 = 598;
    const int GAMEAREA_Y2 = 600;
    
    const int BLOCK_COLUMNS = 11;
    const int BLOCK_ROWS = 5;
    const int BLOCK_WIDTH = 50;
    const int BLOCK_HEIGHT = 25;

    const int GS_SPLASH = 0;
    const int GS_RUNNING = 1;
    const int GS_GAMEOVER = 2;
    const int GS_PAUSED = 3;

    SDL_Window *Window = NULL;
    
    SDL_Surface *Backbuffer = NULL;
    SDL_Surface *BackgroundImage = NULL;
    SDL_Surface *BallImage = NULL;
    SDL_Surface *PlayerPaddleImage = NULL;
    SDL_Surface *BlockImage = NULL;
    SDL_Surface *SplashImage = NULL;
    SDL_Surface *GameoverImage = NULL;
    SDL_Surface *GamepausedImage = NULL;

    TTF_Font *GameFont = NULL;

    Mix_Chunk *BallBounceSound = NULL;
    Mix_Chunk *BallSpawnSound = NULL;
    Mix_Chunk *ExplosionSound = NULL;

    Mix_Music *GameMusic = NULL;

// Declare structure to handle block variables ======================= 
    struct Block
    {
        SDL_Rect rect;
        bool alive;
        int frame;
    };

// Declare structure to handle ball variables ======================= 
    struct Ball
    {
        SDL_Rect rect;
        int xVel;
        int yVel;
        bool isLocked;
    };

// Declare structure to handle player variables ======================= 
    struct Player
    {
        SDL_Rect rect;
        int lives;
    };

// Declare our struct objects ======================= 
    Block blocks[BLOCK_COLUMNS * BLOCK_ROWS];
    Player player;
    Ball ball;
    
    int gameState;

// Declare global functions ======================= 
    SDL_Surface* LoadImage(char* fileName);
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
    void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x,
                       int y, int width, int height, int frame);
    void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font,
                 Uint8 r, Uint8 g, Uint8 b);
    bool ProgramIsRunning();
    bool LoadFiles();
    void FreeFiles();
    bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);
    bool InitSDL();
    void SetBlocks();
    void DrawBlocks();
    int NumBlocksLeft();
    void ResetGame();
    bool InitGame();
    void UpdatePlayer();
    void UpdateBall();
    void UpdateSplash();
    void DrawSplash();
    void UpdateGame();
    void DrawGame();
    void DrawGamePaused();
    void UpdateGameOver();
    void DrawGameOver();
    void RunGame();
    void DrawScreen();
    void FreeGame();

// Main, Initialize SDL and game loop ======================= 
    int main(int argc, char *argv[])
    {
        if(!InitGame())
        {
            FreeGame();
            return 0;
        }
        
        while(ProgramIsRunning())
        {
            long int oldTime = SDL_GetTicks();
            SDL_FillRect(Backbuffer, NULL, 0);
            RunGame();
            DrawScreen();
            
            int frameTime = SDL_GetTicks() - oldTime;
            
            if(frameTime < FRAME_DELAY)
                SDL_Delay(FRAME_DELAY - frameTime);
            
            SDL_UpdateWindowSurface(Window);
        }
        
        FreeGame();
        return 0;
    }

// Image processing ======================= 
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

// Render image according to coordinates ======================= 
    void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        
        SDL_BlitSurface( image, NULL, destSurface, &destRect);
    }

// Break up block images into segments ======================= 
    void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y,
                       int width, int height, int frame)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        
        int columns = image->w/width;
        
        SDL_Rect sourceRect;
        sourceRect.y = (frame/columns)*height;
        sourceRect.x = (frame%columns)*width;
        sourceRect.w = width;
        sourceRect.h = height;
        
        SDL_BlitSurface(image, &sourceRect, destSurface, &destRect);
    }

// Function for true type font ======================= 
    void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font,
                 Uint8 r, Uint8 g, Uint8 b)
    {
        SDL_Surface* renderedText = NULL;
        SDL_Color color;
        color.r = r;
        color.g = g;
        color.b = b;
        
        renderedText = TTF_RenderText_Solid( font, string, color);
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        
        SDL_BlitSurface(renderedText, NULL, surface, &pos);
        SDL_FreeSurface(renderedText);
    }

// Game loop, handle various game states ======================= 
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
                    if(gameState == GS_RUNNING)
                        gameState = GS_PAUSED;
                    else if(gameState == GS_PAUSED)
                        gameState = GS_RUNNING;
                }
            }
        }
        return running;
    }

// Check if bitmaps and sound files load ======================= 
    bool LoadFiles()
    {
        BackgroundImage = LoadImage("graphics/background.bmp");
        SplashImage = LoadImage("graphics/splash.bmp");
        GameoverImage = LoadImage("graphics/gameover.bmp");
         GamepausedImage = LoadImage("graphics/gamepaused.bmp");
        BallImage = LoadImage("graphics/ball.bmp");
        PlayerPaddleImage = LoadImage("graphics/player.bmp");
        BlockImage = LoadImage("graphics/blocks.bmp");
        
        if(BackgroundImage == NULL)
            return false;
        if(SplashImage == NULL)
            return false;
        if(GameoverImage == NULL)
            return false;
        if(GamepausedImage == NULL)
            return false;
        if(BallImage == NULL)
            return false;
        if(PlayerPaddleImage == NULL)
            return false;
        if(BlockImage == NULL)
            return false;
        
        GameFont = TTF_OpenFont("graphics/brick.ttf", 20);
        
        if(GameFont == NULL)
            return false;
        
        BallBounceSound = Mix_LoadWAV("audio/ball_bounce.wav");
        BallSpawnSound = Mix_LoadWAV("audio/ball_spawn.wav");
        ExplosionSound = Mix_LoadWAV("audio/explosion.wav");
        
        if(BallBounceSound == NULL)
            return false;
        if(BallSpawnSound == NULL)
            return false;
        if(ExplosionSound == NULL)
            return false;
        
        GameMusic = Mix_LoadMUS("audio/song.ogg");
        
        if(GameMusic == NULL)
            return false;
        
        return true;
    }

// Garbage collection ======================= 
    void FreeFiles()
    {
        SDL_FreeSurface(BackgroundImage);
        SDL_FreeSurface(SplashImage);
        SDL_FreeSurface(GameoverImage);
        SDL_FreeSurface(GamepausedImage);
        SDL_FreeSurface(BallImage);
        SDL_FreeSurface(PlayerPaddleImage);
        SDL_FreeSurface(BallImage);
        
        TTF_CloseFont(GameFont);
        
        Mix_FreeChunk(BallBounceSound);
        Mix_FreeChunk(BallSpawnSound);
        Mix_FreeChunk(ExplosionSound);
        
        Mix_FreeMusic(GameMusic);
    }

// Collision Detection ======================= 
    bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
    {
        if(rect1.x >= rect2.x+rect2.w)
            return false;
        if(rect1.y >= rect2.y+rect2.h)
            return false;
        if(rect2.x >= rect1.x+rect1.w)
            return false;
        if(rect2.y >= rect1.y+rect1.h)
            return false;
        
        return true;
    }

// Initialize SDL ======================= 
    bool InitSDL()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            return 0;
        }
        
        Window = SDL_CreateWindow("Interesting",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 800, 600, SDL_WINDOW_SHOWN);
        
        Backbuffer = SDL_GetWindowSurface(Window);
        
        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == - 1)
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

// Set the coordinates for each block ======================= 
    void SetBlocks()
    {
        int i = 0;
        
        for(int x = 0; x < BLOCK_COLUMNS; x++)
        {
            for(int y = 0; y < BLOCK_ROWS; y++)
            {
                blocks[i].rect.x = x * BLOCK_WIDTH + GAMEAREA_X1 + x*3;
                blocks[i].rect.y = (y*2) * BLOCK_HEIGHT + GAMEAREA_Y1 + y*3;
                blocks[i].rect.w = BLOCK_WIDTH;
                blocks[i].rect.h = BLOCK_HEIGHT;
                blocks[i].alive = true;
                blocks[i].frame = i%4;
                i++;
            }
        }
    }

// Render the block images ======================= 
    void DrawBlocks()
    {
        for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
        {
            if(blocks[i].alive)
            {
                DrawImageFrame(BlockImage, Backbuffer, blocks[i].rect.x, blocks[i].rect.y,
                               blocks[i].rect.w, blocks[i].rect.h, blocks[i].frame);
            }
        }
    }

// Count the amount of blocks left======================= 
    int NumBlocksLeft()
    {
        int result = 0;
        
        for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
        {
            if(blocks[i].alive)
            {
                result++;
            }
        }
        return result;
    }

// Set location for paddles, blocks and ball ======================= 
    void ResetGame()
    {
        player.rect.x = (GAMEAREA_X2-GAMEAREA_X1)/2 - PADDLE_WIDTH/2 
        + GAMEAREA_X1;
        player.rect.y = PADDLE_Y;
        player.rect.w = PADDLE_WIDTH;
        player.rect.h = PADDLE_HEIGHT;
        
        ball.rect.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
        ball.rect.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
        ball.rect.w = BALL_WIDTH;
        ball.rect.h = BALL_HEIGHT;
        
        Mix_PlayChannel(-1, BallSpawnSound, 0);
        
        SetBlocks();
        ball.isLocked = true;
        player.lives = 3;
    }
        
// Intro Game state ======================= 
    bool InitGame()
    {
        if(!InitSDL())
            return false;
        if(!LoadFiles())
            return false;
        
        Mix_PlayMusic(GameMusic, -1);
        gameState = GS_SPLASH;
        
        return true;
    }

// Move player paddle ======================= 
    void UpdatePlayer()
    {
        Uint8 *keys = (Uint8*)SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_LEFT])
            player.rect.x -= PLAYER_SPEED;
        
        if(keys[SDL_SCANCODE_RIGHT])
            player.rect.x += PLAYER_SPEED;
        
        if(keys[SDL_SCANCODE_SPACE] && ball.isLocked)
        {
            ball.isLocked = false;
            ball.xVel = rand()%3 - 1;
            ball.yVel = BALL_SPEED;
        }
        
        if(player.rect.x < GAMEAREA_X1)
            player.rect.x = GAMEAREA_X1;
        
        if(player.rect.x > GAMEAREA_X2-player.rect.w)
            player.rect.x = GAMEAREA_X2-player.rect.w;
    }

// Handle ball physics for when ball hits blocks or sides of screen ======= 
    void UpdateBall()
    {
        if(ball.isLocked)
        {
            int PaddleCenterX = player.rect.x + player.rect.w/2;
            ball.rect.x = PaddleCenterX - ball.rect.w/2;
            ball.rect.y = player.rect.y - ball.rect.h;
        }
        else
        {
            ball.rect.x += ball.xVel;
            
            if(RectsOverlap(ball.rect, player.rect))
            {
                ball.rect.x -= ball.xVel;
                ball.xVel *= -1;
                Mix_PlayChannel(-1, BallBounceSound, 0);
            }
            else
            {
                for(int i = 0; i < BLOCK_COLUMNS*BLOCK_ROWS; i++)
                {
                    if(blocks[i].alive && RectsOverlap(ball.rect, blocks[i].rect))
                    {
                        ball.rect.x -= ball.xVel;
                        ball.xVel *= -1;
                        blocks[i].alive = false;
                        Mix_PlayChannel(-1, ExplosionSound, 0);
                    }
                }
            }
            ball.rect.y += ball.yVel;
            
            if(RectsOverlap(ball.rect, player.rect))
            {
                ball.rect.y -= ball.yVel;
                ball.yVel *= -1;
                
                int ballCenterX = ball.rect.x+ball.rect.w/2;
                int paddleCenterX = player.rect.x+player.rect.w/2;
                
                ball.xVel = (ballCenterX - paddleCenterX)/5;
                Mix_PlayChannel(-1, BallBounceSound, 0);
            }
            else
            {
< BLOCK_COLUMNS*BLOCK_ROWS; i++)
                {
                    if(blocks[i].alive && RectsOverlap(ball.rect, blocks[i].rect))
                    {
                        ball.rect.y -= ball.yVel;
                        ball.yVel *= -1;
                        blocks[i].alive = false;
                        Mix_PlayChannel(-1, ExplosionSound, 0);
                    }
                }
            }
            
            if(ball.rect.y < GAMEAREA_Y1)
            {
                ball.rect.y < GAMEAREA_Y1;
                ball.yVel *= -1;
                Mix_PlayChannel(-1, BallBounceSound, 0);
            }
            
            if(ball.rect.x > GAMEAREA_X2 - ball.rect.w)
            {
                ball.rect.x = GAMEAREA_X2 - ball.rect.w;
                ball.xVel *= -1;
                Mix_PlayChannel(-1, BallBounceSound, 0);
            }
            
            if(ball.rect.x < GAMEAREA_X1)
            {
                ball.rect.x = GAMEAREA_X1;
                ball.xVel *= -1;
                Mix_PlayChannel(-1, BallBounceSound, 0);
            }
            
            if(ball.rect.y > GAMEAREA_Y2)
            {
                ball.isLocked = true;
                int PaddleCenterX = player.rect.x + player.rect.w/2;
                ball.rect.x = PaddleCenterX - ball.rect.w/2;
                ball.rect.y = player.rect.y - ball.rect.h;
                
                player.lives--;
                Mix_PlayChannel(-1, BallSpawnSound, 0);
            }
        }
    }

// Update anothe game state ======================= 
    void UpdateSplash()
    {
        Uint8 *keys = (Uint8*)SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_RETURN])
        {
            ResetGame();
            gameState = GS_RUNNING;
        }
    }

// Draw images for each game state pause, intro, gameover ============= 
    void DrawSplash()
    {
        DrawImage(SplashImage, Backbuffer, 0, 0);
    }

    void UpdateGame()
    {
        UpdatePlayer();
        UpdateBall();
        
        if(player.lives <= 0)
            gameState = GS_GAMEOVER;
        
        if(NumBlocksLeft() <= 0)
            ResetGame();
    }

// Draw paddle, blocks, text and background ======================= 
    void DrawGame()
    {
        DrawImage(BackgroundImage, Backbuffer, 0, 0);
        DrawImage(BallImage, Backbuffer, ball.rect.x, ball.rect.y);
        DrawImage(PlayerPaddleImage, Backbuffer, player.rect.x, player.rect.y);
        DrawBlocks();
        
        char blocksText[64];
        char livesText[64];
        
        sprintf(blocksText, "Blocks: %d", NumBlocksLeft());
        sprintf(livesText, "Lives: %d", player.lives);
        DrawText(Backbuffer, blocksText, 645, 150, GameFont, 255, 255, 255);
        DrawText(Backbuffer, livesText, 645, 170, GameFont, 255, 255, 255);
    }

// Draw image for paused game state ======================= 
    void DrawGamePaused()
    {
        DrawGame();
        DrawImage(GamepausedImage, Backbuffer, 0, 0);
    }

// Update game state ======================= 
    void UpdateGameOver()
    {
        Uint8 *keys = (Uint8*)SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_SPACE])
        {
            gameState = GS_SPLASH;
        }
    }

// Draw image for game over state ======================= 
    void DrawGameOver()
    {
        DrawGame();
        DrawImage(GameoverImage, Backbuffer, 0, 0);
    }

// Event handling for various game states ======================= 
    void RunGame()
    {
        switch(gameState)
        {
            case GS_SPLASH:
                UpdateSplash();
                break;
                    
            case GS_RUNNING:
                UpdateGame();
                break;
                
            case GS_GAMEOVER:
                UpdateGameOver();
                break;
                
            default:
                break;                
        }
    }

// Image handling for various game states ======================= 
    void DrawScreen()
    {
        switch(gameState)
        {
            case GS_SPLASH:
                DrawSplash();
                break;
                
            case GS_RUNNING:
                DrawGame();
                break;
                
            case GS_GAMEOVER:
                DrawGameOver();
                break;
                
            case GS_PAUSED:
                DrawGamePaused();
                break;
                
            default:
                break;
        }
    }

// Garbage collection ======================= 
    void FreeGame()
    {
        SDL_DestroyWindow(Window);
        Mix_HaltMusic();
        FreeFiles();
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
    }