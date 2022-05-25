CC = gcc
CFLAGS = --std=c11 -g -Wall
INCLUDES = -I headers \
	-I vendor/SDL/include \
	-I vendor/SDL_ttf
LIBRARIES = -lSDL2 -lSDL2main -lSDL2_ttf
SOURCES = main.c
COBJS =

SOURCES += src/render/draw.c
SOURCES += src/render/tools.c
SOURCES += src/input/keyboard.c
SOURCES += src/input/mouse.c

PROGRAM_FOLDER = ~/.local/share/PureModManager
CURRENT_USER = `whoami`
all:
	${CC} ${CFLAGS} ${SOURCES} ${INCLUDES} ${LIBRARIES} -o PMM
install:
	mkdir -pv ${PROGRAM_FOLDER}
	cp -rv src/images ${PROGRAM_FOLDER}
	if [ ${CURRENT_USER} == "root" ]; then \
		mv -v PMM /usr/local/bin; \
	fi
clean:
	rm -v PMM