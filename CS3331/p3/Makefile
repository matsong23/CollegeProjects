CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -std=c11 -D_GNU_SOURCE -fno-pie
OBJ_FILE = thread-main.o thread.o 
EXE_FILE = prog3 
LDFLAGS += -no-pie
LDLIBS += -lpthread -ldl

${EXE_FILE}: ${OBJ_FILE} thread.h
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${LDFLAGS} ${LDLIBS}

thread-main.o: thread-main.c
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.c

thread.o: thread.c
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.c

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
