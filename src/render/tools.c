#include <stdio.h>
#include <string.h>
#include "tools.h"

int debug(char message[], int warningLevel)
{
    char debugTag[8] = "[DBG]";
    char debugColour[32]= "";

    switch(warningLevel)
    {
        case 1:
            strncat(debugColour, "\e[33m", 16);
            break;
        case 2:
            strncat(debugColour, "\e[31m", 16);
            break;
        default:
            break;
    }

    printf("%s%s: %s\e[0m\n", debugColour, debugTag, message);

    return 0;
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

// Add the texture path to a object.
int appendTexturePath(char object[], char texture[], char texturePath[])
{
    char buffer[256];
    int bufferLenght, textureLenght, texturePathLenght;

    textureLenght = strlen(texture);
    texturePathLenght = strlen(texturePath);
    strncpy(buffer, texturePath, textureLenght+texturePathLenght);

    bufferLenght = strlen(buffer);
    snprintf(object, bufferLenght+textureLenght+1, "%s%s", buffer, texture);

    return 0;
}