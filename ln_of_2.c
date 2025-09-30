#include<stdio.h>
#include"frac.h"

int main() {
    struct Fraction* a = Fraction_constructor();
    struct Fraction* b = Fraction_constructor();
    struct Fraction* c = NULL;
    struct Fraction* tmp = NULL;
    a -> numerator -> my_long = 1;
    a -> denominator -> my_long = 1;
    b -> numerator -> my_long = 0;
    b -> denominator -> my_long = 1;
    for (long i=2; i<10000; i++) {
        tmp = b;
        b = Fraction_add(a, b);
        tmp = Fraction_destructor(tmp);
        a -> denominator -> my_long = i;
        a -> numerator -> sign *= -1;
    }
    Fraction_print(a); printf("\n");
    Fraction_print(b); printf("\n");
    a = Fraction_destructor(a);
    b = Fraction_destructor(b);
}