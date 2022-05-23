#include <stdio.h>
#include "SDL2/SDL.h"
#include "keyboard.h"

int keyboardCursor(int key)
{
    switch(key)
    {
        case SDL_SCANCODE_ESCAPE:
            return 1;
        case SDL_SCANCODE_TAB:
            return 2;
        case SDL_SCANCODE_LSHIFT:
            return 3;
        default:
            break;
    }

    return 0;
}