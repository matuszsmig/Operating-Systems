CC = gcc -Wall

all:
	$(CC) -c exec_file.c
	$(CC) exec_file.c -o exec_f -L.
	$(CC) -c main.c
	$(CC) main.c -o pipes -L.

test:
	./pipes 0.000001 100 > raport.txt
	./pipes 0.000002 200 >> raport.txt
	./pipes 0.000002 100 >> raport.txt
	./pipes 0.000005 250 >> raport.txt

clean:
	rm -f *.o
	rm -f pipes exec_f