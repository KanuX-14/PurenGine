#define _XOPEN_SOURCE 500 // popen() don't work without this, but cause mouse freeze bug.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "draw.h"
#include "main.h"
#include "tools.h"

struct Background
{
    char path[1024];
    char type[64];
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
};

struct Button
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

struct Font
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

struct Program
{
    char name[128];
    char path[1024];
    char iconPath[1024];
    char runtime[64];
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *iconSurface;
    SDL_Texture *iconTexture;
    int posX;
    int posY;
    int sizeX;
    int sizeY;
};

struct Background background;
struct Background gameListMenu;

struct Button searchDirectory;
struct Button fileViewer;
struct Button fileInstaller;
struct Button lamp;

struct Program game;

struct Font font;

char drawBuffer[128];
char home[64];
char path[64];
char imagePath[128];
char gameListPath[128];
char programListPath[128];

// Main process.
int drawProcess(SDL_Renderer *renderer)
{
    // Load variables

    snprintf(home, stringSize(getenv("HOME")), "%s", getenv("HOME"));
    snprintf(path, stringSize(home)+stringSize("/.local/share/PureModManager/"), "%s/.local/share/PureModManager/", home);
    snprintf(imagePath, stringSize(path)+stringSize("images/"), "%simages/", path);

    // Render UI

    SDL_RenderClear(renderer);
    drawBackground(renderer,
                   background.surface,
                   background.texture,
                   background.rect,
                   background.path,
                   background.posX,
                   background.posY,
                   background.sizeX,
                   background.sizeY);
    drawUI(renderer);
    SDL_RenderPresent(renderer);

    // Avoid memory leak

    SDL_DestroyTexture(background.texture);
    SDL_DestroyTexture(gameListMenu.texture);
    SDL_DestroyTexture(searchDirectory.texture);
    SDL_DestroyTexture(searchDirectory.iconTexture);
    SDL_DestroyTexture(fileViewer.texture);
    SDL_DestroyTexture(fileViewer.iconTexture);
    SDL_DestroyTexture(fileInstaller.texture);
    SDL_DestroyTexture(fileInstaller.iconTexture);
    SDL_DestroyTexture(lamp.texture);
    SDL_DestroyTexture(lamp.iconTexture);
    SDL_DestroyTexture(game.texture);
    SDL_DestroyTexture(game.iconTexture);
    SDL_DestroyTexture(font.texture);

    return 0;
}

