#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
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

    if(!running)
        SDL_DestroyTexture(backgroundTexture);

    return 0;
}

int drawUI(SDL_Renderer *renderer)
{
    int i, mouseX, mouseY;
    char buttonPath[128] = "src/images/button/";
    char fontPath[64] = "src/fonts/";
    SDL_Color fontColour = {0, 0, 0};
    SDL_GetMouseState(&mouseX, &mouseY);

    if(isFlashbanged)
    {
        strncat(buttonPath, "flashbang/", 11);
        fontColour.r = 0;
        fontColour.g = 0;
        fontColour.b = 0;
    }
    else
    {
        strncat(buttonPath, "dark/", 6);
        fontColour.r = 255;
        fontColour.g = 255;
        fontColour.b = 255;
    }
    
    SDL_Texture *lightButtonTexture = NULL;
    SDL_Surface *lightButtonSurface = NULL;
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/cantarell/Cantarell-VF.otf", 256);
    SDL_Texture *fontTexture = NULL;

    // Process light button

    int lightButtonSizeX = 32;
    int lightButtonSizeY = 32;
    int lightButtonPosX = windowX - 10 - lightButtonSizeX;
    int lightButtonPosY = windowY - (windowY-10);
    SDL_Rect lightButtonRect = {lightButtonPosX, lightButtonPosY, lightButtonSizeX, lightButtonSizeY};

    // Process font

    SDL_Surface *fontSurface = NULL;
    SDL_Rect fontRect = {windowX/2, windowY-20, 256, 16};

    fontSurface = TTF_RenderText_Solid(font, "PureModManager v0.0.0 testing", fontColour);

    // Process mouse interaction

    if(mouseX > lightButtonPosX && mouseX < lightButtonPosX+lightButtonSizeX && mouseY > lightButtonPosY && mouseY < lightButtonSizeY+10)
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
    else
        strncat(buttonPath, "idle-button.bmp", 16);

    // Process textures

    lightButtonSurface = SDL_LoadBMP(buttonPath);
    lightButtonTexture = SDL_CreateTextureFromSurface(renderer, lightButtonSurface);

    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

    SDL_FreeSurface(lightButtonSurface);
    SDL_FreeSurface(fontSurface);

    // Deploy the modules

    SDL_RenderCopy(renderer, lightButtonTexture, NULL, &lightButtonRect);
    SDL_RenderCopy(renderer, fontTexture, NULL, &fontRect);

    if(!running)
    {
        TTF_Quit();
        SDL_DestroyTexture(lightButtonTexture);
        SDL_DestroyTexture(fontTexture);
    }

    return 0;
}