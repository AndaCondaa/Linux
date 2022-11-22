CFLAGS=-g -ggdb

test :	input.o print.o main.o
	gcc -o test main.o print.o input.o

main.o: main.c
	gcc $(CFLAGS) -c main.c
print.o: print.c
	gcc $(CFLAGS) -c print.c
input.o: input.c
	gcc $(CFLAGS) -c input.c

clean :
	rm *.o test
