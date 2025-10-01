default:
	gcc main.c -o main.out -L. -lbigint -lmyint -lfrac -lpolynomials
	./main.out

bigint:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so

bigint-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	./bigint_test.out

myint:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so

myint-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	./myint_test.out
	
frac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so

frac-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc frac_test.c -L. -lmyint -lbigint -o frac_test.out
	./frac_test.out
	
polynomial:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.so

polynomial-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.so
	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out
	./polynomial_test.out

all:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.so
	gcc main.c -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
	./main.out

all-tests:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.so
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	gcc frac_test.c -L. -lmyint -lbigint -o frac_test.out
	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out

clean:
	rm *.o
	rm *.out
	rm *.so

mac:
	gcc main.c -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
	./main.out

bigint-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib

bigint-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	./bigint_test.out

myint-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib

myint-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	./myint_test.out 2> tmp.txt
	
frac-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib

frac-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc frac_test.c -L. -lmyint -lbigint -o frac_test.out
	./frac_test.out
	
polynomial-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib

polynomial-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib
	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out
	./polynomial_test.out

all-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib
	gcc main.c -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
	./main.out

all-tests-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc -c -fPIC polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	gcc frac_test.c -L. -lmyint -lbigint -o frac_test.out
	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out

clean-mac:
	rm *.o
	rm *.out
	rm *.dylib