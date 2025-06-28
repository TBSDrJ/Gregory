#include<stdbool.h>
#include"frac.h"

// Functions for Bigint
struct Bigint* construct_Bigint() {
    struct Bigint* b = malloc(sizeof(struct Bigint));
    b -> sign = 1;
    b -> head = NULL;
    b -> tail = NULL;
    b -> len = 0;
    return b;
}

struct Bigint* destruct_Bigint(struct Bigint* b) {
    struct Entry_long* e = b -> head;
    while (e) {e = destruct_entry_long(e);}
    b -> head = NULL;
    b -> tail = NULL;
    b -> len = 0;
    free(b);
    return NULL;
}

void enqueue_to_Bigint(struct Bigint* b, unsigned long n) {
    struct Entry_long* e = construct_entry_long(n);
    if (b -> tail) {
        b -> tail -> next = e;
        b -> tail = e;
        b -> len++;
    } else {
        b -> head = e;
        b -> tail = e;
        b -> len = 1;
    }
}

void print_Bigint(struct Bigint* b) {
    if (!b) {
        printf("Trying to print empty Bigint");
        return;
    }
    struct Entry_long* e = b -> head;
    while(e) {
        printf("%lu, ", e -> content);
        e = e -> next;
    }
}

struct Bigint* add_Bigints(struct Bigint* a, struct Bigint* b) {
    bool fail = false;
    if (!a || !(a -> head)) {
        printf("ERROR: Trying to add empty Bigint, a from add(a,b)\n");
        fail = true;
    } 
    if (!b || !(b -> head)) {
        printf("ERROR: Trying to add empty Bigint, b from add(a,b)\n");
        fail = true;
    }
    if (fail) {
        return NULL;
    }
    if ((a -> sign == -1) && (b -> sign == 1)) {
        struct Bigint* neg_a = neg_Bigint(a);
        // return subtract_Bigints(b, neg_a);
        return NULL;
    } else if ((a -> sign == 1) && (b -> sign == -1)) {
        struct Bigint* neg_b = neg_Bigint(b);
        // return subtract_Bigints(a, neg_b);
        return NULL;
    } 
    struct Bigint* c = malloc(sizeof(struct Bigint));
    if ((a -> sign == -1) && (b -> sign == -1)) {
        c -> sign = -1;
    }
    // We are now set up so that everything works as if a, b, c are positive.
    struct Entry_long* entry_a = a -> head;
    struct Entry_long* entry_b = b -> head;
    unsigned long a_digit;
    unsigned long b_digit;
    // Keep track of top digits to check for and deal with overflow
    unsigned long sum = 0, top_a, top_b, top_sum, tops;
    while ((entry_a) || (entry_b)) {
        if (entry_a) {
            // Separate top digit from rest of number
            a_digit = entry_a -> content;
            top_a = a_digit >> (sizeof(long)*8 - 1);
            a_digit = a_digit << 1 >> 1;
            entry_a = entry_a -> next;
        } else {
            a_digit = 0;
            top_a = 0;
        }
        if (entry_b) {
            b_digit = entry_b -> content;            
            top_b = b_digit >> (sizeof(long)*8 - 1);
            b_digit = b_digit << 1 >> 1;
            entry_b = entry_b -> next;
        } else {
            b_digit = 0;
            top_b = 0;
        }
        sum += a_digit + b_digit;
        top_sum = sum >> (sizeof(unsigned long)*8 - 1);
        tops = top_a + top_b + top_sum;
        if (tops > 1) {
            if (tops == 2) {
                // In this case, we have a 1 in the highest bit, but that 1 is
                //    already accounted for in tops, so drop it.
                sum = sum << 1 >> 1;
            }
            // if (tops == 3), we need 1s in both the highest and 1st place of
            //    the next 'digit' we leave the 1 that is in the highest place.
            enqueue_to_Bigint(c, sum);
            // Start the next sum with the 1 that regroups up to the 1st place
            //    of the next 'digit'
            sum = 1;
        } else {
            sum += tops << (sizeof(unsigned long)*8 - 1);
            enqueue_to_Bigint(c, sum);
            sum = 0;
        }
    }
    if (sum == 1) {
        enqueue_to_Bigint(c, 1);
    }
    return c;
}

struct Bigint* neg_Bigint(struct Bigint* a) {
    struct Bigint* c = malloc(sizeof(struct Bigint));
    c -> sign = -1 * (a -> sign);
    struct Entry_long* e = a -> head;
    while (e) {
        enqueue_to_Bigint(c, e -> content);
        e = e -> next;
    }
    return c;
}

