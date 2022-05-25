#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "tools.h"
#include "draw.h"

char toolsBuffer[128];

// [DBG]: Beep-beep. Boop-boop. Sending log/warning/error message.
int debug(char message[], int warningLevel)
{
    char debugTag[8] = "[DBG]";
    char debugColour[32]= "";

    switch(warningLevel)
    {
        case 1:
            snprintf(debugColour, stringSize("\e[33m"), "\e[33m");
            break;
        case 2:
            snprintf(debugColour, stringSize("\e[31m"), "\e[31m");
            break;
        default:
            break;
    }

    printf("%s%s: %s\e[0m\n", debugColour, debugTag, message);

    return 0;
}

// Re-created C++ stoi() function.
int stoi(char number[])
{
    int i;
    int lenght = stringSize(number)-1;
    int xBuffer;
    int ascii;

    struct size
    {
        int max;
        int result;
    } x;

    x.max = 1;
    x.result = 0;
    
    // Prepare the max size for the math.

    for(i=0;i<lenght;i++)
    {
        x.max *= 10;
    }

    // Convert HEX to ASCII, making the math to discover the integer.

    for(i=0;i<lenght;i++)
    {
        switch(number[i])
        {
            case 48: ascii = 0; break;
            case 49: ascii = 1; break;
            case 50: ascii = 2; break;
            case 51: ascii = 3; break;
            case 52: ascii = 4; break;
            case 53: ascii = 5; break;
            case 54: ascii = 6; break;
            case 55: ascii = 7; break;
            case 56: ascii = 8; break;
            case 57: ascii = 9; break;
        }

        xBuffer = x.max;
        xBuffer *= ascii;
        x.result += xBuffer;
        x.max /= 10;
    }

    x.result /= 100;

    return x.result;
}

// Remove characters from a string.
int removeChar(char string[], int cursor, int count)
{
    int lenght = strlen(string);
    if(lenght < count)
    {
        //debug("Unnecessary counter resized to string lenght.", 1);
        count = lenght;
    }
    int i;

    for(i=0;i<=count;i++)
    {
        strcpy(&string[count-i],"");
    }

    return 0;
}

// Get string size. Strings with value lesser than 1 will return a warning.
int stringSize(char string[])
{
    int lenght = strlen(string)+1;

    switch(lenght)
    {
        case 0:
            debug("Parsed string returned a value less than 1", 1);
            break;
        default:
            return lenght;
    }

    return 1;
}

// Void the content of a string.
int clearString(char string[])
{
    snprintf(string, 2, "\n");

    return 0;
}

// Add the texture path to a object.
int appendTexturePath(char object[], char texture[], char texturePath[])
{
    clearString(toolsBuffer);

    snprintf(toolsBuffer, stringSize(texture)+stringSize(texturePath), "%s", texturePath);
    snprintf(object, stringSize(toolsBuffer)+stringSize(texture), "%s%s", toolsBuffer, texture);

    return 0;
}