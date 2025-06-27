/*
This constructs an arbitrary-precision fraction of integers.  
We use a queue of 64-bit unsigned integers to represent 'digits' base 2^64,
so the i-th entry, n_i, in the queue has value n_i * (2^64)^i, for i>=0.
*/
#ifndef FRAC_H
#define FRAC_H

#include<stdlib.h>
#include<stdio.h>

// First the data types:
struct Entry_long;
struct Fraction;

struct Queue_long {
    struct Entry_long* head;
    struct Entry_long* tail;
    long len;
};

struct Queue_long* construct_queue_long();
void destruct_queue_long(struct Queue_long* q);
void enqueue_long(struct Queue_long* q, unsigned long n);
void dump_queue(struct Queue_long* q);

struct Entry_long {
    unsigned long content;
    struct Entry_long* next;
};

struct Entry_long* construct_entry_long(unsigned long n);
struct Entry_long* destruct_entry_long(struct Entry_long* e);

struct Fraction {
    // sign should always be +1 or -1.
    char sign;
    struct Queue_long numerator;
    struct Queue_long denominator;
};

struct Fraction* construct_frac(long numerator, long denominator);

#endif