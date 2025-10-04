default:
	gcc main.c -O2 -o main.out -L. -lbigint -lmyint -lfrac -lpolynomials
	./main.out

bigint:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so

bigint-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	./bigint_test.out

myint:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so

myint-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	./myint_test.out
	
frac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so

frac-test:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc frac_test.c -L. -lmyint -lbigint -lfrac -o frac_test.out
	./frac_test.out
	
ln-test:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc ln_of_2.c -L. -lbigint -lmyint -lfrac -o ln_of_2.out
	/usr/bin/time -v ./ln_of_2.out

polynomial:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC -O2 polynomial.c -o polynomial.o
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
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.so
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.so
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.so
	gcc -c -fPIC -O2 polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.so
	gcc main.c -O2 -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
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
	gcc frac_test.c -L. -lmyint -lbigint -lfrac -o frac_test.out
	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out
	./bigint_test.out 1> tmp.tests.txt 2> tmp.ptrs.txt
	./myint_test.out 1>> tmp.tests.txt 2>> tmp.ptrs.txt
	./frac_test.out 1>> tmp.tests.txt 2>> tmp.ptrs.txt
	python3 check_for_memory_leaks.py
	cat tmp.tests.txt | grep ERROR

clean:
	rm *.o
	rm *.out
	rm *.so
	rm tmp.*

mac:
	gcc main.c -O2 -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
	./main.out

bigint-mac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib

bigint-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	./bigint_test.out

myint-mac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib

myint-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	./myint_test.out
	
frac-mac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib

frac-test-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc frac_test.c -L. -lmyint -lbigint -lfrac -o frac_test.out
	./frac_test.out
		
ln-test-mac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc ln_of_2.c -L. -lbigint -lmyint -lfrac -o ln_of_2.out
	/usr/bin/time -l ./ln_of_2.out

polynomial-mac:
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC -O2  myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC -O2 polynomial.c -o polynomial.o
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
	gcc -c -fPIC -O2 bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC -O2 myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC -O2 frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
	gcc -c -fPIC -O2 polynomial.c -o polynomial.o
	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib
	gcc main.c -O2 -o main.out -L. -lbigint -lmyint -lfrac -lpolynomial
	./main.out

all-tests-mac:
	gcc -c -fPIC bigint.c -o bigint.o
	gcc -shared bigint.o -o libbigint.dylib
	gcc -c -fPIC myint.c -o myint.o
	gcc -shared myint.o -L. -lbigint -o libmyint.dylib
	gcc -c -fPIC frac.c -o frac.o
	gcc -shared frac.o -L. -lbigint -lmyint -o libfrac.dylib
# 	gcc -c -fPIC polynomial.c -o polynomial.o
# 	gcc -shared polynomial.o -L. -lbigint -lmyint -o libpolynomial.dylib
	gcc bigint_test.c -L. -lbigint -o bigint_test.out
	gcc myint_test.c -L. -lmyint -lbigint -o myint_test.out
	gcc frac_test.c -L. -lmyint -lbigint -lfrac -o frac_test.out
# 	gcc polynomial_test.c  -L. -lbigint -lmyint -lpolynomial -o polynomial_test.out
	./bigint_test.out 1> tmp.tests.txt 2> tmp.ptrs.txt
	./myint_test.out 1>> tmp.tests.txt 2>> tmp.ptrs.txt
	./frac_test.out 1>> tmp.tests.txt 2>> tmp.ptrs.txt
	python3 check_for_memory_leaks.py
	cat tmp.tests.txt | grep ERROR

clean-mac:
	rm *.o
	rm *.out
	rm *.dylib
	rm tmp.*