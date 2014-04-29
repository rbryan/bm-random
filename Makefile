CC = /usr/local/bin/gcc -fdiagnostics-color=auto
DEBUG = -g -pg
CFLAGS =  $(DEBUG) -Wall -O3 -fopenmp -ffast-math -funroll-loops -lgomp -L /usr/local/lib64 

img: main.o ident.o images.o particles.o tracking.o hashing.o config.h Makefile
	$(CC) $(CFLAGS) -Wall tracking.o images.o ident.o particles.o hashing.o main.o -o img `imlib2-config --cflags` `imlib2-config --libs` -lm -lcrypto -lssl -lSDL -lpthread

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
images.o: images.c 
	$(CC) $(CFLAGS) -c images.c 
	
particles.o: particles.c
	$(CC) $(CFLAGS) -c particles.c
	
ident.o: ident.c
	$(CC) $(CFLAGS) -c ident.c
	
tracking.o: tracking.c
	$(CC) $(CFLAGS) -c tracking.c

hashing.o: hashing.c
	$(CC) $(CFLAGS) -c hashing.c
	
clean: 
	rm -f *.o img
