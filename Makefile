CFLAGS= -o cracker -Wall -Werror -std=c99 -pthread -g
TARGETS = cracker *.o

cracker : cracker.c cracker.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h
	gcc -c cracker.c -Wall -Werror -std=c99 -g
	gcc -c src/sha256.c
	gcc -c src/reverse.c
	gcc -o cracker sha256.o reverse.o cracker.o -pthread

#tests : cracker.c cracker.h src/reverse.c src/reverse.h src/sha256.c src/sha256.h test_compare.c
#	gcc -c test_compare.c -Wall -Werror -std=c99 -g
#	gcc -o test_compare cracker.o sha256.o reverse.o test_compare.o -pthread
#	./test_compare test_input/test_compare_src.txt



clean :
	$(RM) $(TARGETS)