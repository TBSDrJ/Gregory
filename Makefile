default:
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint
	./main.out

poly:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.so
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2
	./main.out

queue:
	gcc -c -fPIC queue_2.c -o queue_2.o
	gcc -shared queue_2.o -L. -lpolynomials -o libqueue_2.so
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2
	./main.out

bigint:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint
	./main.out

all:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.so
	gcc -c -fPIC queue_2.c -o queue_2.o
	gcc -shared queue_2.o -L. -lpolynomials -o libqueue_2.so
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint
	./main.out

clean:
	rm polynomials.o
	rm libpolynomials.so
	rm queue_2.o
	rm libqueue_2.so
	rm bigint.o
	rm libbigint.so
	rm main.out
	
mac:
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint
	./main.out

poly-mac:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.dylib
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2
	./main.out

queue-mac:
	gcc -c -fPIC queue_2.c -o queue_2.o
	gcc -shared queue_2.o -L. -lpolynomials -o libqueue_2.dylib
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2
	./main.out

bigint-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint
	./main.out

myint-mac:
	gcc myint.c -L. -lbigint -o myint.out
	./myint.out

all-mac:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.dylib
	gcc -c -fPIC queue_2.c -o queue_2.o
	gcc -shared queue_2.o -L. -lpolynomials -o libqueue_2.dylib
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2 -lbigint -lmyint
	./main.out

clean-mac:
	rm polynomials.o
	rm libpolynomials.dylib
	rm queue_2.o
	rm libqueue_2.dylib
	rm bigint.o
	rm libbigint.dylib
	rm myint.o
	rm libmyint.dylib
	rm main.out
