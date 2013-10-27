CC = gcc
CFLAGS = -g -Wall -O2

img: main.o ident.o images.o particles.o tracking.o hashing.o config.h
	$(CC) $(CFLAGS) -Wall tracking.o images.o ident.o particles.o hashing.o main.o -o img `imlib2-config --cflags` `imlib2-config --libs` -lm -lcrypto -lssl

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
