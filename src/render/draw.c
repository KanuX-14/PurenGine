#define _XOPEN_SOURCE 500 // popen() don't work without this, but cause mouse freeze bug.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "draw.h"
#include "main.h"
#include "tools.h"

struct background
{
    char path[1024];
    SDL_Surface *surface;
    SDL_Texture *texture;
};

struct button
{
    char path[1024];
    char iconPath[1024];
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *iconSurface;
    SDL_Texture *iconTexture;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
    char content[1024];
};

struct font
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color colour;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
};

struct background background;

struct button searchDirectory;
struct button fileViewer;
struct button lamp;

struct font font;

char buffer[16];
char path[64];

// Main process.
int drawProcess(SDL_Renderer *renderer)
{
    // Load images

    strncpy(buffer, getenv("HOME"), 16);
    snprintf(path, 63, "%s/.local/share/PureModManager/images/", buffer);

    // Render UI

    SDL_RenderClear(renderer);
    drawBackground(renderer);
    drawUI(renderer);
    SDL_RenderPresent(renderer);

    // Avoid memory leak

    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(searchDirectory.texture);
    SDL_DestroyTexture(searchDirectory.iconTexture);
    SDL_DestroyTexture(fileViewer.texture);
    SDL_DestroyTexture(fileViewer.iconTexture);
    SDL_DestroyTexture(font.texture);

    return 0;
}

// Define the background theme.
int drawBackground(SDL_Renderer *renderer)
{
    if(isFlashbanged)
    {
        appendTexturePath(background.path, "background_flashbang.bmp", path);
        background.surface = SDL_LoadBMP(background.path);
    }
    else
    {
        appendTexturePath(background.path, "background.bmp", path);
        background.surface = SDL_LoadBMP(background.path);
    }
    background.texture = SDL_CreateTextureFromSurface(renderer, background.surface);

    SDL_FreeSurface(background.surface);
    SDL_RenderCopy(renderer, background.texture, NULL, NULL);

    return 0;
}

// Process the UI modules.
int drawUI(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    char buttonPath[128];
    char iconPath[128];
    int extendedSizeX = windowX/18;
    int extendedSizeY = windowY/16;

    strncpy(buttonPath, path, 63);
    strncpy(iconPath, path, 63);
    strncat(buttonPath, "button/", 9);
    strncat(iconPath, "icons/", 7);

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

    // Process file directory search button

    searchDirectory.posX = windowX-42;
    searchDirectory.posY = windowY-(windowY-10);
    searchDirectory.sizeX = 16;
    searchDirectory.sizeY = 32;
    strncpy(searchDirectory.path, buttonPath, 64);
    snprintf(searchDirectory.content, 5, "...");

    // Process file directory

    fileViewer.posX = 10;
    fileViewer.posY = windowY-(windowY-10);
    fileViewer.sizeX = 16*extendedSizeX;
    fileViewer.sizeY = 32;
    appendTexturePath(fileViewer.path, "round-idle.bmp", buttonPath);

    // Process lamp directory

    lamp.posX = windowX-42;
    lamp.posY = windowY-42;
    lamp.sizeX = 16;
    lamp.sizeY = 32;

    // Default font variable

    font.font = TTF_OpenFont("/usr/share/fonts/cantarell/Cantarell-VF.otf", 64);
    font.sizeY = 27;

    // Process mouse interaction

    if(mouseX > searchDirectory.posX && mouseX < searchDirectory.posX+(searchDirectory.sizeX*2) && mouseY > searchDirectory.posY && mouseY < searchDirectory.sizeY+10)
    {
        appendTexturePath(searchDirectory.path, "round-hover.bmp", buttonPath);

        if(isClicked)
        {
            FILE *fp = popen("zenity --file-selection", "r");
            fgets(fileViewer.content, 1024, fp);
            pclose(fp);
            appendTexturePath(searchDirectory.path, "round-click.bmp", buttonPath);
        }
    }
    else
        appendTexturePath(searchDirectory.path, "round-idle.bmp", buttonPath);
    
    if(mouseX > lamp.posX && mouseX < lamp.posX+(lamp.sizeX*2) && mouseY > lamp.posY && mouseY < lamp.posY+lamp.sizeY)
    {
        appendTexturePath(lamp.path, "round-hover.bmp", buttonPath);

        if(isFlashbanged)
            appendTexturePath(lamp.iconPath, "lamp-on.bmp", iconPath);
        else
            appendTexturePath(lamp.iconPath, "lamp-off.bmp", iconPath);
        
        if(isClicked)
        {
            if(isFlashbanged)
            {
                isFlashbanged = SDL_FALSE;
            }
            else
                isFlashbanged = SDL_TRUE;
        }
    }
    else
    {
        if(isFlashbanged)
            appendTexturePath(lamp.iconPath, "lamp-on.bmp", iconPath);
        else
            appendTexturePath(lamp.iconPath, "lamp-off.bmp", iconPath);
        
        appendTexturePath(lamp.path, "round-idle.bmp", buttonPath);
    }
    
    font.sizeX = (strlen(fileViewer.content)*10);
    // Deploy the modules

    drawButton(renderer,
               searchDirectory.surface,
               searchDirectory.texture,
               searchDirectory.iconSurface,
               searchDirectory.iconTexture,
               searchDirectory.iconPath,
               searchDirectory.path,
               searchDirectory.posX,
               searchDirectory.posY,
               searchDirectory.sizeX,
               searchDirectory.sizeY);
    
    drawText(renderer,
             font.surface,
             font.texture,
             font.font,
             searchDirectory.content,
             font.colour,
             searchDirectory.posX,
             searchDirectory.posY,
             15,
             font.sizeY);
    
    drawButton(renderer,
               fileViewer.surface,
               fileViewer.texture,
               fileViewer.iconSurface,
               fileViewer.iconTexture,
               fileViewer.iconPath,
               fileViewer.path,
               fileViewer.posX,
               fileViewer.posY,
               fileViewer.sizeX,
               fileViewer.sizeY);
    
    drawText(renderer,
             font.surface,
             font.texture,
             font.font,
             fileViewer.content,
             font.colour,
             fileViewer.posX,
             fileViewer.posY,
             font.sizeX,
             font.sizeY);
    
    drawButton(renderer,
               lamp.surface,
               lamp.texture,
               lamp.iconSurface,
               lamp.iconTexture,
               lamp.iconPath,
               lamp.path,
               lamp.posX,
               lamp.posY,
               lamp.sizeX,
               lamp.sizeY);

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

    if(iconPath != NULL)
    {
        SDL_Rect iconRect = {positionX+2, positionY+2, 28, sizeY-4};

        iconSurface = SDL_LoadBMP(iconPath);
        iconTexture = SDL_CreateTextureFromSurface(renderer, iconSurface);

        SDL_FreeSurface(iconSurface);
        SDL_RenderCopy(renderer, iconTexture, NULL, &iconRect);
    }

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
    SDL_Rect fontRect = {positionX+8, positionY, sizeX, sizeY};

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