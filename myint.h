#include<stdbool.h>

struct Myint {
    enum _type_t {LONG, BIGINT} int_type;
    char sign;
    unsigned long my_long;
    struct Bigint* bigint;
};

struct Myint* construct_Myint();
struct Myint* destruct_Myint(struct Myint* a);
bool contract_Myint(struct Myint* a);
void print_Myint(struct Myint* a);
void reduce_Myint(struct Myint* a);
long intlog2(struct Myint* a);
struct Myint* add_Myints(struct Myint* a, struct Myint* b);
struct Myint* subtract_Myints(struct Myint* a, struct Myint* b);
struct Myint* multiply_Myints(struct Myint* a, struct Myint* b);
