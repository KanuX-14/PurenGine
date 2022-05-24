#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "main.h"
#include "tools.h"

struct button
{
    char path[1024];
    char iconPath[1024];
    char mode[128];
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *iconSurface;
    SDL_Texture *iconTexture;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
    char content[2048];
};

struct font
{
    TTF_Font *font;
    SDL_Surface *fontSurface;
    SDL_Texture *fontTexture;
    SDL_Color colour;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
};

struct button searchDirectory;
struct button fileViewer;

struct font font;

// Main process.
int drawProcess(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    drawBackground(renderer);
    drawUI(renderer);
    SDL_RenderPresent(renderer);

    return 0;
}

// Define the background theme.
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

// Process the UI modules.
int drawUI(SDL_Renderer *renderer)
{
    int i, mouseX, mouseY;
    char path[64];
    char buttonPath[128];
    char iconPath[128];
    char filename[1024];
    
    strncat(path, getenv("HOME"), 16);
    strncat(path, "/.local/share/PureModManager/", 31);

    strncpy(buttonPath, path, 63);
    strncpy(iconPath, path, 63);
    strncat(buttonPath, "images/button/", 16);
    strncat(iconPath, "images/icons/", 15);

    font.colour.r = 255;
    font.colour.g = 255;
    font.colour.b = 255;

    SDL_GetMouseState(&mouseX, &mouseY);

    if(isFlashbanged)
    {
        strncat(buttonPath, "flashbang/", 11);
        font.colour.r = 0;
        font.colour.g = 0;
        font.colour.b = 0;
    }
    else
    {
        strncat(buttonPath, "dark/", 6);
        font.colour.r = 255;
        font.colour.g = 255;
        font.colour.b = 255;
    }

    // Process light button

    searchDirectory.posX = windowX-42;
    searchDirectory.posY = windowY-(windowY-10);
    searchDirectory.sizeX = 16;
    searchDirectory.sizeY = 32;
    strncpy(searchDirectory.path, buttonPath, 64);
    strncpy(searchDirectory.iconPath, buttonPath, 64);
    sprintf(searchDirectory.content, "...");

    // Process file directory

    fileViewer.posX = 10;
    fileViewer.posY = windowY-(windowY-10);
    fileViewer.sizeX = 560;
    fileViewer.sizeY = 32;
    strncpy(fileViewer.path, buttonPath, 64);
    strncpy(fileViewer.iconPath, buttonPath, 64);
    sprintf(fileViewer.mode, "%sround-idle.bmp", fileViewer.path);

    // Default font variable

    font.font = TTF_OpenFont("/usr/share/fonts/cantarell/Cantarell-VF.otf", 64);
    font.sizeX = 512;
    font.sizeY = 17;

    // Process mouse interaction

    if(mouseX > searchDirectory.posX && mouseX < searchDirectory.posX+searchDirectory.sizeX && mouseY > searchDirectory.posY && mouseY < searchDirectory.sizeY+10)
    {
        sprintf(searchDirectory.mode, "%sround-hover.bmp", searchDirectory.path);

        if(isClicked)
        {
            // if(isFlashbanged)
            //     isFlashbanged = SDL_FALSE;
            // else
            //     isFlashbanged = SDL_TRUE;
            FILE *fp = popen("zenity --file-selection", "r");
            fgets(fileViewer.content, 1024, fp);
            pclose(fp);
            sprintf(searchDirectory.mode, "%sround-click.bmp", searchDirectory.path);
        }
    }
    else
        sprintf(searchDirectory.mode, "%sround-idle.bmp", searchDirectory.path); 
    
    // Deploy the modules

    drawButton(renderer, searchDirectory.surface, searchDirectory.texture, searchDirectory.iconSurface, searchDirectory.iconTexture, searchDirectory.iconPath, searchDirectory.mode, searchDirectory.posX, searchDirectory.posY, searchDirectory.sizeX, searchDirectory.sizeY);
    drawText(renderer, font.fontSurface, font.fontTexture, font.font, searchDirectory.content, font.colour, searchDirectory.posX, searchDirectory.posY, 15, font.sizeY);
    drawButton(renderer, fileViewer.surface, fileViewer.texture, fileViewer.iconSurface, fileViewer.iconTexture, fileViewer.iconPath, fileViewer.mode, fileViewer.posX, fileViewer.posY, fileViewer.sizeX, fileViewer.sizeY);
    drawText(renderer, font.fontSurface, font.fontTexture, font.font, fileViewer.content, font.colour, fileViewer.posX, fileViewer.posY, font.sizeX, font.sizeY);

    return 0;
}

// Process the UI button module. Variables marked as NULL will be ignored. 
int drawButton(SDL_Renderer *renderer,
               SDL_Surface *buttonSurface,
               SDL_Texture *buttonTexture,
               SDL_Surface *iconSurface,
               SDL_Texture *iconTexture,
               char iconPath[],
               char imagePath[],
               int positionX,
               int positionY,
               int sizeX,
               int sizeY)
{
    SDL_Rect buttonRect = {positionX, positionY, 16, sizeY};
    SDL_Rect buttonRectFlip = {positionX+sizeX, positionY, 16, sizeY};

    int i;

    char extendedImagePath[256];
    strncpy(extendedImagePath, imagePath, 96);
    int extendedImagePathLenght = strlen(extendedImagePath);
    strcpy(&extendedImagePath[extendedImagePathLenght-4], "-ext.bmp");

    SDL_Surface *extendedButtonSurface = SDL_LoadBMP(extendedImagePath);
    SDL_Texture *extendedButtonTexture = SDL_CreateTextureFromSurface(renderer, extendedButtonSurface);

    float rotation = 0.0f;

    buttonSurface = SDL_LoadBMP(imagePath);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

    SDL_FreeSurface(buttonSurface);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);
    if(sizeX > 16)
    {
        int count = sizeX/16;
        for(i=0;i<count;i++)
        {
            SDL_Rect extendedButtonRect;
            switch(i)
            {
                case 0:
                {
                    extendedButtonRect.x = positionX+16;
                    extendedButtonRect.y = positionY;
                    extendedButtonRect.w = 16;
                    extendedButtonRect.h = sizeY;
                    break;
                }
                default:
                {
                    extendedButtonRect.x = positionX+16*i;
                    extendedButtonRect.y = positionY;
                    extendedButtonRect.w = 16;
                    extendedButtonRect.h = sizeY;
                    break;
                }
            }
            SDL_RenderCopy(renderer, extendedButtonTexture, NULL, &extendedButtonRect);
        }
    }
    SDL_RenderCopyEx(renderer, buttonTexture, NULL, &buttonRectFlip, rotation, NULL, SDL_FLIP_HORIZONTAL);

    if(!running)
        SDL_DestroyTexture(buttonTexture);

    return 0;
}

// Process on-screen text drawing.
int drawText(SDL_Renderer *renderer,
             SDL_Surface *fontSurface,
             SDL_Texture *fontTexture,
             TTF_Font *font,
             char content[],
             SDL_Color colour,
             int positionX,
             int positionY,
             int sizeX,
             int sizeY)
{
    SDL_Rect fontRect = {positionX+10, positionY+7, sizeX, sizeY};

    fontSurface = TTF_RenderText_Solid(font, content, colour);
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

    SDL_FreeSurface(fontSurface);
    SDL_RenderCopy(renderer, fontTexture, NULL, &fontRect);

    if(!running)
    {
        TTF_Quit();
        SDL_DestroyTexture(fontTexture);
    }

    return 0;
}