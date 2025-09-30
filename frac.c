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
        printf("Fraction_contract fails, a -> denominator is NULL.\n");
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

struct Fraction* Fraction_deepcopy(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        printf("Fraction_contract fails at Fraction_deepcopy\n");
        return NULL;
    }
    struct Fraction* b = Fraction_constructor();
    Myint_destructor(b -> numerator);
    Myint_destructor(b -> denominator);
    b -> numerator = Myint_deepcopy(a -> numerator);
    b -> denominator = Myint_deepcopy(a -> denominator);
    return b;
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
        printf("Fraction_contract fails at Fraction_subtract, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_subtract, b\n");
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
        printf("Fraction_contract fails at Fraction_multiply, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_multiply, b\n");
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
        printf("Fraction_contract fails at Fraction_multiply, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        printf("Fraction_contract fails at Fraction_multiply, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* c = Fraction_constructor();
    c -> numerator = Myint_multiply(a -> numerator, b -> denominator);
    c -> denominator = Myint_multiply(a -> denominator, b -> numerator);
    return c;
}

int main() {
    struct Fraction* a = Fraction_constructor();
    a -> numerator -> my_long = 1;
    a -> denominator -> my_long = 2;
    struct Fraction* b = Fraction_deepcopy(a);
    a = Fraction_destructor(a);
    b = Fraction_destructor(b);
    // struct Fraction* a = Fraction_constructor();
    // struct Fraction* b = Fraction_constructor();
    // struct Fraction* c = Fraction_constructor();
    // struct Fraction* d = Fraction_constructor();
    // struct Fraction* e = Fraction_constructor();
    // a -> numerator -> my_long = 3;
    // a -> denominator -> my_long = 4;
    // b -> numerator -> my_long = 2;
    // b -> denominator -> my_long = 3;
    // struct Myint* two = Myint_constructor();
    // two -> my_long = 2;
    // struct Myint* x = Myint_constructor();
    // x -> my_long = 2;
    // for (int i=0; i<65; i++) {
    //     x = Myint_multiply(x, two);
    // }
    // a -> numerator -> my_long = 1;
    // a -> denominator = x;
    // b -> numerator = x;
    // b -> denominator -> my_long = 1;
    // c = Fraction_multiply(a, b); Fraction_reduce(c);
    // d = Fraction_divide(a, b); Fraction_reduce(d);
    // e = Fraction_divide(b, a); Fraction_reduce(e);
    // printf("a: "); Fraction_print(a); printf("\n");
    // printf("b: "); Fraction_print(b); printf("\n");
    // printf("a * b: "); Fraction_print(c); printf("\n");
    // printf("a / b: "); Fraction_print(d); printf("\n");
    // printf("b / a: "); Fraction_print(e); printf("\n");
    // a = Fraction_destructor(a);
    // b = Fraction_destructor(b);
    // c = Fraction_destructor(c);
    // d = Fraction_destructor(d);
    // e = Fraction_destructor(e);
}