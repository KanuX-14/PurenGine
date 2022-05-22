CC = gcc
INCLUDES += -I headers
CFLAGS = -g -Wall
COBJ += main.c

all:
	${CC} ${INCLUDES} ${CFLAGS} -o PML ${COBJ}
clean:
	rm -v PML