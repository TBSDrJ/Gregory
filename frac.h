/*
This constructs an arbitrary-precision fraction of integers.  
We use a queue of 64-bit unsigned integers to represent 'digits' base 2^64,
so the i-th entry, n_i, in the queue has value n_i * (2^64)^i, for i>=0.
NB: This means that 'digits' are ordered from least significant to most.
*/
#ifndef FRAC_H
#define FRAC_H

#include<stdlib.h>
#include<stdio.h>

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
void destruct_Bigint(struct Bigint* b);
void enqueue_to_Bigint(struct Bigint* b, unsigned long n);
void print_Bigint(struct Bigint* b);
struct Bigint* add_Bigints(struct Bigint* a, struct Bigint* b);
struct Bigint* subtract_Bigints(struct Bigint* a, struct Bigint* b);
// NB: neg_Bigint points to a for its digits, so if a changes, so will its neg!
struct Bigint* neg_Bigint(struct Bigint* a);

struct Entry_long {
    unsigned long content;
    struct Entry_long* next;
};

struct Entry_long* construct_entry_long(unsigned long n);
struct Entry_long* destruct_entry_long(struct Entry_long* e);

struct Fraction {
    struct Bigint* numerator;
    struct Bigint* denominator;
};

struct Fraction* construct_frac();

#endif