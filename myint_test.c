#include"myint.h"

// If this is set to 1, it is intended that you redirect stderr to tmp.txt e.g.:
//      ./bigint_test.out 2> tmp.txt
// The #define MEM_LEAK_CHK in myint.c also needs to be set to 1.
#define MEM_LEAK_CHK 0
#define EXS 5

struct Myint* m_0() {
    struct Myint* a = Myint_constructor();
    return a;
}

struct Myint* m_1() {
    struct Myint* a = Myint_from_long(1);
    return a;
}

struct Myint* m_2() {
    struct Myint* a = Myint_from_long((long) 37*41*43*47*53*59);
    return a;
}

struct Myint* m_3() {
    struct Myint* a = Myint_from_long((long) 1 << 32);
    Myint_promote(a);
    Bigint_enqueue(a -> bigint, (long) 1 << 31);
    Bigint_enqueue(a -> bigint, (long) 1 << 30);
    Bigint_enqueue(a -> bigint, (long) 1 << 29);
    return a;
}

struct Myint* m_4() {
    struct Myint* a = Myint_from_long((long) 1 << 32);
    Myint_promote(a);
    Bigint_enqueue(a -> bigint, 0);
    Bigint_enqueue(a -> bigint, 0);
    Bigint_enqueue(a -> bigint, 0);
    Bigint_enqueue(a -> bigint, (long) 1 << 30);
    return a;
}

struct Myint** m_n() {
    struct Myint** mn = malloc(EXS * sizeof(struct Myint*));
    mn[0] = m_0();
    mn[1] = m_1();
    mn[2] = m_2();
    mn[3] = m_3();
    mn[4] = m_4();
    return mn;
}

void memory_leak_checks() {
    struct Myint* a = m_3();
    struct Myint* b = Myint_deepcopy(a);
    struct Myint* c = Myint_add(a, b);
    struct Myint* d = Myint_multiply(a, b);
    struct Myint* e = Myint_subtract(d, c);
    struct Myint* f = Myint_gcd(d, e);
    struct Myint** dm = Myint_divmod(d, e);
    struct Myint* g = dm[0];
    struct Myint* h = dm[1];
    struct Myint* i = m_2();
    struct Myint* j = Myint_gcd(d, i);
    struct Myint* k = Myint_bitshift_left(d, 100);
    struct Myint* l = Myint_bitshift_right(d, 100);
    printf("a: "); Myint_print(a); printf("\n");
    printf("b: "); Myint_print(b); printf("\n");
    printf("c: "); Myint_print(c); printf("\n");
    printf("d: "); Myint_print(d); printf("\n");
    printf("e: "); Myint_print(e); printf("\n");
    printf("f: "); Myint_print(f); printf("\n");
    printf("g: "); Myint_print(g); printf("\n");
    printf("h: "); Myint_print(h); printf("\n");
    printf("i: "); Myint_print(i); printf("\n");
    printf("j: "); Myint_print(j); printf("\n");
    printf("k: "); Myint_print(k); printf("\n");
    printf("l: "); Myint_print(l); printf("\n");
    a = Myint_destructor(a);
    b = Myint_destructor(b);
    c = Myint_destructor(c);
    d = Myint_destructor(d);
    e = Myint_destructor(e);
    f = Myint_destructor(f);
    g = Myint_destructor(g);
    h = Myint_destructor(h);
    i = Myint_destructor(i);
    j = Myint_destructor(j);
    k = Myint_destructor(k);
    l = Myint_destructor(l);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free dm-from-Myint_divmod-test %li\n", (long) dm);
    }
    free(dm); dm = NULL;
}

