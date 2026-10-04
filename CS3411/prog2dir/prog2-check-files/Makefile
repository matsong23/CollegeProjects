copy: copy.o
	gcc copy.o -o copy

copy.o: copy.c
	gcc copy.c -c -Wall -Wextra -o copy.o

.PHONY: clean
clean: 
	rm *.o copy

.PHONY: zip
zip: 
	zip prog2.zip copy.c Makefile
