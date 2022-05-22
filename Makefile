CC = gcc
CFLAGS = --std=c11 -g -Wall
INCLUDES = -I headers \
	-I vendor/SDL/include
LIBRARIES = -lSDL2 -lSDL2main
SOURCES = main.c
COBJS =

SOURCES += src/render/draw.c

all:
	${CC} ${CFLAGS} ${SOURCES} ${INCLUDES} ${LIBRARIES} -o PMM 
clean:
	rm -v PMM