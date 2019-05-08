CFLAGS= -o cracker2 -Wall -Werror -std=c99 -pthread -g

cracker2 : cracker2.c cracker2.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h
	gcc cracker2.c $(CFLAGS)

