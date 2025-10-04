#include<stdio.h>
#include"polynomial.h"
#include"myint.h"
#include"queue_2.h"


/********************
I am NOT using the standard mathematical definition of a Rational Function
in this library, because of the problem I am trying to solve.  In general,
I am assuming that I have a function of the form g(x) = [a(x)b(L)]/[c(x)d(L)],
where L = ln(x+1), with a,b,c,d all polynomials with integer coefficients.  
So, you can use these functions on standard rational functions by setting 
b = d = 1 (the constant polynomial of degree zero).
*********************/

// If you have p(x)/q(x) with p(0) = 0 and q(0) = 0, cancel x until 
// either p(0) != 0 or q(0) != 0.
void Rationalfn_cancel_zeros(struct Polynomial* p, struct Polynomial* q);
// This assumes that we have a(x)b(L) / c(x)d(L), where L = ln(x+1), and
//    that we get 0/0 when we Polynomial_substitute x=0.
struct Myint* Rationalfn_l_hôpitals_1 (struct Polynomial* a, struct Polynomial* b, 
        struct Polynomial* c, struct Polynomial* d);