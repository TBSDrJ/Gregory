/*
This constructs an arbitrary-precision integer arithmetic library.  
We use a queue of unsigned long integers to represent 'digits' base 2^64,
so the i-th entry, n_i, in the queue has value n_i * (2^64)^i, for i>=0.
While this documentation assumes that an unsigned long is 64 bits, I believe
that the code does not assume this, and will just use digits of size equal
to sizeof(unsigned long), whatever that size is, but I have not tested this.
NB: This means that 'digits' are ordered from least significant to most.
*/
#ifndef BIGINT_H
#define BIGINT_H

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

// First the data types:
struct Entry_long;

struct Bigint {
    // sign should always be +1 or -1.
    char sign;
    struct Entry_long* head;
    struct Entry_long* tail;
    unsigned long len;
};

struct Bigint* Bigint_constructor();
struct Bigint* Bigint_from_long(long a);
struct Bigint* Bigint_destructor(struct Bigint* a);
// This next function represents the contract for most functions.
bool Bigint_contract(struct Bigint* a);
void Bigint_enqueue(struct Bigint* a, unsigned long n);
void Bigint_print(struct Bigint* a);
void Bigint_print_stderr(struct Bigint* a);
struct Bigint* Bigint_deepcopy(struct Bigint* a);
struct Bigint* Bigint_add(struct Bigint* a, struct Bigint* b);
struct Bigint* Bigint_subtract(struct Bigint* a, struct Bigint* b);
struct Bigint* Bigint_multiply(struct Bigint* a, struct Bigint* b);
// Notice different return type. This array of pointers needs to be freed.
// This function returns ptr to a/b at return[0], and ptr to a%b at return[1].
// Also note: I have chosen to have sign of remainder match sign of numerator.  
//      This is *not* entirely typical, it more typical to always use 
//      remainder > 0. In particular, matching results from this with Python 
//      will disagree when numerator < 0.  But, always, a = bq + r.
// As of now, division by zero results in a segfault.
struct Bigint** Bigint_divmod(struct Bigint* a, struct Bigint* b);
// As of now, if (a == 0 or b == 0) then we get a segfault.
// Note: Sign of gcd(a,b) will always be positive.
// I didn't have this before and it caused problems with reducing fractions.
struct Bigint* Bigint_gcd(struct Bigint* a, struct Bigint* b);
// Returns -1 if (a == 0), -2 if error
long Bigint_intlog2(struct Bigint* a);
struct Bigint* Bigint_bitshift_left(struct Bigint* a, unsigned long n);
struct Bigint* Bigint_bitshift_right(struct Bigint* a, unsigned long n);
void Bigint_eliminate_zeros(struct Bigint* a);
bool Bigint_equal(struct Bigint* a, struct Bigint* b);
bool Bigint_lt(struct Bigint* a, struct Bigint* b);
bool Bigint_leq(struct Bigint* a, struct Bigint* b);
bool Bigint_gt(struct Bigint* a, struct Bigint* b);
bool Bigint_geq(struct Bigint* a, struct Bigint* b);

struct Entry_long {
    unsigned long content;
    struct Entry_long* next;
    struct Entry_long* prev;
};

struct Entry_long* Entry_long_constructor(unsigned long n);
struct Entry_long* Entry_long_destructor(struct Entry_long* e);

#endif
