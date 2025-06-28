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

void destruct_Bigint(struct Bigint* b) {
    struct Entry_long* e = b -> head;
    while (e) {e = destruct_entry_long(e);}
    free(b);
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
        return;
    }
    struct Entry_long* e = b -> head;
    while(e) {
        printf("%lu, ", e -> content);
        e = e -> next;
    }
    printf("\n");
}

struct Bigint* add_Bigints(struct Bigint* a, struct Bigint* b) {
    bool fail = false;
    if (!(a -> head)) {
        printf("ERROR: Trying to add empty Bigint, a from add(a,b)\n");
        fail = true;
    } 
    if (!(b -> head)) {
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
            top_a = a_digit >> 63;
            a_digit = a_digit << 1 >> 1;
            entry_a = entry_a -> next;
        } else {
            a_digit = 0;
            top_a = 0;
        }
        if (entry_b) {
            b_digit = entry_b -> content;            
            top_b = b_digit >> 63;
            b_digit = b_digit << 1 >> 1;
            entry_b = entry_b -> next;
        } else {
            b_digit = 0;
            top_b = 0;
        }
        printf("%lu %lu %lu ", a_digit, b_digit, sum);
        sum += a_digit + b_digit;
        printf("%lu ", sum);
        top_sum = sum >> 63;
        tops = top_a + top_b + top_sum;
        printf("%lu\n", tops);
        if (tops > 1) {
            if (tops == 2) {
                // In this case, we have a 1 in the 64th bit, but that 1 is
                //  already accounted for in tops.
                sum = sum << 1 >> 1;
            }
            enqueue_to_Bigint(c, sum);
            // Start the next sum with the 1 that regroups up to the next place
            sum = 1;
        } else {
            sum += tops << 63;
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
    c -> head = a -> head;
    c -> tail = a -> tail;
    return c;
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
    struct Bigint* a = malloc(sizeof(struct Bigint));
    struct Bigint* b = malloc(sizeof(struct Bigint));
    unsigned long n = 0;
    for (int i=0; i<64; i++) {
        n += (unsigned long) 1 << i;
    }
    printf("%lu\n", n);
    enqueue_to_Bigint(a, (unsigned long) n);
    enqueue_to_Bigint(a, (unsigned long) n);
    enqueue_to_Bigint(a, (unsigned long) n);
    enqueue_to_Bigint(b, (unsigned long) n);
    enqueue_to_Bigint(b, (unsigned long) n);
    struct Bigint* c = add_Bigints(a, b);
    print_Bigint(c);
    if (a) {destruct_Bigint(a);}
    if (b) {destruct_Bigint(b);}
    if (c) {destruct_Bigint(c);}
}