CC = gcc
CFLAGS = --std=c11 -g -Wall
INCLUDES = -I headers \
	-I vendor/SDL/include \
	-I vendor/SDL_ttf
LIBRARIES = -lSDL2 -lSDL2main -lSDL2_ttf
SOURCES = main.c
COBJS =

SOURCES += src/render/draw.c
SOURCES += src/input/keyboard.c
SOURCES += src/input/mouse.c

all:
	${CC} ${CFLAGS} ${SOURCES} ${INCLUDES} ${LIBRARIES} -o PMM
	./PMM
clean:
	rm -v PMM