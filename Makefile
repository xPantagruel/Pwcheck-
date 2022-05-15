#Project 1 - IZP - Password strength verification - Makefile
#Author: Matej Macek, FIT
#make - compiles all necessary
GCC = gcc -std=c99 -Wall -Wextra -Werror

all: pwcheck.c
	$(GCC) pwcheck.c -o pwcheck

zip:
	zip xmacek27.zip *.c *.h Makefile

clean:
	rm -f *.o pwcheck
