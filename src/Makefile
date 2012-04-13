TARGET=listtest
CC=clang
CFLAGS=-I../include
LDFLAGS=
SRCS=list.c ini.c misc.c main.c
OBJS=list.o ini.o misc.o main.o

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${CFLAGS} ${OBJS}

clean:
	rm -f ${TARGET} *.o