// Define the background theme.
int drawBackground(SDL_Renderer *renderer,
                   SDL_Surface *surface,
                   SDL_Texture *texture,
                   SDL_Rect rect,
                   char backgroundPath[],
                   int positionX,
                   int positionY,
                   int sizeX,
                   int sizeY)
{
    if(isFlashbanged)
    {
        appendTexturePath(backgroundPath, "background_flashbang.bmp", imagePath);
        surface = SDL_LoadBMP(backgroundPath);
    }
    else
    {
        appendTexturePath(backgroundPath, "background.bmp", imagePath);
        surface = SDL_LoadBMP(backgroundPath);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    return 0;
}

// Process the UI modules.
int drawUI(SDL_Renderer *renderer)
{
    int mouseX, mouseY;
    char buttonPath[128];
    char iconPath[128];

    int buttonSizeX = 16;
    int buttonSizeY = 32;

    int extendedSizeX = windowX/18;
    int extendedSizeY = windowY/16;

    snprintf(buttonPath, stringSize(imagePath)+stringSize("button/"), "%sbutton/", imagePath);
    snprintf(iconPath, stringSize(imagePath)+stringSize("icons/"), "%sicons/", imagePath);

    font.colour.r = 255;
    font.colour.g = 255;
    font.colour.b = 255;

    SDL_GetMouseState(&mouseX, &mouseY);

    clearString(drawBuffer);
    if(isFlashbanged)
    {
        snprintf(drawBuffer, stringSize("flashbang/")+stringSize(buttonPath), "%sflashbang/", buttonPath);
        font.colour.r = 0;
        font.colour.g = 0;
        font.colour.b = 0;
    }
    else
    {
        snprintf(drawBuffer, stringSize("dark/")+stringSize(buttonPath), "%sdark/", buttonPath);
        font.colour.r = 255;
        font.colour.g = 255;
        font.colour.b = 255;
    }
    snprintf(buttonPath, stringSize(drawBuffer), "%s", drawBuffer);
    clearString(drawBuffer);

    // Process file directory search button

    searchDirectory.posX = windowX-42;
    searchDirectory.posY = windowY-(windowY-10);
    searchDirectory.sizeX = buttonSizeX;
    searchDirectory.sizeY = buttonSizeY;
    snprintf(searchDirectory.path, stringSize(buttonPath), "%s", buttonPath);
    snprintf(searchDirectory.content, 5, "...");
    
    // Process file directory

    fileViewer.posX = 10;
    fileViewer.posY = windowY-(windowY-10);
    fileViewer.sizeX = buttonSizeX*extendedSizeX;
    fileViewer.sizeY = buttonSizeY;
    appendTexturePath(fileViewer.path, "round-idle.bmp", buttonPath);

    // Process lamp directory

    lamp.posX = windowX-42;
    lamp.posY = windowY-42;
    lamp.sizeX = buttonSizeX;
    lamp.sizeY = buttonSizeY;

    // Process file installer

    fileInstaller.posX = lamp.posX-64;
    fileInstaller.posY = lamp.posY;
    fileInstaller.sizeX = buttonSizeX*2;
    fileInstaller.sizeY = buttonSizeY;
    snprintf(fileInstaller.path, stringSize(buttonPath), "%s", buttonPath);
    if(strcmp(fileViewer.content, ""))
        snprintf(fileInstaller.content, stringSize("Install"), "Install");
    
    // Process game list

    gameListMenu.posX = 10;
    gameListMenu.posY = windowY-(windowY-64);
    gameListMenu.sizeX = extendedSizeX*5;
    gameListMenu.sizeY = extendedSizeY*10;

    // TODO: Automated function with input file (probably a text file)
    //       
    //       At the moment, the function is very laggy.
    //       Better output will come in future versions.
    //
    //       A shell math, shall we?
    snprintf(gameListPath, stringSize(path)+stringSize("GAME_LIST"), "%sGAME_LIST", path);
    setProgramList(renderer, gameListPath, game.name, game.path, game.runtime);

    // Default font variable

    font.font = TTF_OpenFont("/usr/share/fonts/cantarell/Cantarell-VF.otf", 64);
    font.sizeY = 27;

    // TODO: Use a switch or something... IF and ELSE will dump this code into the rubbish
    // Process mouse interaction

    clearString(drawBuffer);
    if(mouseX > searchDirectory.posX && mouseX < searchDirectory.posX+(searchDirectory.sizeX*2) && mouseY > searchDirectory.posY && mouseY < searchDirectory.sizeY+10)
    {
        appendTexturePath(searchDirectory.path, "round-hover.bmp", buttonPath);

        if(isClicked)
        {
            FILE *fp = popen("zenity --file-selection", "r");
            if(fgets(drawBuffer, 128, fp) != NULL)
                snprintf(fileViewer.content, stringSize(drawBuffer), "%s", drawBuffer);
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



    if(mouseX > fileInstaller.posX && mouseX < fileInstaller.posX+fileInstaller.sizeX && mouseY > fileInstaller.posY && mouseY < fileInstaller.posY+fileInstaller.sizeY)
    {
        appendTexturePath(fileInstaller.path, "round-hover.bmp", buttonPath);

        if(isClicked)
        {
            char command[1024];
            snprintf(command, stringSize(fileViewer.content), "zip -9r %s %s", fileViewer.content, game.path);
            appendTexturePath(fileInstaller.path, "round-click.bmp", buttonPath);
        }
    }
    else
        appendTexturePath(fileInstaller.path, "round-idle.bmp", buttonPath);
    
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
    
    drawButton(renderer,
               fileInstaller.surface,
               fileInstaller.texture,
               fileInstaller.iconSurface,
               fileInstaller.iconTexture,
               fileInstaller.iconPath,
               fileInstaller.path,
               fileInstaller.posX,
               fileInstaller.posY,
               fileInstaller.sizeX,
               fileInstaller.sizeY);
    
    drawText(renderer,
             font.surface,
             font.texture,
             font.font,
             fileInstaller.content,
             font.colour,
             fileInstaller.posX,
             fileInstaller.posY,
             font.sizeX,
             font.sizeY);

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
    snprintf(extendedImagePath, stringSize(imagePath), "%s", imagePath);
    snprintf(&extendedImagePath[stringSize(extendedImagePath)-5], stringSize("-ext.bmp"), "-ext.bmp");

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

    return 0;
}

// Read GAME_LIST file.
int setProgramList(SDL_Renderer *renderer,
                   char programList[],
                   char name[],
                   char dirPath[],
                   char runtime[])
{
    int i, x = 1, y = 0, count;
    char lineCount[16];
    char line[128];
    char init[3];
    SDL_bool isGame = SDL_FALSE;

    clearString(drawBuffer);
    snprintf(drawBuffer, stringSize("TEMP_VAR=`wc -l <  `;echo $(($TEMP_VAR+1))")+stringSize(programList), "TEMP_VAR=`wc -l < %s`;echo $(($TEMP_VAR+1))", programList);
    FILE *pipe = popen(drawBuffer, "r");
    if(fgets(drawBuffer, 128, pipe))
        snprintf(lineCount, stringSize(drawBuffer), "%s", drawBuffer);
    pclose(pipe);
    clearString(drawBuffer);
    count = stoi(lineCount);

    for(i=0;i<count;i++)
    {
        x = i+1;
        snprintf(drawBuffer, stringSize("cat  | sed -n     p")+stringSize(programList), "cat %s | sed -n %dp", programList, x);
        FILE *pipe = popen(drawBuffer, "r");
        if(fgets(drawBuffer, 128, pipe))
            snprintf(line, stringSize(drawBuffer), "%s", drawBuffer);
        pclose(pipe);
        snprintf(init, 3, "%c", *line);
        if(strcmp(init, ";"))
        {
            if(strcmp(init, "{") && strcmp(init, "}"))
            {
                isGame = SDL_TRUE;
            }
        }
        
        if(isGame)
        {
            y += 1;

            switch(y)
            {
                case 0:
                case 1: snprintf(name, stringSize(line)-1, "%s", line); break;
                case 2: snprintf(dirPath, stringSize(line)-1, "%s", line); break;
                case 3: snprintf(runtime, stringSize(line)-1, "%s", line); break;
                default: isGame = SDL_FALSE; y = 0; break;
            }
        }
    }

    return 0;
}