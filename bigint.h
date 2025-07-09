/*
This constructs an arbitrary-precision integer arithmetic library.  
We use a queue of 64-bit unsigned integers to represent 'digits' base 2^64,
so the i-th entry, n_i, in the queue has value n_i * (2^64)^i, for i>=0.
NB: This means that 'digits' are ordered from least significant to most.
*/
#ifndef BIGINT_H
#define BIGINT_H

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

// First the data types:
struct Entry_long;
struct Fraction;

struct Bigint {
    // sign should always be +1 or -1.
    char sign;
    struct Entry_long* head;
    struct Entry_long* tail;
    unsigned long len;
};

struct Bigint* construct_Bigint();
// This next function represents the contract for most functions.
bool check_Bigint_isok(struct Bigint* a);
struct Bigint* destruct_Bigint(struct Bigint* b);
void enqueue_to_Bigint(struct Bigint* b, unsigned long n);
void print_Bigint(struct Bigint* b);
struct Bigint* add_Bigints(struct Bigint* a, struct Bigint* b);
struct Bigint* subtract_Bigints(struct Bigint* a, struct Bigint* b);
struct Bigint* multiply_Bigints(struct Bigint* a, struct Bigint* b);
// Notice different return type. This array of pointers needs to be freed.
// This function returns ptr to a/b at return[0], and ptr to a%b at return[1].
struct Bigint** divmod_Bigints(struct Bigint* a, struct Bigint* b);
struct Bigint* gcd_Bigints(struct Bigint* a, struct Bigint* b);
// Returns location of -1 if (a == 0)
long largest_nonzero_bit(struct Bigint* a);
struct Bigint* bitshift_left_Bigint(struct Bigint* a, unsigned long n);
struct Bigint* bitshift_right_Bigint(struct Bigint* a, unsigned long n);
void eliminate_zeros(struct Bigint* a);
bool equal_Bigint(struct Bigint* a, struct Bigint* b);
bool lt_Bigint(struct Bigint* a, struct Bigint* b);
bool leq_Bigint(struct Bigint* a, struct Bigint* b);
bool gt_Bigint(struct Bigint* a, struct Bigint* b);
bool geq_Bigint(struct Bigint* a, struct Bigint* b);

struct Entry_long {
    unsigned long content;
    struct Entry_long* next;
    struct Entry_long* prev;
};

struct Entry_long* construct_Entry_long(unsigned long n);
struct Entry_long* destruct_Entry_long(struct Entry_long* e);

#endif
