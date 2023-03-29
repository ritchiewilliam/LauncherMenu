PROG = main
DEPS = gui.c input#image.c
SRC = ${PROG}.c
OBJ = gui.o input.o main.o#${SRC:.c=.o}

CC = gcc
CFLAGS = `pkg-config --cflags libevdev`
LIBS = -lX11 `pkg-config --libs libevdev`

all: ${PROG}

${PROG}: ${OBJ}
	${CC} -o $@ ${LIBS} ${OBJ}

#${DEPS}: ${OBJ}
#	${CC} -o $@ ${LIBS} ${OBJ}

%.o: %.c
	${CC} -c ${SRC} ${DEPS}.c ${CFLAGS}

clean:
	-rm ${OBJ} ${PROG}

.PHONY: all clean