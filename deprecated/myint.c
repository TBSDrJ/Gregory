#include"myint.h"

#define MEM_LEAK_CHK 0

struct Myint* Myint_constructor() {
    struct Myint* a = malloc(sizeof(struct Myint));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Myint %li\n", (long) a);
    }
    a -> int_type = LONG;
    a -> sign = 1;
    a -> my_long = 0;
    a -> bigint = NULL;
    return a;
}

struct Myint* Myint_from_long(long n) {
    struct Myint* a = Myint_constructor();
    a -> my_long = n;
    return a;
}

struct Myint* Myint_destructor(struct Myint* a) {
    a -> int_type = -2;
    a -> sign = 0;
    a -> my_long = 0;
    if (a -> bigint) {
        a -> bigint = Bigint_destructor(a -> bigint);
    }
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Myint %li\n", (long) a);
    }
    free(a);
    return NULL;
}

bool Myint_contract(struct Myint* a) {
    if (!a) {
        fprintf(stderr, "Myint_contract fails at a is not NULL.\n");
        return false;
    }
    else if (a -> int_type > 1) {
        fprintf(stderr, "Myint_contract fails at int_type > 1\n"); 
        return false;
    }
    else if (a -> int_type < 0) {
        fprintf(stderr, "Myint_contract fails at int_type < 0\n"); 
        return false;
    }
    else if (a -> sign == 0) {
        fprintf(stderr, "Myint_contract fails at sign == 0\n"); 
        return false;
   }
    else if (a -> sign > 1) {
        fprintf(stderr, "Myint_contract fails at sign > 0\n"); 
        return false;
    }
    else if (a -> sign < -1) {
        fprintf(stderr, "Myint_contract fails at sign < 0\n"); 
        return false;
    }
    else if ((a -> int_type == BIGINT) && !(a -> bigint)) {
        fprintf(stderr, "Myint_contract fails at BIGINT.\n"); 
        return false;
    }
    if (a -> bigint) {
        return Bigint_contract(a -> bigint);
    }
    return true;
}

void Myint_print(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_print\n");
        return;
    }
    if (a -> int_type == LONG) {
        if (Myint_getsign(a) < 0) {printf("-");}
        printf("%lu", a -> my_long);
    } else {
        Bigint_print(a -> bigint);
    }
}

void Myint_print_stderr(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_print\n");
        return;
    }
    if (a -> int_type == LONG) {
        if (Myint_getsign(a) < 0) {fprintf(stderr, "-");}
        fprintf(stderr, "%lu", a -> my_long);
    } else {
        Bigint_print_stderr(a -> bigint);
    }
}

void Myint_promote(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_promote\n");
        return;
    }
    a -> int_type = BIGINT;
    a -> bigint = Bigint_from_long(a -> my_long);
    a -> my_long = 0;
    a -> bigint -> sign = a -> sign;
}

void Myint_reduce(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_reduce\n");
        return;
    }
    if (a -> bigint) {
        Bigint_eliminate_zeros(a -> bigint);
        if (a -> bigint -> len == 1) {
            a -> int_type = LONG;
            a -> my_long = a -> bigint -> head -> content;
            a -> sign = a -> bigint -> sign;
            a -> bigint = Bigint_destructor(a -> bigint);
        }
    }
}

struct Myint* Myint_deepcopy(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_deepcopy, a\n");
        return NULL;
    }
    struct Myint* b = Myint_constructor();
    b -> int_type = a -> int_type;
    if (b -> int_type == LONG) {
        b -> my_long = a -> my_long;
        b -> sign = a -> sign;
    } else {
        b -> bigint = Bigint_deepcopy(a -> bigint);
    }
    if (!Myint_contract(b)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_deepcopy, b\n");
        return NULL;
    }
    return b;
}

long Myint_intlog2(struct Myint* a) {
    if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_intlog2\n");
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

void Myint_neg(struct Myint* a) {
       if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_neg\n");
        return;
    }
    if (a -> int_type == LONG) {
        a -> sign *= -1;
    } else {
        a -> bigint -> sign *= -1;
    }
}

int Myint_getsign(struct Myint* a) {
       if (!Myint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Myint_getsign\n");
        return 0;
    }
    int a_sign;
    if (a -> int_type == LONG) {
        a_sign = a -> sign;
    } else {
        a_sign = a -> bigint -> sign;
    }
    return a_sign;
}

struct Myint* Myint_add(struct Myint* a, struct Myint* b) {
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_add\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_add\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
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
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_subtract\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_subtract\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
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
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_multiply\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_multiply\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
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
    if (c -> int_type == LONG) {
        c -> sign = Myint_getsign(a) * Myint_getsign(b);
    } else {
        c -> bigint -> sign = Myint_getsign(a) * Myint_getsign(b);
    }
    return c;
}

struct Myint* Myint_divide(struct Myint* a, struct Myint* b) {
    struct Myint** dm = Myint_divmod(a, b);
    struct Myint* div = dm[0];
    dm[1] = Myint_destructor(dm[1]);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Myint_divmod %li\n", (long) dm);
    }
    free(dm); dm = NULL;
    return div;
}

struct Myint* Myint_mod(struct Myint* a, struct Myint* b) {
    struct Myint** dm = Myint_divmod(a, b);
    struct Myint* mod = dm[1];
    dm[0] = Myint_destructor(dm[0]);
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Myint_divmod %li\n", (long) dm);
    }
    free(dm); dm = NULL;
    return mod;
}

