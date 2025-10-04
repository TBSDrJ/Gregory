#include"bigint.h"

// If this is set to 1, it is intended that you redirect stderr to tmp.txt e.g.:
//      ./bigint_test.out 2> tmp.txt
// The #define MEM_LEAK_CHK in bigint.c also needs to be set to 1.
#define MEM_LEAK_CHK 0
#define EXS 5

struct Bigint* b_0() {
    struct Bigint* a = Bigint_constructor();
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_constructor b_0 %li\n", (long) a);
    }
    Bigint_enqueue(a, 0);
    return a;
}

struct Bigint* b_1() {
    struct Bigint* a = Bigint_constructor();
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_constructor b_1 %li\n", (long) a);
    }
    Bigint_enqueue(a, 1);
    return a;
}

struct Bigint* b_2() {
    struct Bigint* a = Bigint_constructor();
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_constructor b_2 %li\n", (long) a);
    }
    Bigint_enqueue(a, (long) 37*41*43*47*53*59);
    return a;
}

struct Bigint* b_3() {
    struct Bigint* a = Bigint_constructor();
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_constructor b_3 %li\n", (long) a);
    }
    Bigint_enqueue(a, (long) 1 << 32);
    Bigint_enqueue(a, (long) 1 << 31);
    Bigint_enqueue(a, (long) 1 << 30);
    Bigint_enqueue(a, (long) 1 << 29);
    return a;
}

struct Bigint* b_4() {
    struct Bigint* a = Bigint_constructor();
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_constructor b_4 %li\n", (long) a);
    }
    Bigint_enqueue(a, (long) 1 << 32);
    Bigint_enqueue(a, 0);
    Bigint_enqueue(a, 0);
    Bigint_enqueue(a, 0);
    Bigint_enqueue(a, (long) 1 << 30);
    return a;
}

struct Bigint** b_n() {
    struct Bigint** bn = malloc(EXS * sizeof(struct Bigint*));
    bn[0] = b_0();
    bn[1] = b_1();
    bn[2] = b_2();
    bn[3] = b_3();
    bn[4] = b_4();
    return bn;
}

