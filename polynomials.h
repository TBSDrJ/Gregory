#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

struct Polynomial {
    long degree;
    // Will point to an array of length n+1
    long* coeffs;
};

struct Polynomial* constructor(long deg);
void destructor(struct Polynomial* p);

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

void print_poly(struct Polynomial* p);
struct Polynomial* reduce(struct Polynomial* p);
struct Polynomial* neg(struct Polynomial* p);
struct Polynomial* der(struct Polynomial* p);
long subs(struct Polynomial* p, long x);
struct Polynomial* sum(struct Polynomial* p, struct Polynomial* q);
struct Polynomial* prod(struct Polynomial* p, struct Polynomial* q);

#endif