struct Myint** Myint_divmod(struct Myint* a, struct Myint* b) {
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_divmod\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_divmod\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
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
        if (MEM_LEAK_CHK) {
            fprintf(stderr, "free Bigint_divmod %li\n", (long) dm);
        }
        free(dm); dm = NULL;
        Myint_reduce(a);
        Myint_reduce(b);
        Myint_reduce(div);
        Myint_reduce(mod);
    }
    struct Myint** divmod = malloc(2*sizeof(struct Myint*));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Myint_divmod %li\n", (long) divmod);
    }
    if (div -> int_type == LONG) {
        div -> sign = Myint_getsign(a) * Myint_getsign(b);
    } else {
        div -> bigint -> sign = Myint_getsign(a) * Myint_getsign(b);
    }
    if (mod -> int_type == LONG) {
        mod -> sign = Myint_getsign(a);
    } else {
        mod -> bigint -> sign = Myint_getsign(a);
    }
    divmod[0] = div;
    divmod[1] = mod;
    return divmod;
}

struct Myint* Myint_gcd(struct Myint* a, struct Myint* b) {
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_gcd\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_gcd\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
    struct Myint* gcd = Myint_constructor();
    int a_sign = Myint_getsign(a), b_sign = Myint_getsign(b);
    if (a_sign < 0) {Myint_neg(a);}
    if (b_sign < 0) {Myint_neg(b);}
    if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        unsigned long next_a = 0;
        unsigned long next_b = b -> my_long;
        unsigned long q = a -> my_long / b -> my_long;
        unsigned long r = a -> my_long % b -> my_long;
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
    if (a_sign < 0) {Myint_neg(a);} 
    if (b_sign < 0) {Myint_neg(b);} 
    if (gcd -> int_type == LONG) {
        gcd -> sign = 1;
    } else {
        gcd -> bigint -> sign = 1;
    }
    return gcd;
}

struct Myint* Myint_lcm(struct Myint* a, struct Myint* b) {
    struct Myint* c = Myint_multiply(a, b);
    struct Myint* d = Myint_gcd(a, b);
    struct Myint* lcm = Myint_divide(c, d);
    c = Myint_destructor(c);
    d = Myint_destructor(d);
    return lcm;
}

struct Myint* Myint_bitshift_left(struct Myint* a, unsigned long n) {
    if (!Myint_contract(a)) {
        fprintf(stderr, 
                "ERROR: Failed contract, a from Myint_bitshift_left\n");
        return NULL;
    }
    struct Myint* b = Myint_constructor();
    long dig = Myint_intlog2(a);
    if ((a -> int_type == LONG) && (dig + n < 64)) {
        b -> my_long = a -> my_long << n;
    } else {
        if (a -> int_type == LONG) {
            Myint_promote(a);
        }
        b -> int_type = BIGINT;
        b -> bigint = Bigint_bitshift_left(a -> bigint, n);
        Myint_reduce(a);
        Myint_reduce(b);
    }
    if (b -> int_type == LONG) {
        b -> sign = Myint_getsign(a); 
    } else {
        b -> bigint -> sign = Myint_getsign(a);
    }
    return b;
}

struct Myint* Myint_bitshift_right(struct Myint* a, unsigned long n) {
    if (!Myint_contract(a)) {
        fprintf(stderr, 
                "ERROR: Failed contract, a from Myint_bitshift_right\n");
        return NULL;
    }
    struct Myint* b = Myint_constructor();
    long dig = Myint_intlog2(a);
    if (n > dig) {
        return b;
    }
    if (a -> int_type == LONG) {
        b -> my_long = a -> my_long >> n;
    } else {
        b -> int_type = BIGINT;
        b -> bigint = Bigint_bitshift_right(a -> bigint, n);
        Myint_reduce(b);
    }
    if (b -> int_type == LONG) {
        b -> sign = Myint_getsign(a); 
    } else {
        b -> bigint -> sign = Myint_getsign(a);
    }
    return b;
}

bool Myint_equal(struct Myint* a, struct Myint* b) {
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_equal\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_equal\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
    Myint_reduce(a);
    Myint_reduce(b);
    if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        // Make sure that 0 == -0 is True
        if ((a -> my_long == 0) && (b -> my_long == 0)) {return true;}
        if (Myint_getsign(a) != Myint_getsign(b)) {return false;}
        return a -> my_long == b -> my_long;
    } else {
        if (Myint_getsign(a) != Myint_getsign(b)) {return false;}
        if (a -> int_type == LONG) {
            Myint_promote(a);
        } else if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        return Bigint_equal(a -> bigint, b -> bigint);
    }
    return false;
}

bool Myint_lt(struct Myint* a, struct Myint* b) {
    bool a_ok = Myint_contract(a);
    bool b_ok = Myint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, a from Myint_lt\n");
    }
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, b from Myint_lt\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
    Myint_reduce(a);
    Myint_reduce(b);
    if ((Myint_getsign(a) > 0) && (Myint_getsign(b) < 0)) {
        return false;
    } else if ((Myint_getsign(a) < 0) && (Myint_getsign(b) > 0)) {
        return true;
    } else if ((a -> int_type == LONG) && (b -> int_type == LONG)) {
        if (a -> sign > 0) {
            return a -> my_long < b -> my_long;
        } else {
            return a -> my_long > b -> my_long;
        }
    } else {
        if (a -> int_type == LONG) {
            Myint_promote(a);
        } else if (b -> int_type == LONG) {
            Myint_promote(b);
        }
        bool lt = Bigint_lt(a -> bigint, b -> bigint);
        Myint_reduce(a);
        Myint_reduce(b);
        return lt;
    }
}

bool Myint_leq(struct Myint* a, struct Myint* b) {
    if (Myint_lt(a, b) || Myint_equal(a, b)) {
        return true;
    }
    return false;
}

bool Myint_gt(struct Myint* a, struct Myint* b) {
    return Myint_lt(b, a);
}

bool Myint_geq(struct Myint* a, struct Myint* b) {
    return Myint_leq(b, a);
}