void arithmetic_checks() {
    char signs[8] = {1, 1, 1, -1, -1, 1, -1, -1};
    struct Myint** mn = m_n();
    struct Myint* a = NULL;
    struct Myint* b = NULL;
    struct Myint* c = NULL;
    struct Myint* d = NULL;
    struct Myint** dm = NULL;
    struct Myint* zero = Myint_from_long(0);
    long n;
    long bitshifts[5] = {0, 1, 30, 100, 300};
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        printf("test Myint #%li: ", i); Myint_print(a); printf("\n");
        Myint_neg(a);
        printf("negated: "); Myint_print(a); printf("\n");
        Myint_neg(a);
    }

    // deepcopy test
    printf("\n\nTest deepcopy:\n");
    a = mn[2];
    // Make 2 copies so that I don't destroy the actual array entry
    b = Myint_deepcopy(a);
    c = Myint_deepcopy(b);
    printf("b is deepcopy of mn[2], c is deepcopy of b.\n");
    printf("b is "); Myint_print(b); printf(" at address %li\n", (long) b);
    printf("c is "); Myint_print(c); printf(" at address %li\n", (long) c);
    printf("\nNow destroy b, see if c still exists.\n");
    b = Myint_destructor(b);
    printf("b is "); Myint_print(b); printf(" at address %li\n", (long) b);
    printf("c is "); Myint_print(c); printf(" at address %li\n", (long) c);
    c = Myint_destructor(c);

    printf("\n\nTest addition:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        for (long j=i; j<EXS; j++) {
            b = Myint_deepcopy(mn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
                c = Myint_add(a, b);
                Myint_print(a); printf(" + "); 
                Myint_print(b); printf(" = ");
                Myint_print(c); printf("\n");
                c = Myint_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
            }
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest subtraction:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        for (long j=i; j<EXS; j++) {
            b = Myint_deepcopy(mn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
                c = Myint_subtract(a, b);
                Myint_print(a); printf(" - "); 
                Myint_print(b); printf(" = ");
                Myint_print(c); printf("\n");
                c = Myint_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
            }
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest multiplication:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        for (long j=i; j<EXS; j++) {
            b = Myint_deepcopy(mn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
                c = Myint_multiply(a, b);
                Myint_print(a); printf(" * "); 
                Myint_print(b); printf(" = ");
                Myint_print(c); printf("\n");
                c = Myint_destructor(c);
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
            }
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest divmod:\n");
    a = mn[0];
    b = mn[1];
    dm = Myint_divmod(a, b);
    printf("0 // 1 = "); Myint_print(dm[0]); printf("\n");
    printf("0 %% 1 = "); Myint_print(dm[1]); printf("\n");
    free(dm); dm = NULL;
    // Interestingly, C evaluates n/0 and n%0 and returns 0, even when n=0
    // Makes for a lot fewer crashes, but is mathematically problematic.
    a = mn[1];
    b = mn[0];
    dm = Myint_divmod(a, b);
    printf("1 // 0 = "); Myint_print(dm[0]); printf("\n");
    printf("1 %% 0 = "); Myint_print(dm[1]); printf("\n");
    free(dm); dm = NULL;
    a = mn[0];
    b = mn[0];
    dm = Myint_divmod(a, b);
    printf("0 // 0 = "); Myint_print(dm[0]); printf("\n");
    printf("0 %% 0 = "); Myint_print(dm[1]); printf("\n");
    free(dm); dm = NULL;
    for (long i=1; i<EXS; i++) {
        a = mn[i];
        for (long j=i; j<EXS; j++) {
            b = Myint_deepcopy(mn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
                dm = Myint_divmod(a, b);
                Myint_print(a); printf(" // "); 
                Myint_print(b); printf(" = ");
                Myint_print(dm[0]); printf("\n");
                Myint_print(a); printf(" %% "); 
                Myint_print(b); printf(" = ");
                Myint_print(dm[1]); printf("\n");
                c = Myint_multiply(dm[0], a);
                d = Myint_add(dm[1], c);
                if (Myint_equal(d, a)) {
                    printf("Passes check: a = bq + r\n");
                } else {
                    printf("ERROR: Fails check: a != bq + r\n");
                }
                c = Myint_destructor(c);
                d = Myint_destructor(d);
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                dm = Myint_divmod(b, a);
                Myint_print(b); printf(" // "); 
                Myint_print(a); printf(" = ");
                Myint_print(dm[0]); printf("\n");
                Myint_print(b); printf(" %% "); 
                Myint_print(a); printf(" = ");
                Myint_print(dm[1]); printf("\n");
                c = Myint_multiply(dm[0], a);
                d = Myint_add(dm[1], c);
                if (Myint_equal(d, b)) {
                    printf("Passes check: b = aq + r\n");
                } else {
                    printf("ERROR: Fails check: b != aq + r\n");
                }
                c = Myint_destructor(c);
                d = Myint_destructor(d);
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
            }
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest gcd:\n");
    // These are similar to the divmod results dividing by zero: they don't 
    //      crash, but are mathematically problematic.
    a = mn[0];
    b = mn[1];
    c = Myint_gcd(a, b);
    printf("gcd(0, 1) = "); Myint_print(c); printf("\n");
    a = mn[1];
    b = mn[0];
    c = Myint_gcd(a, b);
    printf("gcd(1, 0) = "); Myint_print(c); printf("\n");
    a = mn[0];
    b = mn[0];
    c = Myint_gcd(a, b);
    printf("gcd(0, 0) = "); Myint_print(c); printf("\n");
    for (long i=1; i<EXS; i++) {
        a = mn[i];
        for (long j=i; j<EXS; j++) {
            b = Myint_deepcopy(mn[j]);
            for (long k=0; k<4; k++) {
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
                c = Myint_gcd(a, b);
                printf("gcd("); Myint_print(a); printf(", "); 
                Myint_print(b); printf(") = ");
                Myint_print(c); printf("\n");
                dm = Myint_divmod(a, c);
                if (Myint_equal(dm[1], zero)) {
                    printf("Passes check: a %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: a %% gcd(a, b) !=0\n");
                    printf("Got "); Myint_print(c); printf(" instead.\n");
                }
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                dm = Myint_divmod(b, c);
                if (Myint_equal(dm[1], zero)) {
                    printf("Passes check: b %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
                    printf("Got "); Myint_print(c); printf(" instead.\n");
                }
                c = Myint_destructor(c);
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                c = Myint_gcd(b, a);
                printf("gcd("); Myint_print(b); printf(", "); 
                Myint_print(a); printf(") = ");
                Myint_print(c); printf("\n");
                dm = Myint_divmod(a, c);
                if (Myint_equal(dm[1], zero)) {
                    printf("Passes check: a %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: a %% gcd(a, b) != 0\n");
                    printf("Got "); Myint_print(c); printf(" instead.\n");
                }
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                dm = Myint_divmod(b, c);
                if (Myint_equal(dm[1], zero)) {
                    printf("Passes check: b %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
                    printf("Got "); Myint_print(c); printf(" instead.\n");
                }
                c = Myint_destructor(c);
                dm[0] = Myint_destructor(dm[0]);
                dm[1] = Myint_destructor(dm[1]);
                free(dm); dm = NULL;
                if (signs[2*k] < 0) {
                    Myint_neg(a);
                }
                if (signs[2*k + 1] < 0) {
                    Myint_neg(b);
                }
            }
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest intlog2:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        n = Myint_intlog2(a);
        printf("Largest nonzero bit of "); Myint_print(a); 
        printf(" = %li\n", n); 
        Myint_neg(a);
        n = Myint_intlog2(a);
        printf("Largest nonzero bit of "); Myint_print(a); 
        printf(" = %li\n", n); 
        Myint_neg(a);
    }

    printf("\n\nTest bitshift_left:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        for (long j=0; j<5; j++) {
            b = Myint_bitshift_left(a, bitshifts[j]);
            Myint_print(a); printf(" << %li = ", bitshifts[j]);
            Myint_print(b); printf("\n");
            b = Myint_destructor(b);
        }
    }

    printf("\n\nTest bitshift_right:\n");
    for (long i=0; i<EXS; i++) {
        a = mn[i];
        for (long j=0; j<5; j++) {
            b = Myint_bitshift_right(a, bitshifts[j]);
            Myint_print(a); printf(" >> %li = ", bitshifts[j]);
            Myint_print(b); printf("\n");
            b = Myint_destructor(b);
        }
    }

}    

int main() {
    // memory_leak_checks();
    arithmetic_checks();
}