#include<stdio.h>
#include<stdlib.h>
#include"polynomials.h"
#include"myint.h"

#define EXS 4

struct Polynomial* p_0() {
    struct Polynomial* p = Polynomial_constructor(0);
    return p;
}

struct Polynomial* p_1() {
    struct Polynomial* p = Polynomial_constructor(0);
    p -> coeffs[0] -> my_long = 1;
    return p;
}

struct Polynomial* p_2() {
    struct Polynomial* p = Polynomial_constructor(5);
    long sign = -1;
    for (long i=0; i<=5; i++) {
        p -> coeffs[i] = Myint_constructor();
        p -> coeffs[i] -> my_long = i;
        p -> coeffs[i] -> sign = sign;
        sign *= -1;
    }
    return p;
}

struct Polynomial* p_3() {
    struct Polynomial* p = Polynomial_constructor(2);
    p -> coeffs[0] -> my_long = 1;
    p -> coeffs[0] -> sign = -1;
    p -> coeffs[1] -> my_long = 4;
    p -> coeffs[1] -> sign = -1;
    struct Myint* tmp = p -> coeffs[1];
    struct Myint* a = Myint_constructor(); a -> my_long = (long) 1 << 63;
    p -> coeffs[1] = Myint_multiply(p -> coeffs[1], a);
    tmp = Myint_destructor(tmp);
    p -> coeffs[2] -> my_long = 8;
    tmp = p -> coeffs[2];
    p -> coeffs[2] = Myint_multiply(p -> coeffs[2], a);
    tmp = Myint_destructor(tmp);
    return p;
}

struct Polynomial** p_n() {
    struct Polynomial** pn = malloc(EXS*sizeof(struct Polynomial*));
    pn[0] = p_0();
    pn[1] = p_1();
    pn[2] = p_2();
    pn[3] = p_3();
    return pn;
}

void test_Polynomial_constructor() {
    printf("\nTesting Polynomial_constructor(): \n");
    printf("Expected: \n");
    printf("-0 1 -2 3 -4 5\n");

    printf("Actual: \n");
    struct Polynomial* p = p_2();
    for (long i=0; i<= 5; i++) {
        Myint_print(p -> coeffs[i]); printf(" ");
    }
    printf("\n");
    p = Polynomial_destructor(p);
}

void test_Polynomial_print() {
    printf("\nTesting Polynomial_print(): \n");
    printf("Expected: \n");
    printf("p: 0\n");
    printf("p: 1\n");
    printf("p: 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0  \n");
    printf("p: (0, 4)x^2 + (-0, 2)x + -1\n");

    printf("Actual: \n");
    struct Polynomial** pn = p_n();
    for (int i=0; i<EXS; i++) {
        printf("p: "); Polynomial_print(pn[i]); printf("\n");
        pn[i] = Polynomial_destructor(pn[i]);
    }
    free(pn);
}

void test_Polynomial_reduce() {
    printf("\nTesting Polynomial_reduce(): \n");
    printf("Expected: \n");
    for (int i=1; i<9; i++) {
        printf("p before reduce: ");
        for (int j=i; j>1; j--) {
            printf("0x^%d + ", j);
        }
        printf("0x + 1\n");
        printf("p after reduce: ");
        printf("1\n");
    }
    printf("p before reduce: ");
    printf("0x^9 + 0x^8 + 0x^7 + 0x^6 + 1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 1\n");
    printf("p after reduce: 1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 1\n");

    printf("Actual: \n");
    struct Polynomial* p;
    for (int i=1; i<10; i++) {
        p = Polynomial_constructor(i);
        p -> coeffs[0] = Myint_constructor();
        p -> coeffs[0] -> my_long = 1;
        for (int j=1; j<=i; j++) {
            p -> coeffs[j] = Myint_constructor();
        }
        if (i == 9) {
            p -> coeffs[5] -> my_long = 1;
        }
        printf("p before reduce: ");
        Polynomial_print(p); printf("\n");
        Polynomial_reduce(p);
        printf("p after reduce: "); 
        Polynomial_print(p); printf("\n");
    }
}

