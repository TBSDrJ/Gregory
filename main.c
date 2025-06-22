#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"polynomials.h"

#define n 7

// If you have p(x)/q(x) with p(0) = 0 and q(0) = 0, cancel x until 
// either p(0) != 0 or q(0) != 0.
void cancel(struct Polynomial* p, struct Polynomial* q) {
    long p_0, q_0;
    p_0 = subs(p, 0);
    q_0 = subs(q, 0);
    while ((p_0 == 0) && (q_0 == 0)) {
        // Notice that p,q -> coeffs[0] = 0 if we're here.
        for (int i=0; i<(p -> degree); i++) {
            p -> coeffs[i] = p -> coeffs[i+1];
        }
        p -> coeffs[p -> degree] = 0;
        for (int i=0; i<(q -> degree); i++) {
            q -> coeffs[i] = q -> coeffs[i+1];
        }
        q -> coeffs[q -> degree] = 0;
        p = reduce(p);
        q = reduce(q);
        p_0 = subs(p, 0);
        q_0 = subs(q, 0);
    }
}

int main() {
    // Initialize: a=x^n, b=1, c=1, d=x^n
    // a and c are polynomials in x
    // b and d are polynomials in L = ln(x+1)
    // We start with (a(x)b(L))/(c(x)d(L))
    struct Polynomial* a; 
    struct Polynomial* b;
    struct Polynomial* c;
    struct Polynomial* d; 
    struct Polynomial* da_0; 
    struct Polynomial* db_0; 
    struct Polynomial* dc_0;
    struct Polynomial* dd_0;
    struct Polynomial* da_1; 
    struct Polynomial* db_1; 
    struct Polynomial* dc_1;
    struct Polynomial* dd_1;
    struct Polynomial* x_plus_1;
    a = constructor(n);
    a -> coeffs[n] = 1;
    for (int i=0; i<n; i++) {a -> coeffs[i] = 0;}
    b = constructor(0);
    b -> coeffs[0] = 1;
    c = constructor(0);
    c -> coeffs[0] = 1;
    d = constructor(n);
    d -> coeffs[n] = 1;
    for (int i=0; i<n; i++) {d -> coeffs[i] = 0;}
    x_plus_1 = constructor(1);
    x_plus_1 -> coeffs[0] = 1;
    x_plus_1 -> coeffs[1] = 1;
    // Calculate first derivative
    da_0 = sum(prod(der(a), c), neg(prod(a, der(c))));
    db_0 = b;
    dc_0 = prod(c, c);
    dd_0 = d;
    da_1 = a;
    db_1 = sum(prod(der(b), d), neg(prod(b, der(d))));
    dc_1 = prod(x_plus_1, c);
    dd_1 = prod(d, d);
    cancel(da_0, dc_0);
    cancel(db_0, dd_0);
    cancel(da_1, dc_1);
    cancel(db_1, dd_1);
    printf("da_0: "); print_poly(da_0); printf("\n");
    printf("db_0: "); print_poly(db_0); printf("\n");
    printf("dc_0: "); print_poly(dc_0); printf("\n");
    printf("dd_0: "); print_poly(dd_0); printf("\n");
    printf("da_1: "); print_poly(da_1); printf("\n");
    printf("db_1: "); print_poly(db_1); printf("\n");
    printf("dc_1: "); print_poly(dc_1); printf("\n");
    printf("dd_1: "); print_poly(dd_1); printf("\n");
    printf("term0: %li/%li\n", subs(da_0, 0)*subs(db_0, 0), subs(dc_0, 0)*subs(dd_0, 0));
    printf("term1: %li/%li\n", subs(da_1, 0)*subs(db_1, 0), subs(dc_1, 0)*subs(dd_1, 0));
}