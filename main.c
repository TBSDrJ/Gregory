#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"polynomial.h"
#include"myint.h"

#define n 4

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
    a = Polynomial_constructor(n);
    struct Myint* myint_tmp = NULL;
    struct Myint* myint_tmp_2 = NULL;
    struct Myint* zero = Myint_constructor();
    struct Myint* one = Myint_constructor(); one -> my_long = 1;
    a -> coeffs[n] = one;
    for (int i=0; i<n; i++) {a -> coeffs[i] = zero;}
    b = Polynomial_constructor(0);
    b -> coeffs[0] = one;
    c = Polynomial_constructor(0);
    c -> coeffs[0] = one;
    d = Polynomial_constructor(n);
    d -> coeffs[n] = one;
    for (int i=0; i<n; i++) {d -> coeffs[i] = zero;}
    x_plus_1 = Polynomial_constructor(1);
    x_plus_1 -> coeffs[0] = one;
    x_plus_1 -> coeffs[1] = one;
    // Calculate first derivative
    da_0 = Polynomial_add(Polynomial_multiply(Polynomial_der(a), c), 
                Polynomial_neg(Polynomial_multiply(a, Polynomial_der(c))));
    db_0 = b;
    dc_0 = Polynomial_multiply(c, c);
    dd_0 = d;
    da_1 = a;
    db_1 = Polynomial_add(Polynomial_multiply(Polynomial_der(b), d), 
                Polynomial_neg(Polynomial_multiply(b, Polynomial_der(d))));
    dc_1 = Polynomial_multiply(x_plus_1, c);
    dd_1 = Polynomial_multiply(d, d);
    printf("da_0: "); Polynomial_print(da_0); printf("\n");
    printf("db_0: "); Polynomial_print(db_0); printf("\n");
    printf("dc_0: "); Polynomial_print(dc_0); printf("\n");
    printf("dd_0: "); Polynomial_print(dd_0); printf("\n");
    printf("da_1: "); Polynomial_print(da_1); printf("\n");
    printf("db_1: "); Polynomial_print(db_1); printf("\n");
    printf("dc_1: "); Polynomial_print(dc_1); printf("\n");
    printf("dd_1: "); Polynomial_print(dd_1); printf("\n");
    printf("term0: ");
    myint_tmp = Myint_multiply(Polynomial_subs(da_0, 0), 
            Polynomial_subs(db_0, 0));
    myint_tmp_2 = Myint_multiply(Polynomial_subs(dc_0, 0), 
            Polynomial_subs(dd_0, 0));
    Myint_print(myint_tmp); printf(" / "); 
    Myint_print(myint_tmp_2); printf("\n");
    printf("term1: ");
    myint_tmp = Myint_multiply(Polynomial_subs(da_1, 0), 
            Polynomial_subs(db_1, 0));
    myint_tmp_2 =  Myint_multiply(Polynomial_subs(dc_1, 0), 
            Polynomial_subs(dd_1, 0));
    Myint_print(myint_tmp); printf(" / "); 
    Myint_print(myint_tmp_2); printf("\n");
}