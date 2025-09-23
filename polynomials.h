#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#include<stdbool.h>

struct Polynomial {
    long degree;
    // Will point to an array of length n+1
    struct Myint** coeffs;
};

struct Polynomial* Polynomial_constructor(long deg);
struct Polynomial* Polynomial_destructor(struct Polynomial* p);
bool Polynomial_contract(struct Polynomial* p);

/* 
Unary operations:
1. Print
2. Reduce (if highest deg coeff = 0, reduce degree)
3. Negative
4. Derivative
5. Substitution

Binary Operations:
1. Sum
2. Product
*/

void Polynomial_print(struct Polynomial* p);
void Polynomial_reduce(struct Polynomial* p);
struct Polynomial* Polynomial_deepcopy(struct Polynomial* p);
struct Polynomial* Polynomial_neg(struct Polynomial* p);
struct Polynomial* Polynomial_der(struct Polynomial* p);
struct Myint* Polynomial_subs(struct Polynomial* p, long x);
struct Polynomial* Polynomial_add(
        struct Polynomial* p, struct Polynomial* q);
struct Polynomial* Polynomial_multiply(
        struct Polynomial* p, struct Polynomial* q);

#endif