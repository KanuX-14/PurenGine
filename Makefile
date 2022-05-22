CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I headers \
	-I vendor/SDL/include
LIBRARIES = -lSDL2 -lSDL2main
SOURCES = main.c
COBJS =

all:
	${CC} ${CFLAGS} ${SOURCES} ${INCLUDES} ${LIBRARIES} -o PML 
clean:
	rm -v PML