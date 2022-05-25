#ifndef DRAW_H
#define DRAW_H

int drawProcess(SDL_Renderer *renderer);
int drawBackground(SDL_Renderer *renderer);
int drawUI(SDL_Renderer *renderer);
int drawButton(SDL_Renderer *renderer, SDL_Surface *buttonSurface, SDL_Texture *buttonTexture, SDL_Surface *iconSurface, SDL_Texture *iconTexture, char iconPath[], char imagePath[], int positionX, int positionY, int sizeX, int sizeY);
int drawText(SDL_Renderer *renderer, SDL_Surface *fontSurface, SDL_Texture *fontTexture, TTF_Font *font, char content[], SDL_Color colour, int positionX, int positionY, int sizeX, int sizeY);

#endif