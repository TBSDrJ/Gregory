#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"polynomial.h"
#include"myint.h"
#include"bigint.h"

/*
I stumbled across this, which had some good suggestions about how to make a
constructor of sorts, it makes things a little simpler than what I had:
https://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
Looking back, that probably should have been obvious.
*/

struct Polynomial* Polynomial_constructor(long deg) {
    struct Polynomial* p = malloc(sizeof(struct Polynomial));
    if (deg < 0) {deg = 0;}
    if (!p) {return NULL;}
    // Notice we need 1 *more* than the degree 
    struct Myint** coeffs = malloc((deg + 1) * sizeof(struct Myint*));
    if (!coeffs) {return NULL;}
    // I'm choosing to initialize also
    for (long i=0; i<=deg; i++) {
        coeffs[i] = Myint_constructor();
        coeffs[i] -> my_long = 0;
    }
    p -> degree = deg;
    p -> coeffs = coeffs;
    return p;
}

struct Polynomial* Polynomial_destructor(struct Polynomial* p) {
    if (p) {
        for (long i=0; i<=(p -> degree); i++) {
            p -> coeffs[i] = Myint_destructor(p -> coeffs[i]);
        }
        free(p -> coeffs);
        free(p);
    }
    return NULL;
}

bool Polynomial_contract(struct Polynomial* p) {
    bool fail = false;
    if (!p) {
        printf("Polynomial contract fails, p is null.\n");
        fail = true;
    }
    if (p -> degree < 0) {
        printf("Polynomial contract fails, p -> degree is negative.\n");
        fail = true;
    }
    if (!p -> coeffs) {
        printf("Polynomial contract fails, p -> coeffs is null.\n");
        fail = true;
    }
    return !fail;
}

/* 
Unary operations:
1. Print
2. Reduce (if highest deg coeff = 0, Polynomial_reduce degree)
3. Negative
4. Derivative
5. Substitute a value

Binary Operations:
1. Sum
2. Product
*/

void Polynomial_print(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_print.\n");
        return;
    }
    for (long i=(p -> degree); i>1; i--) {
        if (p -> coeffs[i] -> int_type == LONG) {
            Myint_print(p -> coeffs[i]);
        } else {
            printf("("); Myint_print(p -> coeffs[i]); printf(")");
        }
        printf("x^%li + ", i);
    }
    if (p -> degree > 0) {
        if (p -> coeffs[1] -> int_type == LONG) {
            Myint_print(p -> coeffs[1]);
        } else {
            printf("("); Myint_print(p -> coeffs[1]); printf(")");
        }
        printf("x + ");
    }
    if (p -> coeffs[0] -> int_type == LONG) {
        Myint_print(p -> coeffs[0]);
    } else {
        printf("("); Myint_print(p -> coeffs[0]); printf(")");
    }
}

void Polynomial_reduce(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_reduce.\n");
        return;
    }
    long n = p -> degree;
    struct Myint* zero = Myint_constructor();
    while(Myint_equal(p -> coeffs[n], zero)) {
        n--;
        if (n < 0) {n = 0; break;}
    }
    if (n < p -> degree) {
        p -> degree = n;
        struct Myint** coeffs = malloc((n+1)*sizeof(struct Myint*));
        memcpy(coeffs, p -> coeffs, (n+1)*sizeof(struct Myint*));
        free(p -> coeffs); p -> coeffs = NULL;
        p -> coeffs = coeffs;
    }
}

struct Polynomial* Polynomial_deepcopy(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_deepcopy.\n");
        return NULL;
    }
    struct Polynomial* q = Polynomial_constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        q -> coeffs[i] = Myint_deepcopy(p -> coeffs[i]);
    }
    Polynomial_reduce(q);
    return q;
}

struct Polynomial* Polynomial_neg(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_neg.\n");
        return NULL;
    }
    struct Polynomial* q = Polynomial_constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        q -> coeffs[i] = Myint_deepcopy(p -> coeffs[i]);
        if (q -> coeffs[i] -> int_type == LONG) {
            q -> coeffs[i] -> sign *= -1;
        } else {
            q -> coeffs[i] -> bigint -> sign *= -1;
        }
    }
    Polynomial_reduce(q);
    return q;
}

