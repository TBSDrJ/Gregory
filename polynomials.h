#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

struct Polynomial {
    long degree;
    // Will point to an array of length n+1
    long* coeffs;
};

struct Polynomial* Polynomial_constructor(long deg);
void Polynomial_destructor(struct Polynomial* p);

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
struct Polynomial* Polynomial_reduce(struct Polynomial* p);
struct Polynomial* Polynomial_neg(struct Polynomial* p);
struct Polynomial* Polynomial_der(struct Polynomial* p);
long Polynomial_subs(struct Polynomial* p, long x);
struct Polynomial* Polynomial_add(
        struct Polynomial* p, struct Polynomial* q);
struct Polynomial* Polynomial_multiply(
        struct Polynomial* p, struct Polynomial* q);

#endif