void test_Polynomial_neg() {
    printf("\nTesting Polynomial_neg(): \n");
    printf("Expected: \n");
    printf("p: 0 \n");
    printf("-p: -0 \n");
    printf("p: 1 \n");
    printf("-p: -1 \n");
    printf("p: 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0  \n");
    printf("-p: -5x^5 + 4x^4 + -3x^3 + 2x^2 + -1x + 0  \n");
    printf("p: (0, 4)x^2 + (-0, 2)x + -1 \n");
    printf("-p: (-0, 4)x^2 + (0, 2)x + 1 \n");

    printf("Actual: \n");
    struct Polynomial** pn = p_n();
    struct Polynomial* neg_p;
    for (int i=0; i<EXS; i++) {
        neg_p = Polynomial_neg(pn[i]);
        printf("p: "); Polynomial_print(pn[i]); printf("\n");
        printf("-p: "); Polynomial_print(neg_p); printf("\n");
        pn[i] = Polynomial_destructor(pn[i]);
        neg_p = Polynomial_destructor(neg_p);
    }
    free(pn);
}

void test_Polynomial_der() {
    printf("\nTesting Polynomial_der(): \n");
    printf("Expected: \n");
    printf("p: 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0  \n");
    printf("dp/dx: 25x^4 + -16x^3 + 9x^2 + -4x + 1  \n");
    printf("p: 0 \n");
    printf("dp/dx: 0 \n");
    printf("p: 1 \n");
    printf("dp/dx: 0 \n");
    printf("p: (0, 4)x^2 + (-0, 2)x + -1 \n");
    printf("dp/dx: (0, 8)x^2 + (-0, 2) \n");

    printf("Actual: \n");
    struct Polynomial** pn = p_n();
    struct Polynomial* der_p;
    for (int i=0; i<EXS; i++) {
        der_p = Polynomial_der(pn[i]);
        printf("p: "); Polynomial_print(pn[i]); printf("\n");
        printf("dp/dx: "); Polynomial_print(der_p); printf("\n");
        pn[i] = Polynomial_destructor(pn[i]);
        der_p = Polynomial_destructor(der_p);
    }
    free(pn);
}

void test_Polynomial_subs() {
    printf("\nTesting Polynomial_der(): \n");
    printf("Expected: \n");
    printf("p: 0 \n");
    printf("p(0): 0 \n");
    printf("p(2): 0 \n");
    printf("p(-2): 0 \n");
    printf("p: 1 \n");
    printf("p(0): 1 \n");
    printf("p(2): 1 \n");
    printf("p(-2): 1 \n");
    printf("p: 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0  \n");
    printf("p(0): -0  \n");
    printf("p(2): 114 \n");
    printf("p(-2): -258 \n");
    printf("p: (0, 4)x^2 + (-0, 2)x + -1 \n");
    printf("p(0): -1 \n");
    printf("p(2): 18446744073709551615, 11 Note: = (0, 12) - 1 \n"); 
    printf("p(-2): 18446744073709551615, 19 Note: = (0, 20) - 1 \n");

    printf("Actual: \n");
    struct Polynomial** pn = p_n();
    struct Myint* a;

    for (int i=0; i<EXS; i++) {
        printf("p: "); Polynomial_print(pn[i]); printf("\n");
        a = Polynomial_subs(pn[i], 0);
        printf("p(0): "); Myint_print(a); printf("\n");
        a = Myint_destructor(a);
        a = Polynomial_subs(pn[i], 2);
        printf("p(2): "); Myint_print(a); printf("\n");
        a = Myint_destructor(a);
        a = Polynomial_subs(pn[i], -2);
        printf("p(-2): "); Myint_print(a); printf("\n");
        a = Myint_destructor(a);
        pn[i] = Polynomial_destructor(pn[i]);
    }
    free(pn);
}

