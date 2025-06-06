#include<stdio.h>
#include"polynomials.h"

int main() {
    struct Polynomial* p = constructor(3);
    p -> coeffs[0] = 0;
    p -> coeffs[1] = 4;
    p -> coeffs[2] = -5;
    p -> coeffs[3] = 1;
    struct Polynomial* q = constructor(2);
    q -> coeffs[0] = 1;
    q -> coeffs[1] = 1;
    struct Polynomial* n = neg(p);
    struct Polynomial* d = der(p);
    printf("p(x) = ");
    print_poly(p);
    printf("-p(x) = ");
    print_poly(n);
    printf("d/dx(p(x)) = ");
    print_poly(d);
    printf("p(x) + p'(x) = ");
    print_poly(sum(p, d));
    printf("p(x) * p'(x) = ");
    print_poly(prod(p, d));
    printf("q(x) = ");
    print_poly(q);
    printf("q^n(x) = , n ∈ {2, ..., 11}\n");
    struct Polynomial* tmp = q;
    for (long i=0; i<10; i++) {
        tmp = prod(q, tmp);
        print_poly(tmp);
    }
    destructor(p);
    destructor(q);
    destructor(n);
    destructor(d);
    destructor(tmp);
}