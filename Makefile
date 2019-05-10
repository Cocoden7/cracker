CFLAGS= -o cracker -Wall -Werror -std=c99 -pthread -g
TARGETS = cracker test1 test2 test3 *.o

cracker : src/cracker.c src/cracker.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h
	gcc -c src/cracker.c -Wall -Werror -std=c99 -g
	gcc -c src/sha256.c
	gcc -c src/reverse.c
	gcc -o cracker sha256.o reverse.o cracker.o -pthread

tests : src/cracker.c src/cracker.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h tests/test1.c tests/test2.c tests/test3.c
	gcc -c tests/test2.c -Wall -Werror -std=c99 -g
	gcc -o test2 sha256.o reverse.o cracker.o -pthread
	gcc -c tests/test3.c -Wall -Werror -std=c99 -g
	gcc -o test2 sha256.o reverse.o cracker.o -pthread
	./test2
	./cracker -t 2 tests/02_6c_5.bin
	./cracker -t 2 -c tests/02_6c_5.bin
	./test3
	./cracker -t 3 tests/02_6c_5.bin
	./cracker -t 3 -c tests/02_6c_5.bin

all : src/cracker.c src/cracker.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h tests/test1.c tests/test2.c tests/test3.c
	gcc -c src/cracker.c -Wall -Werror -std=c99 -g
	gcc -c src/sha256.c
	gcc -c src/reverse.c
	gcc -o cracker sha256.o reverse.o cracker.o -pthread
	gcc -c tests/test2.c -Wall -Werror -std=c99 -g
	gcc -o test2 sha256.o reverse.o cracker.o -pthread
	gcc -c tests/test3.c -Wall -Werror -std=c99 -g
	gcc -o test2 sha256.o reverse.o cracker.o -pthread
	./test2
	./cracker -t 2 tests/02_6c_5.bin
	./cracker -t 2 -c tests/02_6c_5.bin
	./test3
	./cracker -t 3 tests/02_6c_5.bin
	./cracker -t 3 -c tests/02_6c_5.bin



clean :
	$(RM) $(TARGETS)