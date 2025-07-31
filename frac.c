#include"frac.h"

struct Fraction* Fraction_constructor() {
    struct Myint* numerator = Myint_constructor();
    struct Myint* denominator = Myint_constructor();
    struct Fraction* a = malloc(sizeof(struct Fraction));
    a -> numerator = numerator;
    a -> denominator = denominator;
    return a;
}

struct Fraction* Fraction_destructor(struct Fraction* a) {
    a -> numerator = Myint_destructor(a -> numerator);
    a -> denominator = Myint_destructor(a -> denominator);
    free(a); a = NULL;
    return a;
}

int main() {
    printf("frac\n");
}