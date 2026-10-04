CC = gcc
#CC = gcc -g
#CC = gcc -O3
CFLAGS = -Wall -Wextra

all: czy dzy bitsy
bitsy: bitsy.h bitsy.c
	$(CC) $(CFLAGS) -c bitsy.c
czy: bitsy czy.c
	$(CC) $(CFLAGS) -o czy bitsy.o czy.c
dzy: bitsy dzy.c
	$(CC) $(CFLAGS) -o dzy bitsy.o dzy.c
clean:
	rm czy dzy bitsy.o
submission:
	zip prog3.zip Makefile bitsy.h bitsy.c czy.c dzy.c README
