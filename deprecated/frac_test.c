#include"frac.h"

// If this is set to 1, it is intended that you redirect stderr to tmp.txt e.g.:
//      ./bigint_test.out 2> tmp.txt
// The #define MEM_LEAK_CHK in frac.c also needs to be set to 1.
#define MEM_LEAK_CHK 1
#define EXS 9

/* Test coverage:
f_0: 0/1
f_1: less than 8-bit int both num & den, num < den, in lowest terms
f_2: less than 8-bit int both num & den, num > den, in lowest terms
f_3: between 32 & 64 bits int in both num & den, num < den, in lowest terms
f_4: between 32 & 64 bits int in both num & den, num > den, in lowest terms
f_5: num betw 32 & 64 bits, den more than 64 bits, in lowest terms
f_6: num more than 64 bits, den betw 32 & 64 bits, in lowest terms
f_7: num & den more than 64 bits, num < den, not in lowest terms
f_8: num & den more than 64 bits, num > den, not in lowest terms
*/

// I decided to just use the arithmetic checks as the memory leak checks.

struct Fraction* f_0() {
    struct Myint* n = Myint_from_long(0);
    struct Myint* d = Myint_from_long(1);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_1() {
    struct Myint* n = Myint_from_long(1);
    struct Myint* d = Myint_from_long(5);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_2() {
    struct Myint* n = Myint_from_long(11);
    struct Myint* d = Myint_from_long(7);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_3() {
    struct Myint* n = Myint_from_long((long) 23*29*31*61*67*71);
    struct Myint* d = Myint_from_long((long) 37*41*43*47*53*59);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}


struct Fraction* f_4() {
    struct Myint* n = Myint_from_long((long) 47*53*59*61*67*71);
    struct Myint* d = Myint_from_long((long) 37*41*43*73*79*83);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_5() {
    struct Myint* n = Myint_from_long((long) 47*53*59*61*67*71);
    struct Myint* d = Myint_from_long((long) 1 << 32);
    Myint_promote(d);
    Bigint_enqueue(d -> bigint, (long) 1 << 31);
    Bigint_enqueue(d -> bigint, (long) 1 << 30);
    Bigint_enqueue(d -> bigint, (long) 1 << 29);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_6() {
    struct Myint* n = Myint_from_long((long) 1 << 32);
    Myint_promote(n);
    Bigint_enqueue(n -> bigint, (long) 1 << 31);
    Bigint_enqueue(n -> bigint, (long) 1 << 30);
    Bigint_enqueue(n -> bigint, (long) 1 << 29);
    struct Myint* d = Myint_from_long((long) 37*41*43*47*53*59);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_7() {
    struct Myint* n = Myint_from_long((long) 1 << 32);
    Myint_promote(n);
    Bigint_enqueue(n -> bigint, (long) 1 << 31);
    Bigint_enqueue(n -> bigint, (long) 1 << 30);
    Bigint_enqueue(n -> bigint, (long) 1 << 29);
    struct Myint* d = Myint_from_long((long) 1 << 32);
    Myint_promote(d);
    Bigint_enqueue(d -> bigint, 0);
    Bigint_enqueue(d -> bigint, 0);
    Bigint_enqueue(d -> bigint, 0);
    Bigint_enqueue(d -> bigint, (long) 1 << 30);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_8() {
    struct Myint* n = Myint_from_long((long) 1 << 32);
    Myint_promote(n);
    Bigint_enqueue(n -> bigint, 0);
    Bigint_enqueue(n -> bigint, 0);
    Bigint_enqueue(n -> bigint, 0);
    Bigint_enqueue(n -> bigint, (long) 1 << 30);
    struct Myint* d = Myint_from_long((long) 1 << 32);
    Myint_promote(d);
    Bigint_enqueue(d -> bigint, (long) 1 << 31);
    Bigint_enqueue(d -> bigint, (long) 1 << 30);
    Bigint_enqueue(d -> bigint, (long) 1 << 29);
    struct Fraction* a = Fraction_from_Myints(n, d);
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction** f_n() {
    struct Fraction** fn = malloc(EXS * sizeof(struct Fraction*));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc fn_array %li\n", (long) fn);
    }
    fn[0] = f_0();
    fn[1] = f_1();
    fn[2] = f_2();
    fn[3] = f_3();
    fn[4] = f_4();
    fn[5] = f_5();
    fn[6] = f_6();
    fn[7] = f_7();
    fn[8] = f_8();
    return fn;
}

void arithmetic_checks() {
    int signs[8] = {1, 1, 1, -1, -1, 1, -1, -1};
    struct Fraction** fn = f_n();
    struct Fraction* a = NULL;
    struct Fraction* b = NULL;
    struct Fraction* c = NULL;
    struct Fraction* d = NULL;
    struct Fraction** dm = NULL;
    struct Fraction* zero = fn[0];
    long n;
    long bitshifts[5] = {0, 1, 30, 100, 300};
    for (long i=0; i<EXS; i++) {
        a = fn[i];
        printf("test Fraction #%li: ", i); Fraction_print(a); printf("\n");
        Myint_neg(a -> numerator);
        printf("negated: "); Fraction_print(a); printf("\n");
        Myint_neg(a -> numerator);
    }

    // deepcopy test
    printf("\n\nTest deepcopy:\n");
    for (long i=1; i<5; i++) {
        a = fn[2*i];
        // Make 2 copies so that I don't destroy the actual array entry
        b = Fraction_deepcopy(a);
        c = Fraction_deepcopy(b);
        printf("b is deepcopy of fn[%li], c is deepcopy of b.\n", 2*i);
        printf("b is "); Fraction_print(b); 
        printf(" at address %li\n", (long) b);
        printf("c is "); Fraction_print(c); 
        printf(" at address %li\n", (long) c);
        printf("\nNow destroy b, see if c still exists.\n");
        b = Fraction_destructor(b);
        printf("b is "); Fraction_print(b); 
        printf(" at address %li\n", (long) b);
        printf("c is "); Fraction_print(c); 
        printf(" at address %li\n", (long) c);
        c = Fraction_destructor(c);
        printf("\n");
    }

    printf("\n\nTest addition:\n");
    for (long i=0; i<EXS; i++) {
        a = fn[i];
        for (long j=i; j<EXS; j++) {
            b = Fraction_deepcopy(fn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
                c = Fraction_add(a, b);
                Fraction_print(a); printf(" + "); 
                Fraction_print(b); printf(" = ");
                Fraction_print(c); printf("\n");
                d = Fraction_subtract(c, a);
                if (Fraction_equal(d, b)) {
                    printf("Passed check: (a+b) - a = b\n");
                } else {
                    printf("ERROR: (a+b) - a != b, a+b = ");
                    Fraction_print(d); printf("\n");
                }
                d = Fraction_destructor(d);
                d = Fraction_subtract(c, b);
                if (Fraction_equal(d, a)) {
                    printf("Passed check: (a+b) - b = a\n");
                } else {
                    printf("ERROR: (a+b) - b != a, a+b = ");
                    Fraction_print(d); printf("\n");
                }
                d = Fraction_destructor(d);
                c = Fraction_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
            }
            b = Fraction_destructor(b);
        }
    }

    printf("\n\nTest subtraction:\n");
    for (long i=0; i<EXS; i++) {
        a = fn[i];
        for (long j=i; j<EXS; j++) {
            b = Fraction_deepcopy(fn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
                c = Fraction_subtract(a, b);
                Fraction_print(a); printf(" - "); 
                Fraction_print(b); printf(" = ");
                Fraction_print(c); printf("\n");
                d = Fraction_add(c, b);
                if (Fraction_equal(d, a)) {
                    printf("Passes check: (a-b) + b = a\n");
                } else {
                    printf("ERROR: (a-b) + b != a, a-b = ");
                    Fraction_print(d); printf("\n");
                }
                d = Fraction_destructor(d);
                c = Fraction_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
            }
            b = Fraction_destructor(b);
        }
    }

    printf("\n\nTest multiplication:\n");
    for (long i=0; i<EXS; i++) {
        a = fn[i];
        for (long j=i; j<EXS; j++) {
            b = Fraction_deepcopy(fn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
                c = Fraction_multiply(a, b);
                Fraction_print(a); printf(" * "); 
                Fraction_print(b); printf(" = ");
                Fraction_print(c); printf("\n");
                if (i > 0 && j > 0) {
                    d = Fraction_divide(c, b);
                    Fraction_reduce(d);
                    if (Fraction_equal(d, a)) {
                        printf("Passes check: ab // b = a\n");
                    } else {
                        printf("ERROR: Fails check: ab // b = ");
                        Fraction_print(d);
                        printf(" != a\n");
                    }
                    d = Fraction_destructor(d);
                    d = Fraction_divide(c, a);
                    Fraction_reduce(d);
                    if (Fraction_equal(d, b)) {
                        printf("Passes check: ab // a = b\n");
                    } else {
                        printf("ERROR: Fails check: ab // a =\n");
                        Fraction_print(d);
                        printf(" != b\n");
                    }
                    d = Fraction_destructor(d);
                }
                c = Fraction_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
            }
            b = Fraction_destructor(b);
        }
    }

    printf("\n\nTest divide:\n");
    a = fn[0];
    b = fn[1];
    d = Fraction_divide(a, b);
    printf("0 // 1 = "); Fraction_print(d); printf("\n");
    d = Fraction_destructor(d);
    // Interestingly, C evaluates n/0 and n%0 and returns 0, even when n=0
    // Makes for a lot fewer crashes, but is mathematically problematic.
    a = fn[1];
    b = fn[0];
    d = Fraction_divide(a, b);
    printf("1 // 0 = "); Fraction_print(d); printf("\n");
    d = Fraction_destructor(d);
    a = fn[0];
    b = fn[0];
    d = Fraction_divide(a, b);
    printf("0 // 0 = "); Fraction_print(d); printf("\n");
    d = Fraction_destructor(d);
    for (long i=1; i<EXS; i++) {
        a = fn[i];
        for (long j=i; j<EXS; j++) {
            b = Fraction_deepcopy(fn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
                d = Fraction_divide(a, b);
                Fraction_reduce(d);
                Fraction_print(a); printf(" // "); 
                Fraction_print(b); printf(" = ");
                Fraction_print(d); printf("\n");
                c = Fraction_multiply(d, b);
                if (Fraction_equal(c, a)) {
                    printf("Passes check: a = bq\n");
                } else {
                    printf("ERROR: Fails check: a != bq\n");
                }
                c = Fraction_destructor(c);
                d = Fraction_destructor(d);
                d = Fraction_divide(b, a);
                Fraction_reduce(d);
                Fraction_print(b); printf(" // "); 
                Fraction_print(a); printf(" = ");
                Fraction_print(d); printf("\n");
                c = Fraction_multiply(d, a);
                if (Fraction_equal(c, b)) {
                    printf("Passes check: b = aq\n");
                } else {
                    printf("ERROR: Fails check: b != aq\n");
                }
                c = Fraction_destructor(c);
                d = Fraction_destructor(d);
                if (signs[2*k] < 0) {
                    Myint_neg(a -> numerator);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b -> numerator);
                }
            }
            b = Fraction_destructor(b);
        }
    }

    for (long i=0; i<EXS; i++) {
        fn[i] = Fraction_destructor(fn[i]);
    }
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free fn_array %li\n", (long) fn);
    }
    free(fn); fn = NULL;
}    

int main() {
    arithmetic_checks();
}