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

bool Fraction_contract(struct Fraction* a) {
    bool fail = false;
    if (!(a -> numerator)) {
        printf("Fraction_contract fails, a -> numerator is NULL.\n");
        fail = true;
    }
    if (!(a -> denominator)) {
        printf("Fraction_contract fails, a -> denomintor is NULL.\n");
        fail = true;
    }
    if (!(Myint_contract(a -> numerator))) {
        printf("Fraction_contract fails, a -> numerator, Myint_contract.\n");
        fail = true;
    }
    if (!(Myint_contract(a -> denominator))) {
        printf("Fraction_contract fails, a -> denominator, Myint_contract.\n");
        fail = true;
    }
    if (fail) {return false;}
    return true;
}

void Fraction_print(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_print\n");
        return;
    }
    Myint_print(a -> numerator); printf(" / "); Myint_print(a -> denominator);
}

void Fraction_reduce(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_reduce\n");
        return;
    }
    struct Myint* gcd = Myint_gcd(a -> numerator, a -> denominator);
    struct Myint* one = Myint_constructor();
    one -> my_long = 1;
    if (Myint_gt(gcd, one)) {
        struct Myint** divmod_num = malloc(2*sizeof(struct Myint*));
        divmod_num = Myint_divmod(a -> numerator, gcd);
        struct Myint** divmod_den = Myint_divmod(a -> denominator, gcd);
        divmod_num[1] = Myint_destructor(divmod_num[1]);
        divmod_den[1] = Myint_destructor(divmod_den[1]);
        Myint_destructor(a -> numerator);
        Myint_destructor(a -> denominator);
        a -> numerator = divmod_num[0];
        a -> denominator = divmod_den[0];
    }
    one = Myint_destructor(one);
    gcd = Myint_destructor(gcd);
}

int main() {
    struct Fraction* a = Fraction_constructor();
    a -> numerator -> my_long = 14161676921445484;
    a -> denominator -> my_long = 706049982514030;
    struct Myint* lcm = Myint_lcm(a -> numerator, a -> denominator);
    printf("lcm: "); Myint_print(lcm); printf("\n");
    Fraction_reduce(a);
    printf("a: "); Fraction_print(a); printf("\n");
}