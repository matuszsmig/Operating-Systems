CC = gcc -Wall

all:
	$(CC) -c exec_file.c
	$(CC) -c main.c
	$(CC) exec_file.c -o exec_file -L.
	$(CC) main.c -o signals -L.

clean:
	rm -f *.o
	rm -f signals exec_file