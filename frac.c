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

struct Fraction* Fraction_from_Myints(struct Myint* n, struct Myint* d) {
    struct Fraction* a = malloc(sizeof(struct Fraction));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Fraction %li\n", (long) a);
    }
    a -> numerator = n;
    a -> denominator = d;
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

void Fraction_print_stderr(struct Fraction* a) {
    if (!(Fraction_contract(a))) {
        fprintf(stderr, "Fraction_contract fails at Fraction_print\n");
        return;
    }
    if (a) {
        Myint_print_stderr(a -> numerator);
        printf(" / "); 
        Myint_print_stderr(a -> denominator);
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
    struct Myint* one = Myint_from_long(1);
    struct Myint* tmp = NULL;
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
    struct Fraction* tmp_a = Fraction_deepcopy(a);
    struct Fraction* tmp_b = Fraction_deepcopy(b);
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    struct Myint* c_num = Myint_add(a -> numerator, b -> numerator);
    struct Myint* c_den = Myint_deepcopy(a -> denominator);
    struct Fraction* c = Fraction_from_Myints(c_num, c_den);
    a -> numerator = Myint_destructor(a -> numerator);
    a -> denominator = Myint_destructor(a -> denominator);
    memcpy(a, tmp_a, sizeof(struct Fraction));
    b -> numerator = Myint_destructor(b -> numerator);
    b -> denominator = Myint_destructor(b -> denominator);
    memcpy(b, tmp_b, sizeof(struct Fraction));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Fraction %li\n", (long) tmp_a);
    }
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Fraction %li\n", (long) tmp_b);
    }
    free(tmp_a); tmp_a = NULL;
    free(tmp_b); tmp_b = NULL;
    struct Myint* zero = Myint_from_long(0);
    if (!Myint_equal(c -> numerator, zero) && 
            !Myint_equal(c -> denominator, zero)) {
        Fraction_reduce(c);
    } else if (!Myint_equal(c -> denominator, zero)) {
        c -> numerator = Myint_destructor(c -> numerator);
        c -> denominator = Myint_destructor(c -> denominator);
        c -> numerator = Myint_from_long(0);
        c -> denominator = Myint_from_long(1);
    } else {
        fprintf(stderr, "ERROR: denominator is zero in Fraction_add\n");
    }
    Myint_destructor(zero);
    return c;
}

struct Fraction* Fraction_subtract(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_subtract, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_subtract, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Fraction* tmp_a = Fraction_deepcopy(a);
    struct Fraction* tmp_b = Fraction_deepcopy(b);
    Fraction_common_denom(a, b);
    // a & b should have the same denominator at this point
    struct Myint* c_num = Myint_subtract(a -> numerator, b -> numerator);
    struct Myint* c_den = Myint_deepcopy(a -> denominator);
    struct Fraction* c = Fraction_from_Myints(c_num, c_den);
    a -> numerator = Myint_destructor(a -> numerator);
    a -> denominator = Myint_destructor(a -> denominator);
    memcpy(a, tmp_a, sizeof(struct Fraction));
    b -> numerator = Myint_destructor(b -> numerator);
    b -> denominator = Myint_destructor(b -> denominator);
    memcpy(b, tmp_b, sizeof(struct Fraction));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Fraction %li\n", (long) tmp_a);
    }
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Fraction %li\n", (long) tmp_b);
    }
    free(tmp_a); tmp_a = NULL;
    free(tmp_b); tmp_b = NULL;
    struct Myint* zero = Myint_from_long(0);
    if (!Myint_equal(c -> numerator, zero) && 
            !Myint_equal(c -> denominator, zero)) {
        Fraction_reduce(c);
    } else if (!Myint_equal(c -> denominator, zero)) {
        c -> numerator = Myint_destructor(c -> numerator);
        c -> denominator = Myint_destructor(c -> denominator);
        c -> numerator = Myint_from_long(0);
        c -> denominator = Myint_from_long(1);
    } else {
        fprintf(stderr, "ERROR: denominator is zero in Fraction_subtract\n");
    }
    Myint_destructor(zero);
    Fraction_reduce(c);
    return c;
}

struct Fraction* Fraction_multiply(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_multiply, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_multiply, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c_num = Myint_multiply(a -> numerator, b -> numerator);
    struct Myint* c_den = Myint_multiply(a -> denominator, b -> denominator);
    struct Fraction* c = Fraction_from_Myints(c_num, c_den);
    return c;
}

struct Fraction* Fraction_divide(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_divide, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_divide, b\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c_num = Myint_multiply(a -> numerator, b -> denominator);
    struct Myint* c_den = Myint_multiply(a -> denominator, b -> numerator);
    struct Fraction* c = Fraction_from_Myints(c_num, c_den);
    return c;
}

bool Fraction_equal(struct Fraction* a, struct Fraction* b) {
    bool fail = false;
    if (!(Fraction_contract(a))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_equal, a\n");
        fail = true;
    }
    if (!(Fraction_contract(b))) {
        fprintf(stderr, 
                "ERROR: Fraction_contract fails at Fraction_equal, b\n");
        fail = true;
    }
    struct Myint* zero = Myint_from_long(0);
    if (Myint_equal(a -> denominator, zero)) {
        fprintf(stderr, "ERROR: Division by zero in Fraction_equal, a\n");
        fail = true;
    }
    if (Myint_equal(b -> denominator, zero)) {
        fprintf(stderr, "ERROR: Division by zero in Fraction_equal, b\n");
        fail = true;
    }
    if (fail) {return false;}
    bool eq = false;
    if (Myint_equal(a -> numerator, zero) && 
            Myint_equal(a -> numerator, zero)) {
        eq = true;
    }
    zero = Myint_destructor(zero);
    if (!eq) {
        struct Myint* c = NULL;
        struct Myint* d = NULL;
        bool a_first = false;
        if (Myint_geq(a -> numerator, b -> numerator)) {
            c = Myint_divide(a -> numerator, b -> numerator);
            a_first = true;
        } else {
            c = Myint_divide(b -> numerator, a -> numerator);
        }
        if (a_first) {
            d = Myint_divide(a -> denominator, b -> denominator);
        } else {
            d = Myint_divide(b -> denominator, a -> denominator);
        }
        if (Myint_equal(c, d)) {eq = true;}
        c = Myint_destructor(c);
        d = Myint_destructor(d);
    }
    return eq;
}