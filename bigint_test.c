#include"bigint.h"

// If this is set to true, it is intended that you redirect output to tmp.txt 
//      ./bigint_test.out > tmp.txt
#define MEM_LEAK_CHK 1

void memory_leak_checks() {
    struct Bigint* a = Bigint_constructor();
    Bigint_enqueue(a, (long) 1 << 32);
    Bigint_enqueue(a, (long) 1 << 31);
    Bigint_enqueue(a, (long) 1 << 30);
    Bigint_enqueue(a, (long) 1 << 29);
    struct Bigint* b = Bigint_deepcopy(a);
    struct Bigint* c = Bigint_add(a, b);
    struct Bigint* d = Bigint_multiply(a, b);
    struct Bigint* e = Bigint_subtract(d, c);
    struct Bigint* f = Bigint_gcd(d, e);
    struct Bigint** dm = Bigint_divmod(d, e);
    struct Bigint* g = dm[0];
    struct Bigint* h = dm[1];
    struct Bigint* i = Bigint_constructor();
    Bigint_enqueue(i, (long) 37*41*43*47*53*59);
    struct Bigint* j = Bigint_gcd(d, i);
    struct Bigint* k = Bigint_bitshift_left(d, 100);
    struct Bigint* l = Bigint_bitshift_right(d, 100);
    printf("a: "); Bigint_print(a); printf("\n");
    printf("b: "); Bigint_print(b); printf("\n");
    printf("c: "); Bigint_print(c); printf("\n");
    printf("d: "); Bigint_print(d); printf("\n");
    printf("e: "); Bigint_print(e); printf("\n");
    printf("f: "); Bigint_print(f); printf("\n");
    printf("g: "); Bigint_print(g); printf("\n");
    printf("h: "); Bigint_print(h); printf("\n");
    printf("i: "); Bigint_print(i); printf("\n");
    printf("j: "); Bigint_print(j); printf("\n");
    printf("k: "); Bigint_print(k); printf("\n");
    printf("l: "); Bigint_print(l); printf("\n");
    a = Bigint_destructor(a);
    b = Bigint_destructor(b);
    c = Bigint_destructor(c);
    d = Bigint_destructor(d);
    e = Bigint_destructor(e);
    f = Bigint_destructor(f);
    g = Bigint_destructor(g);
    h = Bigint_destructor(h);
    i = Bigint_destructor(i);
    j = Bigint_destructor(j);
    k = Bigint_destructor(k);
    l = Bigint_destructor(l);
    if (MEM_LEAK_CHK) {
        printf("free dm-from-Bigint_divmod-test %li\n", (long) dm);
    }
    free(dm); dm = NULL;
}

void arithmetic_checks() {
    
}

int main() {
    memory_leak_checks();
}