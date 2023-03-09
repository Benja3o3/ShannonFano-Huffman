CC=g++
CFLAGS=-Wall -O0 -std=c++14 -O3
BINS=main vectorShannon.o hash.o arbolHuffman.o
all: clean main

main: vectorShannon.o hash.o arbolHuffman.o
	$(CC) $(CFLAGS) -o main main.cpp vectorShannon.o hash.o arbolHuffman.o

vectorShannon.o: hash.o vectorShannon.cpp
	$(CC) $(CFLAGS) -c vectorShannon.cpp

hash.o: hash.cpp
	$(CC) $(CFLAGS) -c hash.cpp

arbolHuffman.o: arbolHuffman.cpp
	$(CC) $(CFLAGS) -c arbolHuffman.cpp

clean:
	@echo " [CLN] Removing binary files"
	@rm -f $(BINS)
