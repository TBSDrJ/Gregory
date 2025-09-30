#ifndef FRAC_H
#define FRAC_H

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"myint.h"

struct Fraction {
    struct Myint* numerator;
    struct Myint* denominator;
};

struct Fraction* Fraction_constructor();
struct Fraction* Fraction_destructor(struct Fraction* a);
bool Fraction_contract(struct Fraction* a);
void Fraction_print(struct Fraction* a);
struct Fraction* Fraction_deepcopy(struct Fraction* a);
void Fraction_reduce(struct Fraction* a);
// This converts a and b in place so that they have a common denominator
void Fraction_common_denom(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_add(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_subtract(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_multiply(struct Fraction* a, struct Fraction* b);
struct Fraction* Fraction_divide(struct Fraction* a, struct Fraction* b);

#endif