void test_Polynomial_add() {
    printf("\nTesting Polynomial_add(): \n");
    printf("Expected:\n");
    printf("0 + 0 = 0\n");
    printf("0 + 1 = 1\n");
    printf("0 + 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0 = ");
        printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + 0\n");
    printf("0 + (0, 4)x^2 + (-0, 2)x + -1 = (0, 4)x^2 + (-0, 2)x + -1\n");
    printf("1 + 1 = 2\n");
    printf("1 + 5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0 = ");
        printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + 1");
    printf("1 + (0, 4)x^2 + (-0, 2)x + -1 = (0, 4)x^2 + (-0, 2)x + 0\n");
    printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0 + ");
        printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0 = ");
        printf("10x^5 + -8x^4 + 6x^3 + -4x^2 + 2x + -0\n");
    printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0 + ");
        printf("(0, 4)x^2 + (-0, 2)x + -1 = ");
        printf("5x^5 + -4x^4 + 3x^3 + (18446744073709551614, 3)x^2 ");
        printf("+ (-18446744073709551615, 1)x + -1\n");
    printf("(0, 4)x^2 + (-0, 2)x + -1 + (0, 4)x^2 + (-0, 2)x + -1 = ");
        printf("(0, 8)x^2 + (-0, 4)x + -2\n");
    
    printf("\nActual:\n");
    struct Polynomial** pn = p_n();
    struct Polynomial* p;

    for (int i=0; i<EXS; i++) {
        for (int j=i; j<EXS; j++) {
            Polynomial_print(pn[i]); printf(" + "); 
            Polynomial_print(pn[j]); printf(" = ");
            p = Polynomial_add(pn[i], pn[j]);
            Polynomial_print(p); printf("\n");
            p = Polynomial_destructor(p);
        }
    }
}

void test_Polynomial_multiply() {
    printf("\nTesting Polynomial_add(): \n");
    printf("Expected:\n");
    printf("0 * 0 = 0\n");
    printf("0 * 1 = 0\n");
    printf("0 * (5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0) = 0\n");
    printf("0 * ((0, 4)x^2 + (-0, 2)x + -1) = 0\n");
    printf("1 * 1 = 1\n");
    printf("1 * (5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0) = ");
        printf("5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0\n");
    printf("1 * (0, 4)x^2 + (-0, 2)x + -1 = (0, 4)x^2 + (-0, 2)x + -1\n");
    printf("(5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0) * ");
        printf("(5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0) = ");
        printf("25x^10 + -40x^9 + 46x^8 + -44x^7 + 35x^6 + -20x^5 + 10x^4 ");
        printf("+ -4x^3 + 1x^2 + 0x + 0\n");
    printf("(5x^5 + -4x^4 + 3x^3 + -2x^2 + 1x + -0) * ");
        printf("((0, 4)x^2 + (-0, 2)x + -1) = ");
        printf("(0, 20)x^7 + (-0, 26)x^6 + (18446744073709551611, 19)x^5 + ");
        printf("(-18446744073709551612, 13)x^4 + ");
        printf("(18446744073709551613, 7)x^3 ");
        printf("(-18446744073709551614, 1)x^2 + -1x + 0\n");
    printf("((0, 4)x^2 + (-0, 2)x + -1) * ((0, 4)x^2 + (-0, 2)x + -1) = ");
        printf("(0, 0, 16)x^4 + (-0, 0, 16)x^3 + ");
        printf("(0, 18446744073709551608, 3)x^2 + (0, 4)x + 1\n");
    
    printf("\nActual:\n");
    struct Polynomial** pn = p_n();
    struct Polynomial* p;

    for (int i=0; i<EXS; i++) {
        for (int j=i; j<EXS; j++) {
            printf("("); Polynomial_print(pn[i]); printf(") * ("); 
            Polynomial_print(pn[j]); printf(") = ");
            p = Polynomial_multiply(pn[i], pn[j]);
            Polynomial_print(p); printf("\n");
            p = Polynomial_destructor(p);
        }
    }
} 

int main() {
    // test_Polynomial_constructor();
    // test_Polynomial_print();
    // test_Polynomial_reduce();
    // test_Polynomial_neg();
    // test_Polynomial_der();
    // test_Polynomial_subs();
    // test_Polynomial_add();
    test_Polynomial_multiply();
    printf("\n\n\n");
}