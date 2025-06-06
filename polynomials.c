#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct polynomial {
    long degree;
    // Will point to an array of length n+1
    long* coeffs;
};

/*
I stumbled across this, which had some good suggestions about how to make a
constructor of sorts, it makes things a little simpler than what I had:
https://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
*/

struct polynomial* constructor(long deg) {
    struct polynomial* p = malloc(sizeof(struct polynomial));
    if (!p) {
        return NULL;
    }
    // Notice we need 1 *more* than the degree 
    long* coeffs = malloc((deg + 1) * sizeof(long));
    if (!coeffs) {
        return NULL;
    }
    // I'm choosing to initialize also
    for (long i=0; i<=deg; i++) {
        coeffs[i] = 0;
    }
    p -> degree = deg;
    p -> coeffs = coeffs;
    return p;
}

void destructor(struct polynomial* p) {
    if (!p) {
        free(p -> coeffs);
        free(p);
    }
}

/* 
Unary operations:
1. Derivative
2. Negative
3. Print
4. Reduce (if highest deg coeff = 0, reduce degree)

Binary Operations:
1. Sum
2. Product
*/

struct polynomial* reduce(struct polynomial* p) {
    long n = p -> degree;
    while(p -> coeffs[n] == 0) {
        n--;
    }
    if (n < p -> degree) {
        p -> degree = n;
        long* coeffs = malloc((n+1)*sizeof(long));
        memcpy(coeffs, p -> coeffs, (n+1)*sizeof(long));
        free(p -> coeffs);
        p -> coeffs = coeffs;
    }
    return p;
}

struct polynomial* der(struct polynomial* p) {
    struct polynomial* d = constructor((p -> degree) - 1);
    for (long i=0; i<(p -> degree); i++) {
        (d -> coeffs)[i] = (p -> coeffs)[i+1] * (i+1);
    }
    return reduce(d);
}

struct polynomial* neg(struct polynomial* p) {
    struct polynomial* n = constructor(p -> degree);
    for (long i=0; i<((n -> degree) + 1); i++) {
        (n -> coeffs)[i] = -(p -> coeffs)[i];
    }
    return reduce(n);
}

void print_poly(struct polynomial* p) {
    for (long i=(p -> degree); i>1; i--) {
        printf("%lix^%li + ", (p -> coeffs)[i], i);
    }
    printf("%lix + ", (p -> coeffs)[1]);
    printf("%li\n", (p -> coeffs)[0]);
}

struct polynomial* sum(struct polynomial* p, struct polynomial* q) {
    // Make p be the one with greater degree if they are different.
    if ((p -> degree) < (q -> degree)) {
        struct polynomial* temp = p;
        p = q;
        q = temp;
    }
    struct polynomial* s = constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        s -> coeffs[i] = (p -> coeffs[i]) + (q -> coeffs[i]);
    }
    for (long i=(q -> degree) + 1; i<=(p -> degree); i++) {
        s -> coeffs[i] = p -> coeffs[i];
    }
    return reduce(s);
}

struct polynomial* prod(struct polynomial* p, struct polynomial* q) {
    struct polynomial* s = constructor((p -> degree) + (q -> degree));
    for (long i=0; i<=(p -> degree); i++) {
        for (long j=0; j<=(q -> degree); j++) {
            s -> coeffs[i+j] += (p -> coeffs[i]) * (q -> coeffs[j]);
        }
    }
    return reduce(s);
}

int main() {
    struct polynomial* p = constructor(3);
    p -> coeffs[0] = 0;
    p -> coeffs[1] = 4;
    p -> coeffs[2] = -5;
    p -> coeffs[3] = 1;
    struct polynomial* q = constructor(2);
    q -> coeffs[0] = 1;
    q -> coeffs[1] = 1;
    struct polynomial* n = neg(p);
    struct polynomial* d = der(p);
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
    printf("q^n(x) = , n ∈ {1, ..., 16}\n");
    struct polynomial* tmp = q;
    for (long i=0; i<16; i++) {
        tmp = prod(q, tmp);
        print_poly(tmp);
    }
    destructor(p);
    destructor(q);
    destructor(n);
    destructor(d);
    destructor(tmp);
}