CFLAGS= -o cracker2 -Wall -Werror -std=c99 -pthread -g

cracker2 : cracker2.c cracker2.h reverse.c reverse.h sha256.c sha256.h
	gcc cracker2.c $(CFLAGS)