void arithmetic_checks() {
    int signs[8] = {1, 1, 1, -1, -1, 1, -1, -1};
    struct Bigint** bn = b_n();
    struct Bigint* a = NULL;
    struct Bigint* b = NULL;
    struct Bigint* c = NULL;
    struct Bigint* d = NULL;
    struct Bigint** dm = NULL;
    struct Bigint* zero = Bigint_from_long(0);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_from_long(0) top of arithmetic_checks %li\n", (long) zero);
    }
    long n;
    long bitshifts[5] = {0, 1, 30, 100, 300};
    for (long i=0; i<EXS; i++) {
        b = bn[i];
        printf("test Bigint #%li: ", i); Bigint_print(b); printf("\n");
        b -> sign = -1;
        printf("negated: "); Bigint_print(b); printf("\n");
        b -> sign = 1;
    }

    // deepcopy test
    printf("\n\nTest deepcopy:\n");
    a = bn[2];
    // Make 2 copies so that I don't destroy the actual array entry
    b = Bigint_deepcopy(a);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_deepcopy to make b in deepcopy test %li\n", (long) b);
    }
    c = Bigint_deepcopy(b);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_deepcopy to make c in deepcopy test %li\n", (long) c);
    }
    printf("b is deepcopy of bn[2], c is deepcopy of b.\n");
    printf("b is "); Bigint_print(b); printf(" at address %li\n", (long) b);
    printf("c is "); Bigint_print(c); printf(" at address %li\n", (long) c);
    printf("\nNow destroy b, see if c still exists.\n");
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_destructor on b in deepcopy test %li\n", (long) b);
    }
    b = Bigint_destructor(b);
    printf("b is "); Bigint_print(b); printf(" at address %li\n", (long) b);
    printf("c is "); Bigint_print(c); printf(" at address %li\n", (long) c);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_destructor on c in deepcopy test %li\n", (long) c);
    }
    c = Bigint_destructor(c);

    printf("\n\nTest addition:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        for (long j=i; j<EXS; j++) {
            b = Bigint_deepcopy(bn[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_deepcopy bn[%li] in addition test %li\n", 
                    j, (long) b);
            }
            for (long k=0; k<4; k++) {
                a -> sign = signs[2*k];
                b -> sign = signs[2*k + 1];
                c = Bigint_add(a, b);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_add in addition test %li\n", (long) c);
                }
                Bigint_print(a); printf(" + "); 
                Bigint_print(b); printf(" = ");
                Bigint_print(c); printf("\n");
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "Bigint_destructor in addition test %li\n", 
                        (long) c);
                }
                c = Bigint_destructor(c);
            }
            if (MEM_LEAK_CHK) {
                fprintf(stderr, "Bigint_destructor in addition test %li\n", 
                    (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest subtraction:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        for (long j=i; j<EXS; j++) {
            b = Bigint_deepcopy(bn[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_deepcopy bn[%li] in subtraction test %li\n", 
                    j, (long) b);
            }
            for (long k=0; k<4; k++) {
                a -> sign = signs[2*k];
                b -> sign = signs[2*k + 1];
                c = Bigint_subtract(a, b);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "Bigint_subtract in subtraction test %li\n", 
                        (long) c);
                }
                Bigint_print(a); printf(" - "); 
                Bigint_print(b); printf(" = ");
                Bigint_print(c); printf("\n");
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_destructor in subtraction test %li\n", 
                        (long) c);
                }
                c = Bigint_destructor(c);
            }
            if (MEM_LEAK_CHK) {
                fprintf(stderr, "Bigint_destructor in subtraction test %li\n", 
                    (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest multiplication:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        for (long j=i; j<EXS; j++) {
            b = Bigint_deepcopy(bn[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_deepcopy bn[%li] in multiplication test %li\n", 
                    j, (long) b);
            }
            for (long k=0; k<4; k++) {
                a -> sign = signs[2*k];
                b -> sign = signs[2*k + 1];
                c = Bigint_multiply(a, b);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_multiply in multiplication test %li\n", 
                        (long) c);
                }
                Bigint_print(a); printf(" * "); 
                Bigint_print(b); printf(" = ");
                Bigint_print(c); printf("\n");
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_destructor in multiplication test %li\n", 
                        (long) c);
                }
                c = Bigint_destructor(c);
            }
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_destructor in multiplication test %li\n", 
                    (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest divmod:\n");
    a = bn[0];
    b = bn[1];
    dm = Bigint_divmod(a, b);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "Bigint_divmod in divmod test %li\n", (long) dm);
        fprintf(stderr, "dm[0] in divmod test %li\n", (long) dm[0]);
        fprintf(stderr, "dm[1] in divmod test %li\n", (long) dm[1]);
    }
    printf("0 // 1 = "); Bigint_print(dm[0]); printf("\n");
    printf("0 %% 1 = "); Bigint_print(dm[1]); printf("\n");
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free(dm) in divmod test %li\n", (long) dm);
        fprintf(stderr, 
            "Bigint_destructor dm[0] in divmod test %li\n", (long) dm[0]);
        fprintf(stderr, 
            "Bigint_destructordm[1] in divmod test %li\n", (long) dm[1]);
    }
    Bigint_destructor(dm[0]);
    Bigint_destructor(dm[1]);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
    }
    free(dm); dm = NULL;
    // As of now, both of these result in a segfault.
    // a = bn[1];
    // b = bn[0];
    // dm = Bigint_divmod(a, b);
    // printf("1 // 0 = "); Bigint_print(dm[0]); printf("\n");
    // printf("1 %% 0 = "); Bigint_print(dm[1]); printf("\n");
    // a = bn[0];
    // b = bn[0];
    // dm = Bigint_divmod(a, b);
    // printf("0 // 0 = "); Bigint_print(dm[0]); printf("\n");
    // printf("0 %% 0 = "); Bigint_print(dm[1]); printf("\n");
    for (long i=1; i<EXS; i++) {
        a = bn[i];
        for (long j=i; j<EXS; j++) {
            b = Bigint_deepcopy(bn[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_deepcopy bn[%li] in divmod test %li\n", 
                    j, (long) b);
            }
            for (long k=0; k<4; k++) {
                a -> sign = signs[2*k];
                b -> sign = signs[2*k + 1];
                dm = Bigint_divmod(a, b);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in divmod test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in divmod test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in divmod test %li\n", (long) dm[1]);
                }
                Bigint_print(a); printf(" // "); 
                Bigint_print(b); printf(" = ");
                Bigint_print(dm[0]); printf("\n");
                Bigint_print(a); printf(" %% "); 
                Bigint_print(b); printf(" = ");
                Bigint_print(dm[1]); printf("\n");
                c = Bigint_multiply(dm[0], b);
                d = Bigint_add(dm[1], c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_multiply in divmod test %li\n", (long) c);
                    fprintf(stderr, 
                        "Bigint_add in divmod test %li\n", (long) d);
                }
                if (Bigint_equal(d, a)) {
                    printf("Passes check: a = bq + r\n");
                } else {
                    printf("ERROR: Fails check: a != bq + r\n");
                }
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_destructor in divmod test %li\n", (long) c);
                    fprintf(stderr, 
                        "Bigint_destructor in divmod test %li\n", (long) d);
                }
                c = Bigint_destructor(c);
                d = Bigint_destructor(d);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm) in divmod test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in divmod test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructordm[1] in divmod test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
                dm = Bigint_divmod(b, a);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in divmod test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in divmod test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in divmod test %li\n", (long) dm[1]);
                }
                Bigint_print(b); printf(" // "); 
                Bigint_print(a); printf(" = ");
                Bigint_print(dm[0]); printf("\n");
                Bigint_print(b); printf(" %% "); 
                Bigint_print(a); printf(" = ");
                Bigint_print(dm[1]); printf("\n");
                c = Bigint_multiply(dm[0], a);
                d = Bigint_add(dm[1], c);
                if (Bigint_equal(d, b)) {
                    printf("Passes check: b = aq + r\n");
                } else {
                    printf("ERROR: Fails check: b != ba + r\n");
                }
                c = Bigint_destructor(c);
                d = Bigint_destructor(d);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm) in divmod test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in divmod test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructordm[1] in divmod test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
            }
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_destructor in divmod test %li\n", 
                    (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest gcd:\n");
    // As of now, all three of these result in a segfault.
    // a = bn[0];
    // b = bn[1];
    // c = Bigint_gcd(a, b);
    // printf("gcd(0, 1) = "); Bigint_print(c); printf("\n");
    // a = bn[1];
    // b = bn[0];
    // c = Bigint_gcd(a, b);
    // printf("gcd(1, 0) = "); Bigint_print(c); printf("\n");
    // a = bn[0];
    // b = bn[0];
    // c = Bigint_gcd(a, b);
    // printf("gcd(0, 0) = "); Bigint_print(c); printf("\n");
    for (long i=1; i<EXS; i++) {
        a = bn[i];
        for (long j=i; j<EXS; j++) {
            b = Bigint_deepcopy(bn[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_deepcopy bn[%li] in gcd test %li\n", 
                    j, (long) b);
            }
            for (long k=0; k<4; k++) {
                a -> sign = signs[2*k];
                b -> sign = signs[2*k + 1];
                c = Bigint_gcd(a, b);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "Bigint_gcd in gcd test %li\n", (long) c);
                }
                printf("gcd("); Bigint_print(a); printf(", "); 
                Bigint_print(b); printf(") = ");
                Bigint_print(c); printf("\n");
                dm = Bigint_divmod(a, c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in gcd test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in gcd test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in gcd test %li\n", (long) dm[1]);
                }
                if (Bigint_equal(dm[1], zero)) {
                    printf("Passes check: a %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: a %% gcd(a, b) !=0\n");
                    printf("Got "); Bigint_print(c); printf(" instead.\n");
                }
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm)1 in gcd test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in gcd test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructordm[1] in gcd test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
                dm = Bigint_divmod(b, c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in gcd test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in gcd test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in gcd test %li\n", (long) dm[1]);
                }
                if (Bigint_equal(dm[1], zero)) {
                    printf("Passes check: b %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
                    printf("Got "); Bigint_print(c); printf(" instead.\n");
                }
                c = Bigint_destructor(c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm)2 in gcd test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in gcd test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructordm[1] in gcd test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
                c = Bigint_gcd(b, a);
                printf("gcd("); Bigint_print(b); printf(", "); 
                Bigint_print(a); printf(") = ");
                Bigint_print(c); printf("\n");
                dm = Bigint_divmod(a, c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in gcd test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in gcd test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in gcd test %li\n", (long) dm[1]);
                }
                if (Bigint_equal(dm[1], zero)) {
                    printf("Passes check: a %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: a %% gcd(a, b) != 0\n");
                    printf("Got "); Bigint_print(c); printf(" instead.\n");
                }
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm)3 in gcd test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in gcd test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructordm[1] in gcd test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
                dm = Bigint_divmod(b, c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "Bigint_divmod in gcd test %li\n", (long) dm);
                    fprintf(stderr, "dm[0] in gcd test %li\n", (long) dm[0]);
                    fprintf(stderr, "dm[1] in gcd test %li\n", (long) dm[1]);
                }
                if (Bigint_equal(dm[1], zero)) {
                    printf("Passes check: b %% gcd(a, b) == 0\n");
                } else {
                    printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
                    printf("Got "); Bigint_print(c); printf(" instead.\n");
                }
                c = Bigint_destructor(c);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, 
                        "free(dm)4 in gcd test %li\n", (long) dm);
                    fprintf(stderr, 
                        "Bigint_destructor dm[0] in gcd test %li\n", 
                        (long) dm[0]);
                    fprintf(stderr, 
                        "Bigint_destructor dm[1] in gcd test %li\n", 
                        (long) dm[1]);
                }
                dm[0] = Bigint_destructor(dm[0]);
                dm[1] = Bigint_destructor(dm[1]);
                if (MEM_LEAK_CHK) {
                    fprintf(stderr, "free Bigint_divmod %li\n", (long)dm);
                }
                free(dm); dm = NULL;
            }
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_destructor in gcd test %li\n", 
                    (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest intlog2:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        n = Bigint_intlog2(a);
        printf("Largest nonzero bit of "); Bigint_print(a); 
        printf(" = %li\n", n); 
        a -> sign = -1;
        n = Bigint_intlog2(a);
        printf("Largest nonzero bit of "); Bigint_print(a); 
        printf(" = %li\n", n); 
        a -> sign = 1;
    }

    printf("\n\nTest bitshift_left:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        for (long j=0; j<5; j++) {
            b = Bigint_bitshift_left(a, bitshifts[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_bitshift_left in bitshift_left test %li\n", 
                    (long) b);
            }
            Bigint_print(a); printf(" << %li = ", bitshifts[j]);
            Bigint_print(b); printf("\n");
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_destructor in bitshift_left test %li\n", (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    printf("\n\nTest bitshift_right:\n");
    for (long i=0; i<EXS; i++) {
        a = bn[i];
        for (long j=0; j<5; j++) {
            b = Bigint_bitshift_right(a, bitshifts[j]);
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_bitshift_right in bitshift_right test %li\n", 
                    (long) b);
            }
            Bigint_print(a); printf(" >> %li = ", bitshifts[j]);
            Bigint_print(b); printf("\n");
            if (MEM_LEAK_CHK) {
                fprintf(stderr, 
                    "Bigint_destructor in bitshift_right test %li\n", (long) b);
            }
            b = Bigint_destructor(b);
        }
    }

    for (long i=0; i<EXS; i++) {
        if (MEM_LEAK_CHK) {
            fprintf(stderr, 
                "Bigint_destructor of bn[%li] %li\n", i, (long) b);
        }
        bn[i] = Bigint_destructor(bn[i]);
    }
    if (MEM_LEAK_CHK) {
        fprintf(stderr, 
            "Bigint_destructor of zero %li\n", (long) b);
    }
    zero = Bigint_destructor(zero);
    free(bn); bn = NULL;
}    

int main() {
    arithmetic_checks();
}