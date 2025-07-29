/* 
This constructs a library that creates a data type thay operates using 
unsigned long data type for arithmetic when that is possible, but automatically
promotes to Myint whenever needed.  This should operate without substantial
speed penalty when operting as unsigned long, while still offering some basic
error checking.
*/

#include<stdbool.h>

#ifndef MYINT_H
#define MYINT_H

struct Myint {
    enum _type_t {LONG, BIGINT} int_type;
    char sign;
    unsigned long my_long;
    struct Bigint* bigint;
};

struct Myint* Myint_constructor();
struct Myint* Myint_destructor(struct Myint* a);
bool Myint_contract(struct Myint* a);
void Myint_print(struct Myint* a);
// if int_type is LONG, promote to a single-digit BIGINT
void Myint_promote(struct Myint* a);
// if int_type is BIGINT, eliminate_zeros, then, if only one digit, convert
//    to int_type LONG.
void Myint_reduce(struct Myint* a);
// intlog2 is equivalent to finding the largest nonzero bit of a.
long Myint_intlog2(struct Myint* a);
struct Myint* Myint_add(struct Myint* a, struct Myint* b);
struct Myint* Myint_subtract(struct Myint* a, struct Myint* b);
struct Myint* Myint_multiply(struct Myint* a, struct Myint* b);
// divmod returns a/b as [0] entry and a%b as [1] entry.
struct Myint** Myint_divmod(struct Myint* a, struct Myint* b);
struct Myint* Myint_gcd(struct Myint* a, struct Myint* b);
struct Myint* Myint_bitshift_left(struct Myint* a, unsigned long n);
struct Myint* Myint_bitshift_right(struct Myint* a, unsigned long n);
bool Myint_equal(struct Myint* a, struct Myint* b);
bool Myint_lt(struct Myint* a, struct Myint* b);
bool Myint_leq(struct Myint* a, struct Myint* b);
bool Myint_gt(struct Myint* a, struct Myint* b);
bool Myint_geq(struct Myint* a, struct Myint* b);

#endif 