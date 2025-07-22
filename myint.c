#include<stdio.h>
#include<stdlib.h>
#include"bigint.h"
#include"myint.h"

struct Myint* construct_Myint() {
    struct Myint* a = malloc(sizeof(struct Myint));
    a -> int_type = LONG;
    a -> sign = 1;
    a -> my_long = 0;
    a -> bigint = NULL;
    return a;
}

struct Myint* destruct_Myint(struct Myint* a) {
    // A couple of changes that will make the contract fail
    a -> int_type = -2;
    a -> sign = 0;
    a -> my_long = 0;
    if (a -> bigint) {
        a -> bigint = destruct_Bigint(a -> bigint);
    }
    return NULL;
}

bool contract_Myint(struct Myint* a) {
    if (a -> int_type > 1) 
        {printf("Contract fails at int_type > 1\n"); return false;}
    else if (a -> int_type < 0) 
        {printf("Contract fails at int_type < 0\n"); return false;}
    else if (a -> sign == 0) 
        {printf("Contract fails at sign == 0\n"); return false;}
    else if (a -> sign > 1) 
        {printf("Contract fails at sign > 0\n"); return false;}
    else if (a -> sign < -1) 
        {printf("Contract fails at sign < 0\n"); return false;}
    else if ((a -> int_type == BIGINT) && !(a -> bigint)) {
        printf("Contract fails at BIGINT.\n"); return false;
    }
    if (a -> bigint) {
        return check_Bigint_isok(a -> bigint);
    }
    return true;
}

void print_Myint(struct Myint* a) {
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, print_Myint\n");
        return;
    }
    if (a -> int_type == LONG) {
        if (a -> sign < 0) {printf("-");}
        printf("%lu", a -> my_long);
    } else {
        print_Bigint(a -> bigint);
    }
}

void promote_Myint(struct Myint* a) {
    a -> int_type = BIGINT;
    a -> bigint = construct_Bigint();
    enqueue_to_Bigint(a -> bigint, a -> my_long);
    a -> my_long = 0;
    a -> bigint -> sign = a -> sign;
}

void reduce_Myint(struct Myint* a) {
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, reduce_Myint\n");
        return;
    }
    if (a -> bigint) {
        eliminate_zeros(a -> bigint);
        if (a -> bigint -> len == 1) {
            a -> int_type = LONG;
            a -> my_long = a -> bigint -> head -> content;
            a -> bigint = destruct_Bigint(a -> bigint);
        }
    }
}

long intlog2_Myint(struct Myint* a) {
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, intlog2_Myint\n");
        return -2;
    }
    if (a -> int_type == LONG) {
        if (a -> my_long == 0) {return -1;}
        for (long j=63; j>=0; j--) {
            if (a -> my_long >> j) {
                return j;
            }
        }
    }
    else {
        return largest_nonzero_bit(a -> bigint);
    };
    return -2; // error, should never reach this.
}

struct Myint* add_Myints(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, a from add_Myints\n");
        fail = true;
    }
    if (!contract_Myint(b)) {
        printf("ERROR: Failed contract, b from add_Myints\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = construct_Myint();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (intlog2_Myint(a) < 63) && (intlog2_Myint(b) < 63)) {
        if (a -> sign == b -> sign) {
            c -> my_long = (a -> my_long) + (b -> my_long);
            c -> sign = a -> sign;
        } else {
            if (a -> my_long > b -> my_long) {
                c -> my_long = (a -> my_long) - (b -> my_long);
                if (a -> sign < 0) {c -> sign = -1;}
            } else {
                c -> my_long = (b -> my_long) - (a -> my_long);
                if (b -> sign < 0) {c -> sign = -1;}
            }
        }
    } else {
        c -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            promote_Myint(a);
        }
        if (b -> int_type == LONG) {
            promote_Myint(b);
        }
        c -> bigint = add_Bigints(a -> bigint, b -> bigint);
        reduce_Myint(a);
        reduce_Myint(b);
    }
    reduce_Myint(c);
    return c;
}

struct Myint* subtract_Myints(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, a from subtract_Myints\n");
        fail = true;
    }
    if (!contract_Myint(b)) {
        printf("ERROR: Failed contract, b from subtract_Myints\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = construct_Myint();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (intlog2_Myint(a) < 63) && (intlog2_Myint(b) < 63)) {
        if (a -> sign == b -> sign) {
            if (a -> my_long > b -> my_long) {
                c -> my_long = (a -> my_long) - (b -> my_long);
                c -> sign = a -> sign;
            } else {
                c -> my_long = (b -> my_long) - (a -> my_long);
                c -> sign = (-1)*(a -> sign);
            }
        } else {
            c -> my_long = (a -> my_long) + (b -> my_long);
            if (a -> sign < 0) {c -> sign = -1;}        
        }
    } else {
        c -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            promote_Myint(a);
        }
        if (b -> int_type == LONG) {
            promote_Myint(b);
        }
        c -> bigint = subtract_Bigints(a -> bigint, b -> bigint);
        reduce_Myint(a);
        reduce_Myint(b);
    }
    reduce_Myint(c);
    return c;
}

struct Myint* multiply_Myints(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, a from multiply_Myints\n");
        fail = true;
    }
    if (!contract_Myint(b)) {
        printf("ERROR: Failed contract, b from multiply_Myints\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = construct_Myint();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (intlog2_Myint(a) + intlog2_Myint(b) < 63)) {
        c -> my_long = (a -> my_long) * (b -> my_long);
    } else {
        c -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            promote_Myint(a);
        }
        if (b -> int_type == LONG) {
            promote_Myint(b);
        }
        c -> bigint = multiply_Bigints(a -> bigint, b -> bigint);
        reduce_Myint(a);
        reduce_Myint(b);
    }
    reduce_Myint(c);
    c -> sign = (a -> sign) * (b -> sign);
    return c;
}

struct Myint** divmod_Myints(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!contract_Myint(a)) {
        printf("ERROR: Failed contract, a from divmod_Myints\n");
        fail = true;
    }
    if (!contract_Myint(b)) {
        printf("ERROR: Failed contract, b from divmod_Myints\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* div = construct_Myint();
    struct Myint* mod = construct_Myint();
    if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        div -> my_long = (a -> my_long) / (b -> my_long);
        mod -> my_long = (a -> my_long) % (b -> my_long);
    } else {
        div -> int_type = BIGINT;
        mod -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            promote_Myint(a);
        }
        if (b -> int_type == LONG) {
            promote_Myint(b);
        }
        struct Bigint** dm = divmod_Bigints(a -> bigint, b -> bigint);
        div -> bigint = dm[0];
        mod -> bigint = dm[1];
        free(dm); dm = NULL;
        reduce_Myint(a);
        reduce_Myint(b);
        reduce_Myint(div);
        reduce_Myint(mod);
    }
    struct Myint** divmod = malloc(2*sizeof(struct Myint*));
    divmod[0] = div;
    divmod[1] = mod;
    return divmod;
}

int main() {
    struct Myint* a = construct_Myint();
    struct Myint* b = construct_Myint();
    struct Myint* c = construct_Myint();
    struct Myint* d = construct_Myint();
    a -> my_long = 5;
    b -> my_long = 2048;
    a -> sign = -1;
    b -> sign = -1;
    struct Myint** dm = divmod_Myints(b, a);
    c = dm[0];
    d = dm[1];
    printf("  a: "); print_Myint(a); printf("\n");
    printf("  b: "); print_Myint(b); printf("\n");
    printf("a/b: "); print_Myint(c); printf("\n");
    printf("a%%b: "); print_Myint(d); printf("\n");
}