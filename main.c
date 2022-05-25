#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "tools.h"
#include "draw.h"
#include "keyboard.h"
#include "mouse.h"

int init_width = 640;
int init_height = 480;
int windowX;
int windowY;
int isClicked = 0;
SDL_bool running = SDL_TRUE;
SDL_bool isFlashbanged = SDL_FALSE;

int main(int argc, char* argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int init_posX = SDL_WINDOWPOS_UNDEFINED;
    int init_posY = SDL_WINDOWPOS_UNDEFINED;
    //int key, cursor;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_PumpEvents();

    window = SDL_CreateWindow("Pure Mod Manager", init_posX, init_posY, init_width, init_height, SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(window == NULL)
    {
        debug("Could not create window.\n", 2);
        SDL_GetError();
        return 1;
    }

    if(renderer == NULL)
    {
        debug("Could not create renderer.\n", 2);
        SDL_GetError();
        return 1;
    }

    SDL_GetWindowSize(window, &windowX, &windowY);

    // Process the launcher

    while(running)
    {
        SDL_Event event;

        // Restart variables

        isClicked = 0;

        // Listen to the X button

        while(SDL_PollEvent(&event))
        {
            //key = event.key.keysym.scancode;

            switch(event.type)
            {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    // switch(key)
                    // {
                    //     case SDL_SCANCODE_ESCAPE:
                    //     case SDL_SCANCODE_TAB:
                    //     case SDL_SCANCODE_LSHIFT:
                    //         cursor = keyboardCursor(key);
                    //         break;
                    //     default:
                    //         break;
                    // }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    isClicked = 1;
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            {
                                windowX = event.window.data1;
                                windowY = event.window.data2;
                            }
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        drawProcess(renderer);

        SDL_Delay(1000/15); // Lock to 15 FPS for low consumption
    }

    // Safely free the allocated memory

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}