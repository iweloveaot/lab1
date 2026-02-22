CC=gcc
CFLAGS=-c -Wall -g -fprofile-arcs -ftest-coverage
LDFLAGS=-lgcov

all: main

main: main.o 3d_vector.o complexType.o doubleType.o tests.o
	$(CC) main.o 3d_vector.o complexType.o doubleType.o tests.o -o main $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

3d_vector.o: 3d_vector.c
	$(CC) $(CFLAGS) 3d_vector.c

complexType.o: complexType.c
	$(CC) $(CFLAGS) complexType.c

doubleType.o: doubleType.c
	$(CC) $(CFLAGS) doubleType.c

tests.o: tests.c
	$(CC) $(CFLAGS) tests.c

clean:
	rm -rf *.o main



