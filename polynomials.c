#include<stdlib.h>
#include<stdio.h>

struct polynomial {
    long degree;
    // Will point to an array of length n+1
    long* coeffs;
};

/* 
Unary operations:
1. Derivative
2. Negative
3. Print

Binary Operations:
1. Sum
2. Product
*/
struct polynomial* der(struct polynomial* p) {
    struct polynomial* d = (struct polynomial*) malloc(sizeof(long) + 
            sizeof(size_t));
    d -> degree = p -> degree - 1;
    d -> coeffs = (long*) malloc(((p -> degree) - 1)*sizeof(long));
    for (long i=0; i<(p -> degree); i++) {
        (d -> coeffs)[i] = (p -> coeffs)[i+1] * (i+1);
    }
    return d;
}

struct polynomial* neg(struct polynomial* p) {
    struct polynomial* n = (struct polynomial*) malloc(sizeof(long) + 
            sizeof(size_t));
    n -> degree = p -> degree;
    n -> coeffs = (long*) malloc((p -> degree)*sizeof(long));
    for (long i=0; i<((n -> degree) + 1); i++) {
        (n -> coeffs)[i] = -(p -> coeffs)[i];
    }
    return n;
}

void print(struct polynomial* p) {
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
    struct polynomial* s = (struct polynomial*) malloc(sizeof(long) + 
            sizeof(size_t));
    s -> degree = p -> degree;
    s -> coeffs = (long*) malloc(sizeof(long) * (p -> degree));
    for (long i=0; i<=(q -> degree); i++) {
        s -> coeffs[i] = (p -> coeffs[i]) + (q -> coeffs[i]);
    }
    for (long i=(q -> degree) + 1; i<=(p -> degree); i++) {
        s -> coeffs[i] = p -> coeffs[i];
    }
    return s;
}

int main() {
    struct polynomial* p = (struct polynomial*) malloc(sizeof(long) + 
            sizeof(size_t));
    p -> degree = 3;
    long* coeffs = (long*) malloc(((p -> degree) + 1)*sizeof(long));
    coeffs[0] = 0;
    coeffs[1] = 4;
    coeffs[2] = -5;
    coeffs[3] = 1;
    p -> coeffs = coeffs;
    struct polynomial* n = neg(p);
    struct polynomial* d = der(p);
    printf("p(x) = ");
    print(p);
    printf("-p(x) = ");
    print(n);
    printf("d/dx(p(x)) = ");
    print(d);
    printf("p(x) + p'(x) = ");
    print(sum(p, d));
    free(n -> coeffs);
    free(n);
    free(p -> coeffs);
    free(p);
    free(d -> coeffs);
    free(d);
}