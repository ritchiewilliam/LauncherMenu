PROG = main
DEPS = input#image.c
SRC = ${PROG}.c
OBJ = ${SRC:.c=.o}

CC = gcc
CFLAGS = `pkg-config --cflags libevdev`
LIBS = -lX11 -lpng `pkg-config --libs libevdev`

all: ${PROG}

${PROG}: ${OBJ}
	${CC} -o $@ ${LIBS} ${OBJ} ${DEPS}.o

#${DEPS}: ${OBJ}
#	${CC} -o $@ ${LIBS} ${OBJ}

%.o: %.c
	${CC} -c ${SRC} ${DEPS}.c ${CFLAGS}

clean:
	-rm ${OBJ} ${PROG}

.PHONY: all clean