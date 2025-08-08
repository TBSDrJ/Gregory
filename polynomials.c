#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"polynomials.h"

/*
I stumbled across this, which had some good suggestions about how to make a
constructor of sorts, it makes things a little simpler than what I had:
https://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
Looking back, that probably should have been obvious.
*/

struct Polynomial* Polynomial_constructor(long deg) {
    struct Polynomial* p = malloc(sizeof(struct Polynomial));
    if (deg < 0) {
        deg = 0;
    }
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

void Polynomial_destructor(struct Polynomial* p) {
    if (!p) {
        free(p -> coeffs);
        free(p);
    }
}

bool Polynomial_contract(struct Polynomial* p) {
    bool fail = false;
    if (!p) {
        printf("Polynomial contract fails, p is null.\n");
        fail = true;
    }
    if (p -> degree < 0) {
        printf("Polynomial contract fails, p -> degree is negative.\n");
        fail = true;
    }
    if (!p -> coeffs) {
        printf("Polynomial contract fails, p -> coeffs is null.\n");
        fail = true;
    }
    return !fail;
}

/* 
Unary operations:
1. Print
2. Reduce (if highest deg coeff = 0, Polynomial_reduce degree)
3. Negative
4. Derivative
5. Substitute a value

Binary Operations:
1. Sum
2. Product
*/

void Polynomial_print(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_print.\n");
        return;
    }
    for (long i=(p -> degree); i>1; i--) {
        printf("%lix^%li + ", (p -> coeffs)[i], i);
    }
    printf("%lix + ", (p -> coeffs)[1]);
    printf("%li", (p -> coeffs)[0]);
}

struct Polynomial* Polynomial_reduce(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_reduce.\n");
        return NULL;
    }
    long n = p -> degree;
    while(p -> coeffs[n] == 0) {
        n--;
    }
    if (n < 0) {n = 0;}
    if (n < p -> degree) {
        p -> degree = n;
        long* coeffs = malloc((n+1)*sizeof(long));
        memcpy(coeffs, p -> coeffs, (n+1)*sizeof(long));
        free(p -> coeffs);
        p -> coeffs = coeffs;
    }
    return p;
}

struct Polynomial* Polynomial_neg(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_neg.\n");
        return NULL;
    }
    struct Polynomial* n = Polynomial_constructor(p -> degree);
    for (long i=0; i<((n -> degree) + 1); i++) {
        (n -> coeffs)[i] = -(p -> coeffs)[i];
    }
    return Polynomial_reduce(n);
}

struct Polynomial* Polynomial_der(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_der.\n");
        return NULL;
    }
    struct Polynomial* d = Polynomial_constructor((p -> degree) - 1);
    for (long i=0; i<(p -> degree); i++) {
        (d -> coeffs)[i] = (p -> coeffs)[i+1] * (i+1);
    }
    return Polynomial_reduce(d);
}

long Polynomial_subs(struct Polynomial* p, long x) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_subs.\n");
        return NULL;
    }
    long y = 0, term;
    if (x == 0) {
        return p -> coeffs[0];
    }
    for (long i=0; i <= (p -> degree); i++) {
        term = 1;
        for (long j=0; j<i; j++) {
            term *= x;
        }
        term *= p -> coeffs[i];
        y += term;
    }
    return y;
}

struct Polynomial* Polynomial_add(
        struct Polynomial* p, struct Polynomial* q) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_add.\n");
        return NULL;
    }
    // Make p be the one with greater degree if they are different.
    if ((p -> degree) < (q -> degree)) {
        struct Polynomial* temp = p;
        p = q;
        q = temp;
    }
    struct Polynomial* s = Polynomial_constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        s -> coeffs[i] = (p -> coeffs[i]) + (q -> coeffs[i]);
    }
    for (long i=(q -> degree) + 1; i<=(p -> degree); i++) {
        s -> coeffs[i] = p -> coeffs[i];
    }
    return Polynomial_reduce(s);
}

struct Polynomial* Polynomial_multiply(
            struct Polynomial* p, struct Polynomial* q) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_multiply.\n");
        return NULL;
    }
    struct Polynomial* s = Polynomial_constructor(
                (p -> degree) + (q -> degree));
    for (long i=0; i<=(p -> degree); i++) {
        for (long j=0; j<=(q -> degree); j++) {
            s -> coeffs[i+j] += (p -> coeffs[i]) * (q -> coeffs[j]);
        }
    }
    return Polynomial_reduce(s);
}
