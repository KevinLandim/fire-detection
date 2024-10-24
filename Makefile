# Makefile para compilar o projeto

CC = gcc
CFLAGS = -pthread -Wall

all: main

main: main.o functions.o
	$(CC) $(CFLAGS) -o main main.o functions.o

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o main