struct Bigint* multiply_Bigints(struct Bigint* a, struct Bigint* b) {
       bool fail = false;
    if (!a || !(a -> head)) {
        printf("ERROR: Trying to multiply empty Bigint, a from mul(a,b)\n");
        fail = true;
    } 
    if (!b || !(b -> head)) {
        printf("ERROR: Trying to multiply empty Bigint, b from mul(a,b)\n");
        fail = true;
    }
    if (fail) {
        return NULL;
    } 
    // So we know that a and b are not NULL and have at least one entry each.
    struct Bigint* c = malloc(sizeof(struct Bigint));
    c -> sign = (a -> sign) * (b -> sign);
    enqueue_to_Bigint(c, 0);
    /* The idea is that (n bits)*(n bits) has <= 2n bits.  So, break a and b
    in half so that each product of two halves fits inside the data type. 
    Do the 4 different multiplications of the halves, adjust them for place 
    value, and add those up. */
    unsigned long a_0, a_1, b_0, b_1, prod_0, prod_1, prod_2, prod_3;
    unsigned long places_a = 0, places_b;
    struct Bigint* c_0;
    struct Bigint* c_1;
    struct Bigint* c_2;
    struct Bigint* c_3;
    struct Entry_long* entry_a = a -> head;
    struct Entry_long* entry_b = b -> head;
    unsigned long digit_a = 0, digit_b = 0;
    while (entry_a) {
        digit_a = entry_a -> content;
        places_b = 0;
        while (entry_b) {
            c_0 = construct_Bigint();
            c_1 = construct_Bigint();
            c_2 = construct_Bigint();
            c_3 = construct_Bigint();
            digit_b = entry_b -> content;
            a_0 = digit_a << sizeof(long)*4 >> sizeof(long)*4;
            a_1 = digit_a >> sizeof(long)*4;
            b_0 = digit_a << sizeof(long)*4 >> sizeof(long)*4;
            b_1 = digit_a >> sizeof(long)*4;
            prod_0 = a_0 * b_0;
            prod_1 = a_0 * b_1;
            prod_2 = a_1 * b_0;
            prod_3 = a_1 * b_1;
            enqueue_to_Bigint(c_0, prod_0);
            c_0 = bitshift_left_Bigint(c_0, places_a + places_b);
            enqueue_to_Bigint(c_1, prod_1);
            c_1 = bitshift_left_Bigint(
                    c_1, sizeof(long)*4 + places_a + places_b);
            enqueue_to_Bigint(c_2, prod_2);
            c_2 = bitshift_left_Bigint(
                    c_2, sizeof(long)*4 + places_a + places_b);
            enqueue_to_Bigint(c_3, prod_3);
            c_3 = bitshift_left_Bigint(
                    c_3, sizeof(long)*8 + places_a + places_b);
            c = add_Bigints(c, c_0);
            c = add_Bigints(c, c_1);
            c = add_Bigints(c, c_2);
            c = add_Bigints(c, c_3);
            entry_b = entry_b -> next;
            places_b += sizeof(long)*8;
            c_0 = destruct_Bigint(c_0);
            c_1 = destruct_Bigint(c_1);
            c_2 = destruct_Bigint(c_2);
            c_3 = destruct_Bigint(c_3);
        }
        entry_a = entry_a -> next;
        places_a += sizeof(long)*8;
        entry_b = b -> head;
    }
    return c;
}

struct Bigint* bitshift_left_Bigint(struct Bigint* a, unsigned long n) {
    struct Bigint* b = construct_Bigint();
    if (!a || (!(a -> head))) {
        printf("ERROR: Attempting to left bitshift empty Bigint\n");
        return NULL;
    }
    struct Entry_long* e = a -> head;
    unsigned long digits = n/64, bits = n%64;
    unsigned long n_0, n_1 = 0;
    for (long i=0; i<digits; i++) {
        enqueue_to_Bigint(b, 0);
    }
    while (e) {
        n_0 = (e -> content) << bits;
        // n_0 should have all 0s wherever n_1 could have 1s and vice versa.
        n_0 |= n_1;
        // if bits == 0, no shift happens because the number of places is equal
        //   to the size of the data type.
        if (bits > 0) {
            n_1 = ((e -> content) >> (sizeof(long)*8 - bits));
        } else {
            n_1 = 0;
        }
        enqueue_to_Bigint(b, n_0);
        e = e -> next;
    }
    if (n_1) {
        enqueue_to_Bigint(b, n_1);
    }
    return b;
}

struct Bigint* bitshift_right_Bigint(struct Bigint* a, unsigned long n) {
    struct Bigint* b = malloc(sizeof(struct Bigint));
    if (!a || (!(a -> head))) {
        printf("ERROR: Attempting to right bitshift empty Bigint\n");
        return NULL;
    }
    struct Entry_long* e = a -> head;
    unsigned long digits = n/64, bits = n%64;
    unsigned long n_0, n_1 = 0;
    for (long i=0; i<digits; i++) {
        e = e -> next;
    }
    while (e) {
        n_0 = (e -> content) >> bits;
        e = e -> next;
        if (e) {
            n_1 = (e -> content) << (sizeof(long)*8 - bits);
        } else {
            n_1 = 0;
        }
        enqueue_to_Bigint(b, n_0 | n_1);
    }
    if (!(b -> head)) {
        // Make sure b is certain to be nonempty.
        enqueue_to_Bigint(b, 0);
    }
    return b;
}

// Functions for Entry_long
struct Entry_long* construct_entry_long(unsigned long n) {
    struct Entry_long* e = malloc(sizeof(struct Entry_long));
    e -> content = n;
    e -> next = NULL;
    return e;
}

struct Entry_long* destruct_entry_long(struct Entry_long* e) {
    struct Entry_long* next = e -> next;
    if (e) {free(e);}
    e = NULL;
    return next;
}

// Functions for Fraction
struct Fraction* construct_frac() {
    struct Bigint* numerator = construct_Bigint();
    struct Bigint* denominator = construct_Bigint();
    struct Fraction* f = malloc(sizeof(struct Fraction));
    f -> numerator = numerator;
    f -> denominator = denominator;
    return f;
}

int main() {
    struct Bigint* a = construct_Bigint();
    struct Bigint* b = construct_Bigint();
    unsigned long n = 37;
    enqueue_to_Bigint(a, 0);
    enqueue_to_Bigint(a, n);
    enqueue_to_Bigint(b, n);
    enqueue_to_Bigint(b, n);
    struct Bigint* c = multiply_Bigints(a, b);
    printf("a: "); print_Bigint(a); printf("\n");
    printf("b: "); print_Bigint(b); printf("\n");
    printf("a*b: "); print_Bigint(c); printf("\n");
    if (a) {a = destruct_Bigint(a);}
    if (b) {b = destruct_Bigint(b);}
    if (c) {c = destruct_Bigint(c);}
}