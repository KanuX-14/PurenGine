#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "main.h"

int drawProcess(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    drawBackground(renderer, isFlashbanged);
    drawUI(renderer);
    SDL_RenderPresent(renderer);

    return 0;
}

int drawBackground(SDL_Renderer *renderer)
{
    SDL_Texture *backgroundTexture;
    SDL_Surface *backgroundSurface;

    if(isFlashbanged)
        backgroundSurface = SDL_LoadBMP("src/images/background_flashbang.bmp");
    else
        backgroundSurface = SDL_LoadBMP("src/images/background.bmp");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

    SDL_FreeSurface(backgroundSurface);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    return 0;
}

int drawUI(SDL_Renderer *renderer)
{
    int i, mouseX, mouseY;
    char buttonPath[128] = "src/images/button/";
    char fontPath[64] = "src/fonts/";
    SDL_GetMouseState(&mouseX, &mouseY);

    if(isFlashbanged)
        strncat(buttonPath, "flashbang/", 11);
    else
        strncat(buttonPath, "dark/", 6);
    
    SDL_Texture *buttonTexture;
    SDL_Surface *buttonSurface;
    SDL_Texture *fontTexture;
    SDL_Surface *fontSurface;

    // Process light button

    int lightButtonSizeX = 32;
    int lightButtonSizeY = 32;
    int lightButtonPosX = windowX - 10 - lightButtonSizeX;
    int lightButtonPosY = windowY - (windowY-10);
    SDL_Rect lightButtonRect = {lightButtonPosX, lightButtonPosY, lightButtonSizeX, lightButtonSizeY};

    // Process mouse interaction

    if(mouseX > lightButtonPosX && mouseX < lightButtonPosX+lightButtonSizeX)
    {
        if(mouseY > lightButtonPosY && mouseY < lightButtonSizeY)
        {
            strncat(buttonPath, "idle-hover.bmp", 15);

            if(isClicked)
            {
                if(isFlashbanged)
                    isFlashbanged = SDL_FALSE;
                else
                    isFlashbanged = SDL_TRUE;
                strncat(buttonPath, "idle-click.bmp", 15);
            }
        }
    }
    else
        strncat(buttonPath, "idle-button.bmp", 16);

    // Process textures

    buttonSurface = SDL_LoadBMP(buttonPath);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

    fontSurface = SDL_LoadBMP("src/images/title/title.bmp");
    fontTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

    SDL_FreeSurface(buttonSurface);
    SDL_FreeSurface(fontSurface);

    // Deploy the modules

    SDL_RenderCopy(renderer, buttonTexture, NULL, &lightButtonRect);

    return 0;
}