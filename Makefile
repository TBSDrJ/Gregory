default:
	gcc main.c -o main.out -L. -lpolynomials
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

all:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.so
	gcc -c -fPIC queue_2.c -o queue_2.o
	gcc -shared queue_2.o -L. -lpolynomials -o libqueue_2.so
	gcc main.c -o main.out -L. -lpolynomials -lqueue_2
	./main.out

clean:
	rm polynomials.o
	rm libpolynomials.so
	rm queue_2.o
	rm libqueue_2.so
	rm main.out
	