CC = gcc -Wall

all:
	make static
	make shared
	make dynamic

static:
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) main.c -o mainSt -L. -lmylib

shared:
	$(CC) -fPIC -c mylib.c
	$(CC) -shared -fPIC -o libmylib.so mylib.o
	$(CC) main.c -o mainSh -L. -lmylib -Wl,-rpath=.

dynamic:
	$(CC) -c mylib.c
	$(CC) -shared -fPIC -o libmylib.so mylib.o
	$(CC) main.c -o mainD -L. -lmylib -ldl -D DYNAMIC

test_static:
	make static
	./mainSt < tests > results_static.txt

test_shared:
	make shared
	./mainSh < tests > results_shared.txt

test_dynamic:
	make dynamic
	./mainD < tests > results_dynamic.txt

test:
	make test_static
	make test_shared
	make test_dynamic

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so*
	rm -f mainSt
	rm -f mainSh
	rm -f mainD
	rm -f results_*
