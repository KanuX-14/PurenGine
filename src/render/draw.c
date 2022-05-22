#include <stdio.h>
#include "SDL2/SDL.h"
#include "draw.h"

int drawBackground(SDL_Renderer *renderer, SDL_Surface *bitmapSurface, SDL_Texture *bitmapTex, SDL_bool isFlashbanged)
{
    if(isFlashbanged)
        bitmapSurface = SDL_LoadBMP("src/images/background_flashbang.bmp");
    else
        bitmapSurface = SDL_LoadBMP("src/images/background.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    SDL_FreeSurface(bitmapSurface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);

    return 0;
}