CFLAGS= -o cracker -Wall -Werror -std=c99 -pthread -g

cracker : cracker.c cracker.h
  gcc cracker.c $(CFLAGS)
