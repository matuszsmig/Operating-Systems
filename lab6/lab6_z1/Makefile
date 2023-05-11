CC = gcc -Wall

all:
	$(CC) -c main.c
	$(CC) main.c -o server -L.
	$(CC) -c client.c
	$(CC) client.c -o client -L.

clean:
	rm -f *.o
	rm -f server
	rm -f client