struct Polynomial* Polynomial_der(struct Polynomial* p) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_der.\n");
        return NULL;
    }
    struct Polynomial* q = Polynomial_constructor((p -> degree) - 1);
    for (long i=0; i<(p -> degree); i++) {
        struct Myint* p_coeff = Myint_deepcopy((p -> coeffs)[i+1]);
        struct Myint* a = Myint_constructor();
        a -> my_long = i + 1;
        (q -> coeffs)[i] = Myint_multiply(p_coeff, a);
        a = Myint_destructor(a);
        p_coeff = Myint_destructor(p_coeff);
    }
    Polynomial_reduce(q);
    return q;
}

struct Myint* Polynomial_subs(struct Polynomial* p, long x) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_subs.\n");
        return NULL;
    }
    if (x == 0) {
        return Myint_deepcopy(p -> coeffs[0]);
    }
    struct Myint* tmp;
    struct Myint* result = Myint_constructor();
    struct Myint* myint_x = Myint_constructor();
    if (x > 0) {
        myint_x -> my_long = x;
    } else {
        myint_x -> my_long = -x;
        myint_x -> sign = -1;
    }
    for (long i=0; i <= (p -> degree); i++) {
        struct Myint* term = Myint_constructor();
        term -> my_long = 1;
        for (long j=0; j<i; j++) {
            tmp = term;
            term = Myint_multiply(term, myint_x);
            tmp = Myint_destructor(tmp);
        }
        tmp = term;
        term = Myint_multiply(p -> coeffs[i], term);
        tmp = Myint_destructor(tmp);
        tmp = result;
        result = Myint_add(result, term);
        tmp = Myint_destructor(tmp);
    }
    return result;
}

struct Polynomial* Polynomial_add(
        struct Polynomial* p, struct Polynomial* q) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_add, p.\n");
        return NULL;
    }
    if (!Polynomial_contract(q)) {
        printf("Polynomial_contract fails at Polynomial_add, q.\n");
        return NULL;
    }
    // Make p be the one with greater degree if they are different.
    if ((p -> degree) < (q -> degree)) {
        struct Polynomial* temp = p;
        p = q;
        q = temp;
    }
    struct Polynomial* s = Polynomial_constructor(p -> degree);
    for (long i=0; i<=(q -> degree); i++) {
        s -> coeffs[i] = Myint_add(p -> coeffs[i], q -> coeffs[i]);
    }
    for (long i=(q -> degree) + 1; i<=(p -> degree); i++) {
        s -> coeffs[i] = Myint_deepcopy(p -> coeffs[i]);
    }
    Polynomial_reduce(s);
    return s;
}

struct Polynomial* Polynomial_multiply(
            struct Polynomial* p, struct Polynomial* q) {
    if (!Polynomial_contract(p)) {
        printf("Polynomial_contract fails at Polynomial_add, p.\n");
        return NULL;
    }
    if (!Polynomial_contract(q)) {
        printf("Polynomial_contract fails at Polynomial_add, q.\n");
        return NULL;
    }
    struct Polynomial* s = Polynomial_constructor(
                (p -> degree) + (q -> degree));
    struct Myint* tmp_1;
    struct Myint* tmp_2;
    for (long i=0; i<=(p -> degree); i++) {
        for (long j=0; j<=(q -> degree); j++) {
            tmp_1 = Myint_multiply(p -> coeffs[i], q -> coeffs[j]);
            tmp_2 = s -> coeffs[i + j];
            s -> coeffs[i + j] = Myint_add(s -> coeffs[i + j], tmp_1);
            tmp_1 = Myint_destructor(tmp_1);
            tmp_2 = Myint_destructor(tmp_2);
        }
    }
    Polynomial_reduce(s);
    return s;
}
