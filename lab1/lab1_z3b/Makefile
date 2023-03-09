CC = gcc -Wall

all:
	make static
	make shared
	make dynamic

o:
	make all FLAG="-O"

static:
	make clean
	$(CC) $(FLAG) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) $(FLAG) main.c -o main -L. -lmylib

shared:
	make clean
	$(CC) $(FLAG) -fPIC -c mylib.c
	$(CC) $(FLAG) -shared -fPIC -o libmylib.so mylib.o
	$(CC) $(FLAG) main.c -o main -L. -lmylib -Wl,-rpath=.

dynamic:
	make clean
	$(CC) $(FLAG) -c mylib.c
	$(CC) $(FLAG) -shared -fPIC -o libmylib.so mylib.o
	$(CC) $(FLAG) main.c -o main -L. -lmylib -ldl -D DYNAMIC

test_static:
	make static
	./main < tests > results_static.txt

test_shared:
	make shared
	./main < tests > results_shared.txt

test_dynamic:
	make dynamic
	./main < tests > results_dynamic.txt

run_test:
	make test_static
	make test_shared
	make test_dynamic

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so*
	rm -f main
	make clean_results

clean_results:
	rm -f results_*