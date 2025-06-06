default:
	gcc main.c -o main.out -L. -lpolynomials
	./main.out

all:
	gcc -c -fPIC polynomials.c -o polynomials.o
	gcc -shared polynomials.o -o libpolynomials.so
	gcc main.c -o main.out -L. -lpolynomials
	./main.out

clean:
	rm polynomials.o
	rm libpolynomials.so
	rm main.out
	