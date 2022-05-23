#include <stdio.h>
#include "SDL2/SDL.h"
#include "draw.h"
#include "keyboard.h"

int main(int argc, char* argv[]) {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    int init_posX = SDL_WINDOWPOS_UNDEFINED;
    int init_posY = SDL_WINDOWPOS_UNDEFINED;
    int init_width = 640;
    int init_height = 480;
    int i, x, y, key, cursor;
    Uint32 keys;
    SDL_bool running = SDL_TRUE;
    SDL_bool isFlashbanged = SDL_FALSE;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_PumpEvents();

    window = SDL_CreateWindow( "Pure Mod Manager", init_posX, init_posY, init_width, init_height, SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Process the launcher

    while(running)
    {
        SDL_Event event;

        keys = SDL_GetMouseState(&x, &y);

        // Listen to the X button

        while(SDL_PollEvent(&event))
        {
            key = event.key.keysym.scancode;

            switch(event.type)
            {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch(key)
                    {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_TAB:
                        case SDL_SCANCODE_LSHIFT:
                            cursor = keyboardCursor(key);
                            break;
                        default:
                            break;
                    }
            }
        }

        drawBackground(renderer, bitmapSurface, bitmapTex, isFlashbanged);

        SDL_Delay(1000/5); // Lock to 5 FPS for low consumption
    }

    // Safely free the allocated memory

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}