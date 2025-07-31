#ifndef FRAC_H
#define FRAC_H

#include<stdlib.h>
#include<stdio.h>
#include"myint.h"

struct Fraction {
    struct Myint* numerator;
    struct Myint* denominator;
};

struct Fraction* Fraction_constructor();
struct Fraction* Fraction_destructor(struct Fraction* a);
struct Fraction* Fraction_print(struct Fraction* a);
struct Fraction* Fraction_reduce(struct Fraction* a);
struct Fraction* Fraction_add(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_subtract(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_multiply(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_divide(struct Fraction* a, struct Fraction* b);

#endif