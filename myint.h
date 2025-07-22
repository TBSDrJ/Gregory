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
struct Myint* destruct_Myint(struct Myint* a);
bool contract_Myint(struct Myint* a);
void print_Myint(struct Myint* a);
void promote_Myint(struct Myint* a);
void reduce_Myint(struct Myint* a);
// intlog2 is equivalent to finding the largest nonzero bit of a.
long intlog2_Myint(struct Myint* a);
struct Myint* add_Myints(struct Myint* a, struct Myint* b);
struct Myint* subtract_Myints(struct Myint* a, struct Myint* b);
struct Myint* multiply_Myints(struct Myint* a, struct Myint* b);
// divmod returns a/b as [0] entry and a%b as [1] entry.
struct Myint** divmod_Myints(struct Myint* a, struct Myint* b);
struct Myint* gcd_Myints(struct Myint* a, struct Myint* b);
struct Myint* bitshift_left_Myint(struct Myint* a, unsigned long n);
struct Myint* bitshift_right_Myint(struct Myint* a, unsigned long n);
bool equal_Myint(struct Myint* a, struct Myint* b);
bool lt_Myint(struct Myint* a, struct Myint* b);
bool leq_Myint(struct Myint* a, struct Myint* b);
bool gt_Myint(struct Myint* a, struct Myint* b);
bool geq_Myint(struct Myint* a, struct Myint* b);

#endif MYINT_H