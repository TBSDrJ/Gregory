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
    struct Myint* zero = Myint_constructor();
    zero -> my_long = 0;
    if (Myint_equal(zero, a -> denominator)) {
        printf("Fraction_contract fails, a -> denominator equals zero.\n");
    }
    zero = Myint_destructor(zero);
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
    struct Myint* tmp = NULL;
    one -> my_long = 1;
    if (Myint_gt(gcd, one)) {
        tmp = a -> numerator;
        a -> numerator = Myint_divide(a -> numerator, gcd);
        Myint_destructor(tmp);
        tmp = a -> denominator;
        a -> denominator = Myint_divide(a -> denominator, gcd);
        Myint_destructor(tmp);
    }
    one = Myint_destructor(one);
    gcd = Myint_destructor(gcd);
}

void Fraction_common_denom(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_common_denom, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_common_denom, b\n");
        fail = true;
    }
    if (fail) {return;}
    struct Myint* common_denom = Myint_lcm(a -> denominator, b -> denominator);
    struct Myint* num_a_factor = Myint_divide(common_denom, a -> denominator);
    struct Myint* num_b_factor = Myint_divide(common_denom, b -> denominator);
    struct Myint* tmp = a -> numerator;
    a -> numerator = Myint_multiply(a -> numerator, num_a_factor);
    tmp = Myint_destructor(tmp);
    num_a_factor = Myint_destructor(num_a_factor);
    tmp = b -> numerator;
    b -> numerator = Myint_multiply(b -> numerator, num_b_factor);
    tmp = Myint_destructor(tmp);
    num_b_factor = Myint_destructor(num_b_factor);
    a -> denominator = Myint_destructor(a -> denominator);
    b -> denominator = Myint_destructor(b -> denominator);
    a -> denominator = common_denom;
    b -> denominator = Myint_deepcopy(common_denom);
}

struct Fraction* Fraction_add(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_add, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_add, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    c -> numerator = Myint_add(a -> numerator, b -> numerator);
    c -> denominator = Myint_deepcopy(a -> denominator);
    Fraction_reduce(a);
    Fraction_reduce(b);
    Fraction_reduce(c);
    return c;
}

struct Fraction* Fraction_subtract(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_subtract, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_subtract, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    c -> numerator = Myint_subtract(a -> numerator, b -> numerator);
    c -> denominator = Myint_deepcopy(a -> denominator);
    Fraction_reduce(a);
    Fraction_reduce(b);
    Fraction_reduce(c);
    return c;
}
int main() {
    struct Fraction* a = Fraction_constructor();
    struct Fraction* b = Fraction_constructor();
    struct Fraction* c = NULL;
    struct Fraction* tmp = NULL;
    a -> numerator -> my_long = 1;
    a -> denominator -> my_long = 1;
    b -> numerator -> my_long = 0;
    b -> denominator -> my_long = 1;
    for (long i=2; i<100; i++) {
        tmp = b;
        b = Fraction_add(a, b);
        tmp = Fraction_destructor(tmp);
        a -> denominator -> my_long = i;
        a -> numerator -> sign *= -1;
    }
    Fraction_print(b); printf("\n");
    a = Fraction_destructor(a);
    b = Fraction_destructor(b);
    a = Fraction_constructor();
    b = Fraction_constructor();
    a -> numerator -> my_long = 1;
    a -> denominator -> my_long = 1;
    b -> numerator -> my_long = 0;
    b -> denominator -> my_long = 1;
    for (long i=1; i<50; i++) {
        tmp = b;
        b = Fraction_add(b, a);
        tmp = Fraction_destructor(tmp);
        a -> denominator -> my_long = 2*i;
        tmp = b;
        b = Fraction_subtract(b, a);
        tmp = Fraction_destructor(tmp);
        a -> denominator -> my_long = 2*i+1;
    }
    Fraction_print(b); printf("\n");
    a = Fraction_destructor(a);
    b = Fraction_destructor(b);
}