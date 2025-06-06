#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"polynomials.h"

/*
I stumbled across this, which had some good suggestions about how to make a
constructor of sorts, it makes things a little simpler than what I had:
https://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
*/

struct Polynomial* constructor(long deg) {
    struct Polynomial* p = malloc(sizeof(struct Polynomial));
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

void destructor(struct Polynomial* p) {
    if (!p) {
        free(p -> coeffs);
        free(p);
    }
}

/* 
Unary operations:
1. Print
2. Reduce (if highest deg coeff = 0, reduce degree)
3. Negative
4. Derivative

Binary Operations:
1. Sum
2. Product
*/

void print_poly(struct Polynomial* p) {
    for (long i=(p -> degree); i>1; i--) {
        printf("%lix^%li + ", (p -> coeffs)[i], i);
    }
    printf("%lix + ", (p -> coeffs)[1]);
    printf("%li\n", (p -> coeffs)[0]);
}

struct Polynomial* reduce(struct Polynomial* p) {
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

struct Polynomial* neg(struct Polynomial* p) {
    struct Polynomial* n = constructor(p -> degree);
    for (long i=0; i<((n -> degree) + 1); i++) {
        (n -> coeffs)[i] = -(p -> coeffs)[i];
    }
    return reduce(n);
}

struct Polynomial* der(struct Polynomial* p) {
    struct Polynomial* d = constructor((p -> degree) - 1);
    for (long i=0; i<(p -> degree); i++) {
        (d -> coeffs)[i] = (p -> coeffs)[i+1] * (i+1);
    }
    return reduce(d);
}

struct Polynomial* sum(struct Polynomial* p, struct Polynomial* q) {
    // Make p be the one with greater degree if they are different.
    if ((p -> degree) < (q -> degree)) {
        struct Polynomial* temp = p;
        p = q;
        q = temp;
    }
    struct Polynomial* s = constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        s -> coeffs[i] = (p -> coeffs[i]) + (q -> coeffs[i]);
    }
    for (long i=(q -> degree) + 1; i<=(p -> degree); i++) {
        s -> coeffs[i] = p -> coeffs[i];
    }
    return reduce(s);
}

struct Polynomial* prod(struct Polynomial* p, struct Polynomial* q) {
    struct Polynomial* s = constructor((p -> degree) + (q -> degree));
    for (long i=0; i<=(p -> degree); i++) {
        for (long j=0; j<=(q -> degree); j++) {
            s -> coeffs[i+j] += (p -> coeffs[i]) * (q -> coeffs[j]);
        }
    }
    return reduce(s);
}
