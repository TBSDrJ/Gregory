#include"frac.h"

#define MEM_LEAK_CHK 1

struct Fraction* Fraction_constructor() {
    struct Myint* numerator = Myint_constructor();
    struct Myint* denominator = Myint_constructor();
    struct Fraction* a = malloc(sizeof(struct Fraction));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Fraction %li\n", (long) a);
    }
    a -> numerator = numerator;
    a -> denominator = denominator;
    return a;
}

struct Fraction* Fraction_destructor(struct Fraction* a) {
    a -> numerator = Myint_destructor(a -> numerator);
    a -> denominator = Myint_destructor(a -> denominator);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Fraction %li\n", (long) a);
    }
    free(a); 
    return NULL;
}

bool Fraction_contract(struct Fraction* a) {
    bool fail = false;
    if (!a) {
        fprintf(stderr, "Fraction_contract fails, a is NULL.\n");
        fail = true;
    }
    else if (!(a -> numerator)) {
        fprintf(stderr, "Fraction_contract fails, a -> numerator is NULL.\n");
        fail = true;
    }
    else if (!(a -> denominator)) {
        fprintf(stderr, "Fraction_contract fails, a -> denominator is NULL.\n");
        fail = true;
    }
    else if (!(Myint_contract(a -> numerator))) {
        fprintf(stderr, 
                "Fraction_contract fails, a -> numerator, Myint_contract.\n");
        fail = true;
    }
    else if (!(Myint_contract(a -> denominator))) {
        fprintf(stderr, 
                "Fraction_contract fails, a -> denominator, Myint_contract.\n");
        fail = true;
    }
    if (fail) {return false;}
    return true;
}

void Fraction_print(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_print\n");
        return;
    }
    if (a) {
        Myint_print(a -> numerator);
        printf(" / "); 
        Myint_print(a -> denominator);
    }
}

struct Fraction* Fraction_deepcopy(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_deepcopy\n");
        return NULL;
    }
    struct Fraction* b = Fraction_from_Myints(
            Myint_deepcopy(a -> numerator), Myint_deepcopy(a -> denominator));
    return b;
}

void Fraction_reduce(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_reduce\n");
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
        fprintf(stderr, 
                "Fraction_contract fails at Fraction_common_denom, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, 
                "Fraction_contract fails at Fraction_common_denom, b\n");
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
        fprintf(stderr, "Fraction_contract fails at Fraction_add, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_add, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    struct Fraction* tmp_a = Fraction_deepcopy(a);
    struct Fraction* tmp_b = Fraction_deepcopy(b);
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    c -> numerator = Myint_add(a -> numerator, b -> numerator);
    c -> denominator = Myint_deepcopy(a -> denominator);
    struct Myint* tmp_num = a -> numerator;
    struct Myint* tmp_den = a -> denominator;
    tmp_num = Myint_destructor(tmp_num);
    tmp_den = Myint_destructor(tmp_den);
    memcpy(a, tmp_a, sizeof(struct Fraction));
    tmp_num = b -> numerator;
    tmp_den = b -> denominator;
    tmp_num = Myint_destructor(tmp_num);
    tmp_den = Myint_destructor(tmp_den);
    memcpy(b, tmp_b, sizeof(struct Fraction));
    Fraction_reduce(c);
    return c;
}

struct Fraction* Fraction_subtract(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_subtract, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_subtract, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    struct Fraction* tmp_a = Fraction_deepcopy(a);
    struct Fraction* tmp_b = Fraction_deepcopy(b);
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    c -> numerator = Myint_subtract(a -> numerator, b -> numerator);
    c -> denominator = Myint_deepcopy(a -> denominator);
    struct Myint* tmp_num = a -> numerator;
    struct Myint* tmp_den = a -> denominator;
    tmp_num = Myint_destructor(tmp_num);
    tmp_den = Myint_destructor(tmp_den);
    memcpy(a, tmp_a, sizeof(struct Fraction));
    tmp_num = b -> numerator;
    tmp_den = b -> denominator;
    tmp_num = Myint_destructor(tmp_num);
    tmp_den = Myint_destructor(tmp_den);
    memcpy(b, tmp_b, sizeof(struct Fraction));
    Fraction_reduce(c);
    return c;
}

struct Fraction* Fraction_multiply(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_multiply, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_multiply, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    c -> numerator = Myint_multiply(a -> numerator, b -> numerator);
    c -> denominator = Myint_multiply(a -> denominator, b -> denominator);
    return c;
}

struct Fraction* Fraction_divide(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_multiply, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_multiply, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    c -> numerator = Myint_multiply(a -> numerator, b -> denominator);
    c -> denominator = Myint_multiply(a -> denominator, b -> numerator);
    return c;
}
