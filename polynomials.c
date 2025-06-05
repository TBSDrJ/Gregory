#include<stdlib.h>
#include<stdio.h>

struct polynomial {
    long degree;
    // Will point to an array of length n+1
    long* coeffs;
};

struct polynomial* derivative(struct polynomial* p) {
    struct polynomial* d = malloc(sizeof(long) + sizeof(size_t));
    d -> degree = p -> degree - 1;
    d -> coeffs = malloc(((p -> degree) - 1)*sizeof(long));
    for (long i=0; i<(p -> degree); i++) {
        (d -> coeffs)[i] = (p -> coeffs)[i+1] * (i+1);
    }
    return d;
}

int main() {
    struct polynomial p;
    // Derivative of 2x^3 + 3x^2 + 11x + 1 is 6x^2 + 6x + 11
    p.degree = 3;
    long* coeffs = (long*) malloc((p.degree + 1)*sizeof(long));
    coeffs[0] = 1;
    coeffs[1] = 11;
    coeffs[2] = 3;
    coeffs[3] = 2;
    p.coeffs = coeffs;
    struct polynomial* d = derivative(&p);
    printf("degree d: %li\n", d -> degree);
    printf("d(x) = %lix^2 + %lix + %li\n", d -> coeffs[2], d -> coeffs[1], d-> coeffs[0]);
}