#ifndef TOOLS_H
#define TOOLS_H

int debug(char message[], int warningLevel);
int stoi(char number[]);
int removeChar(char string[], int cursor, int count);
int stringSize(char string[]);
int clearString(char string[]);
int appendTexturePath(char object[], char texture[], char texturePath[]);

#endif