CC = gcc -Wall

all:
	$(CC) -c main.c
	$(CC) main.c -o pipes -L.

test:
	./pipes 0.000001 100 > raport.txt
	./pipes 0.0000001 1000 >> raport.txt
	./pipes 0.0000002 1000 >> raport.txt
	./pipes 0.0000005 500 >> raport.txt

clean:
	rm -f *.o
	rm -f pipes