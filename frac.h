
#ifndef FRAC_H
#define FRAC_H

struct Fraction {
    struct Bigint* numerator;
    struct Bigint* denominator;
};

struct Fraction* construct_frac();

#endif