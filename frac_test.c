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
    struct Myint* n = Myint_constructor();
    struct Myint* d = Myint_from_long(1);
    struct Fraction* a = Fraction_constructor();
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_1() {
    struct Myint* n = Myint_from_long(1);
    struct Myint* d = Myint_from_long(5);
    struct Fraction* a = Fraction_constructor();
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_2() {
    struct Myint* n = Myint_from_long(11);
    struct Myint* d = Myint_from_long(7);
    struct Fraction* a = Fraction_constructor();
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction* f_3() {
    struct Myint* n = Myint_from_long((long) 23*29*31*61*67*71);
    struct Myint* d = Myint_from_long((long) 37*41*43*47*53*59);
    struct Fraction* a = Fraction_constructor();
    a -> numerator = n;
    a -> denominator = d;
    return a;
}


struct Fraction* f_4() {
    struct Myint* n = Myint_from_long((long) 47*53*59*61*67*71);
    struct Myint* d = Myint_from_long((long) 37*41*43*73*79*83);
    struct Fraction* a = Fraction_constructor();
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
    struct Fraction* a = Fraction_constructor();
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
    struct Fraction* a = Fraction_constructor();
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
    struct Fraction* a = Fraction_constructor();
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
    struct Fraction* a = Fraction_constructor();
    a -> numerator = n;
    a -> denominator = d;
    return a;
}

struct Fraction** f_n() {
    struct Fraction** fn = malloc(EXS * sizeof(struct Fraction*));
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
    char signs[8] = {1, 1, 1, -1, -1, 1, -1, -1};
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

//     printf("\n\nTest addition:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         for (long j=i; j<EXS; j++) {
//             b = Myint_deepcopy(mn[j]);
//             for (long k=0; k<4; k++) {
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//                 c = Myint_add(a, b);
//                 Myint_print(a); printf(" + "); 
//                 Myint_print(b); printf(" = ");
//                 Myint_print(c); printf("\n");
//                 c = Myint_destructor(c);
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//             }
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest subtraction:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         for (long j=i; j<EXS; j++) {
//             b = Myint_deepcopy(mn[j]);
//             for (long k=0; k<4; k++) {
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//                 c = Myint_subtract(a, b);
//                 Myint_print(a); printf(" - "); 
//                 Myint_print(b); printf(" = ");
//                 Myint_print(c); printf("\n");
//                 c = Myint_destructor(c);
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//             }
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest multiplication:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         for (long j=i; j<EXS; j++) {
//             b = Myint_deepcopy(mn[j]);
//             for (long k=0; k<4; k++) {
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//                 c = Myint_multiply(a, b);
//                 Myint_print(a); printf(" * "); 
//                 Myint_print(b); printf(" = ");
//                 Myint_print(c); printf("\n");
//                 c = Myint_destructor(c);
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//             }
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest divmod:\n");
//     a = mn[0];
//     b = mn[1];
//     dm = Myint_divmod(a, b);
//     printf("0 // 1 = "); Myint_print(dm[0]); printf("\n");
//     printf("0 %% 1 = "); Myint_print(dm[1]); printf("\n");
//     free(dm); dm = NULL;
//     // Interestingly, C evaluates n/0 and n%0 and returns 0, even when n=0
//     // Makes for a lot fewer crashes, but is mathematically problematic.
//     a = mn[1];
//     b = mn[0];
//     dm = Myint_divmod(a, b);
//     printf("1 // 0 = "); Myint_print(dm[0]); printf("\n");
//     printf("1 %% 0 = "); Myint_print(dm[1]); printf("\n");
//     free(dm); dm = NULL;
//     a = mn[0];
//     b = mn[0];
//     dm = Myint_divmod(a, b);
//     printf("0 // 0 = "); Myint_print(dm[0]); printf("\n");
//     printf("0 %% 0 = "); Myint_print(dm[1]); printf("\n");
//     free(dm); dm = NULL;
//     for (long i=1; i<EXS; i++) {
//         a = mn[i];
//         for (long j=i; j<EXS; j++) {
//             b = Myint_deepcopy(mn[j]);
//             for (long k=0; k<4; k++) {
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//                 dm = Myint_divmod(a, b);
//                 Myint_print(a); printf(" // "); 
//                 Myint_print(b); printf(" = ");
//                 Myint_print(dm[0]); printf("\n");
//                 Myint_print(a); printf(" %% "); 
//                 Myint_print(b); printf(" = ");
//                 Myint_print(dm[1]); printf("\n");
//                 c = Myint_multiply(dm[0], a);
//                 d = Myint_add(dm[1], c);
//                 if (Myint_equal(d, a)) {
//                     printf("Passes check: a = bq + r\n");
//                 } else {
//                     printf("ERROR: Fails check: a != bq + r\n");
//                 }
//                 c = Myint_destructor(c);
//                 d = Myint_destructor(d);
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 dm = Myint_divmod(b, a);
//                 Myint_print(b); printf(" // "); 
//                 Myint_print(a); printf(" = ");
//                 Myint_print(dm[0]); printf("\n");
//                 Myint_print(b); printf(" %% "); 
//                 Myint_print(a); printf(" = ");
//                 Myint_print(dm[1]); printf("\n");
//                 c = Myint_multiply(dm[0], a);
//                 d = Myint_add(dm[1], c);
//                 if (Myint_equal(d, b)) {
//                     printf("Passes check: b = aq + r\n");
//                 } else {
//                     printf("ERROR: Fails check: b != aq + r\n");
//                 }
//                 c = Myint_destructor(c);
//                 d = Myint_destructor(d);
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//             }
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest gcd:\n");
//     // These are similar to the divmod results dividing by zero: they don't 
//     //      crash, but are mathematically problematic.
//     a = mn[0];
//     b = mn[1];
//     c = Myint_gcd(a, b);
//     printf("gcd(0, 1) = "); Myint_print(c); printf("\n");
//     a = mn[1];
//     b = mn[0];
//     c = Myint_gcd(a, b);
//     printf("gcd(1, 0) = "); Myint_print(c); printf("\n");
//     a = mn[0];
//     b = mn[0];
//     c = Myint_gcd(a, b);
//     printf("gcd(0, 0) = "); Myint_print(c); printf("\n");
//     for (long i=1; i<EXS; i++) {
//         a = mn[i];
//         for (long j=i; j<EXS; j++) {
//             b = Myint_deepcopy(mn[j]);
//             for (long k=0; k<4; k++) {
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//                 c = Myint_gcd(a, b);
//                 printf("gcd("); Myint_print(a); printf(", "); 
//                 Myint_print(b); printf(") = ");
//                 Myint_print(c); printf("\n");
//                 dm = Myint_divmod(a, c);
//                 if (Myint_equal(dm[1], zero)) {
//                     printf("Passes check: a %% gcd(a, b) == 0\n");
//                 } else {
//                     printf("ERROR: Fails check: a %% gcd(a, b) !=0\n");
//                     printf("Got "); Myint_print(c); printf(" instead.\n");
//                 }
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 dm = Myint_divmod(b, c);
//                 if (Myint_equal(dm[1], zero)) {
//                     printf("Passes check: b %% gcd(a, b) == 0\n");
//                 } else {
//                     printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
//                     printf("Got "); Myint_print(c); printf(" instead.\n");
//                 }
//                 c = Myint_destructor(c);
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 c = Myint_gcd(b, a);
//                 printf("gcd("); Myint_print(b); printf(", "); 
//                 Myint_print(a); printf(") = ");
//                 Myint_print(c); printf("\n");
//                 dm = Myint_divmod(a, c);
//                 if (Myint_equal(dm[1], zero)) {
//                     printf("Passes check: a %% gcd(a, b) == 0\n");
//                 } else {
//                     printf("ERROR: Fails check: a %% gcd(a, b) != 0\n");
//                     printf("Got "); Myint_print(c); printf(" instead.\n");
//                 }
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 dm = Myint_divmod(b, c);
//                 if (Myint_equal(dm[1], zero)) {
//                     printf("Passes check: b %% gcd(a, b) == 0\n");
//                 } else {
//                     printf("ERROR: Fails check: b %% gcd(a, b) !=0\n");
//                     printf("Got "); Myint_print(c); printf(" instead.\n");
//                 }
//                 c = Myint_destructor(c);
//                 dm[0] = Myint_destructor(dm[0]);
//                 dm[1] = Myint_destructor(dm[1]);
//                 free(dm); dm = NULL;
//                 if (signs[2*k] < 0) {
//                     Myint_neg(a);
//                 }
//                 if (signs[2*k + 1] < 0) {
//                     Myint_neg(b);
//                 }
//             }
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest intlog2:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         n = Myint_intlog2(a);
//         printf("Largest nonzero bit of "); Myint_print(a); 
//         printf(" = %li\n", n); 
//         Myint_neg(a);
//         n = Myint_intlog2(a);
//         printf("Largest nonzero bit of "); Myint_print(a); 
//         printf(" = %li\n", n); 
//         Myint_neg(a);
//     }

//     printf("\n\nTest bitshift_left:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         for (long j=0; j<5; j++) {
//             b = Myint_bitshift_left(a, bitshifts[j]);
//             Myint_print(a); printf(" << %li = ", bitshifts[j]);
//             Myint_print(b); printf("\n");
//             b = Myint_destructor(b);
//         }
//     }

//     printf("\n\nTest bitshift_right:\n");
//     for (long i=0; i<EXS; i++) {
//         a = mn[i];
//         for (long j=0; j<5; j++) {
//             b = Myint_bitshift_right(a, bitshifts[j]);
//             Myint_print(a); printf(" >> %li = ", bitshifts[j]);
//             Myint_print(b); printf("\n");
//             b = Myint_destructor(b);
//         }
//     }

}    

int main() {
    arithmetic_checks();
}