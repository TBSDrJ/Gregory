#include<stdio.h>
#include<stdlib.h>
#include"bigint.h"
#include"myint.h"

struct Myint* Myint_constructor() {
    struct Myint* a = malloc(sizeof(struct Myint));
    a -> int_type = LONG;
    a -> sign = 1;
    a -> my_long = 0;
    a -> bigint = NULL;
    return a;
}

struct Myint* Myint_destructor(struct Myint* a) {
    // A couple of changes that will make the contract fail
    a -> int_type = -2;
    a -> sign = 0;
    a -> my_long = 0;
    if (a -> bigint) {
        a -> bigint = Bigint_destructor(a -> bigint);
    }
    return NULL;
}

bool Myint_contract(struct Myint* a) {
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
        return Bigint_contract(a -> bigint);
    }
    return true;
}

void Myint_print(struct Myint* a) {
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, Myint_print\n");
        return;
    }
    if (a -> int_type == LONG) {
        if (a -> sign < 0) {printf("-");}
        printf("%lu", a -> my_long);
    } else {
        Bigint_print(a -> bigint);
    }
}

void Myint_promote(struct Myint* a) {
    a -> int_type = BIGINT;
    a -> bigint = Bigint_constructor();
    Bigint_enqueue(a -> bigint, a -> my_long);
    a -> my_long = 0;
    a -> bigint -> sign = a -> sign;
}

void Myint_reduce(struct Myint* a) {
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, Myint_reduce\n");
        return;
    }
    if (a -> bigint) {
        Bigint_eliminate_zeros(a -> bigint);
        if (a -> bigint -> len == 1) {
            a -> int_type = LONG;
            a -> my_long = a -> bigint -> head -> content;
            a -> bigint = Bigint_destructor(a -> bigint);
        }
    }
}

long Myint_intlog2(struct Myint* a) {
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, Myint_intlog2\n");
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
        return Bigint_intlog2(a -> bigint);
    };
    return -2; // error, should never reach this.
}

struct Myint* Myint_add(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, a from Myint_add\n");
        fail = true;
    }
    if (!Myint_contract(b)) {
        printf("ERROR: Failed contract, b from Myint_add\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = Myint_constructor();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (Myint_intlog2(a) < 63) && (Myint_intlog2(b) < 63)) {
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
            Myint_promote(a);
        }
        if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        c -> bigint = Bigint_add(a -> bigint, b -> bigint);
        Myint_reduce(a);
        Myint_reduce(b);
    }
    Myint_reduce(c);
    return c;
}

struct Myint* Myint_subtract(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, a from Myint_subtract\n");
        fail = true;
    }
    if (!Myint_contract(b)) {
        printf("ERROR: Failed contract, b from Myint_subtract\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = Myint_constructor();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (Myint_intlog2(a) < 63) && (Myint_intlog2(b) < 63)) {
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
            Myint_promote(a);
        }
        if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        c -> bigint = Bigint_subtract(a -> bigint, b -> bigint);
        Myint_reduce(a);
        Myint_reduce(b);
    }
    Myint_reduce(c);
    return c;
}

struct Myint* Myint_multiply(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, a from Myint_multiply\n");
        fail = true;
    }
    if (!Myint_contract(b)) {
        printf("ERROR: Failed contract, b from Myint_multiply\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* c = Myint_constructor();
    if ((a -> int_type == LONG) && (b -> int_type == LONG) && 
            (Myint_intlog2(a) + Myint_intlog2(b) < 63)) {
        c -> my_long = (a -> my_long) * (b -> my_long);
    } else {
        c -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            Myint_promote(a);
        }
        if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        c -> bigint = Bigint_multiply(a -> bigint, b -> bigint);
        Myint_reduce(a);
        Myint_reduce(b);
    }
    Myint_reduce(c);
    c -> sign = (a -> sign) * (b -> sign);
    return c;
}

struct Myint** Myint_divmod(struct Myint* a, struct Myint* b) {
    bool fail = false;
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, a from Myint_divmod\n");
        fail = true;
    }
    if (!Myint_contract(b)) {
        printf("ERROR: Failed contract, b from Myint_divmod\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* div = Myint_constructor();
    struct Myint* mod = Myint_constructor();
    if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        div -> my_long = (a -> my_long) / (b -> my_long);
        mod -> my_long = (a -> my_long) % (b -> my_long);
    } else {
        div -> int_type = BIGINT;
        mod -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            Myint_promote(a);
        }
        if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        struct Bigint** dm = Bigint_divmod(a -> bigint, b -> bigint);
        div -> bigint = dm[0];
        mod -> bigint = dm[1];
        free(dm); dm = NULL;
        Myint_reduce(a);
        Myint_reduce(b);
        Myint_reduce(div);
        Myint_reduce(mod);
    }
    struct Myint** divmod = malloc(2*sizeof(struct Myint*));
    divmod[0] = div;
    divmod[1] = mod;
    return divmod;
}

struct Myint* Myint_gcd(struct Myint* a, struct Myint* b) {
        bool fail = false;
    if (!Myint_contract(a)) {
        printf("ERROR: Failed contract, a from Myint_gcd\n");
        fail = true;
    }
    if (!Myint_contract(b)) {
        printf("ERROR: Failed contract, b from Myint_gcd\n");
        fail = true;
    }
    if (fail) {return NULL;}
    struct Myint* gcd = Myint_constructor();
    long a_sign = a -> sign, b_sign = b -> sign;
    a -> sign = 1; b -> sign = 1;
    if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        long next_a = 0;
        long next_b = b -> my_long;
        long q = a -> my_long / b -> my_long;
        long r = a -> my_long % b -> my_long;
        while (r > 0) {
            next_a = next_b;
            next_b = r;
            q = next_a / next_b;
            r = next_a % next_b;
        }
        gcd -> my_long = next_b;
    } else {
        gcd -> int_type = BIGINT;
        if (a -> int_type == LONG) {
            Myint_promote(a);
        }
        if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        gcd -> bigint = Bigint_gcd(a -> bigint, b -> bigint);
        Myint_reduce(a);
        Myint_reduce(b);
        Myint_reduce(gcd);
    }
    a -> sign = a_sign; b -> sign = b_sign;
    return gcd;
}

int main() {
    struct Myint* a = Myint_constructor();
    struct Myint* b = Myint_constructor();
    struct Myint* c = Myint_constructor();
    struct Myint* d = Myint_constructor();
    a -> my_long = 45;
    b -> int_type = BIGINT;
    b -> bigint = Bigint_constructor();
    Bigint_enqueue(b -> bigint, 0);
    Bigint_enqueue(b -> bigint, 0);
    Bigint_enqueue(b -> bigint, 2046);
    // a -> sign = -1;
    // b -> sign = -1;
    c = Myint_gcd(a, b);
    printf("  a: "); Myint_print(a); printf("\n");
    printf("  b: "); Myint_print(b); printf("\n");
    printf("gcd(a,b): "); Myint_print(c); printf("\n");
}