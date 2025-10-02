#include"bigint.h"

#define DEBUG 0
#define MEM_LEAK_CHK 1

// Functions for Bigint
struct Bigint* Bigint_constructor() {
    struct Bigint* a = malloc(sizeof(struct Bigint));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Bigint %li\n", (long) a);
    }
    a -> sign = 1;
    a -> head = NULL;
    a -> tail = NULL;
    a -> len = 0;
    return a;
}

struct Bigint* Bigint_from_long(long a) {
    struct Bigint* b = Bigint_constructor();
    Bigint_enqueue(b, a);
    return b;
}

/* This represents the contract for just about all of the functions -- 
    the Bigint struct exists, has len at least 1, a nonzero sign, and that
    an Entry is attached. We don't check for content because it could be 0, 
    and we don't check for next/prev in the Entry because these are NULL if 
    there is only one Entry. 
    if DEBUG is on, we check that each Entry is well-formed also. */
bool Bigint_contract(struct Bigint* a) {
    if (!a) {
        fprintf(stderr, "Bigint contract fails, a is NULL\n");
        return false;
    }
    if (!(a -> len)) {
        fprintf(stderr, "Bigint contract fails, a -> len is 0\n");
        return false;
    }
    if ((a -> len) < 0) {
        fprintf(stderr, "Bigint contract fails, a -> len is negative.\n");
        return false;
    }
    if (!(a -> sign)) {
        fprintf(stderr, "Bigint contract fails, a -> sign = 0\n");
        return false;
    }
    if ((a -> sign) < -1) {
        fprintf(stderr, "Bigint contract fails, a -> sign < -1\n");
        return false;
    }
    if ((a -> sign) > 1) {
        fprintf(stderr, "Bigint contract fails, a -> sign > 1\n");
        return false;
    }
    if (!(a -> head)) {
        fprintf(stderr, "Bigint contract fails, a -> head is NULL\n");
        return false;
    }
    if (!(a -> tail)) {
        fprintf(stderr, "Bigint contract fails, a -> tail is NULL\n");
        return false;
    }
    if (DEBUG) {
        if (a -> len == 1) {
            if ((a -> head -> prev) || (a -> head -> next) || 
                    (a -> head != a -> tail)) {
                fprintf(stderr, "Bigint contract fails, length is 1, but ");
                fprintf(stderr, "head or tail pointer is not NULL.\n");
                return false;
            }
        } else {
            struct Entry_long* e = a -> head;
            if ((e -> prev) || !(e -> next) || (e != e -> next -> prev)) {
                fprintf(stderr, "Bigint contract fails, head entry ");
                fprintf(stderr, "has an incorrect pointer.\n");
                return false;
            }
            e = e -> next;
            while(e != a -> tail) {
                if (!(e -> prev) || !(e -> next) || (e != e -> next -> prev)) {
                    fprintf(stderr, "Bigint contract fails, interior entry ");
                    fprintf(stderr, "has an incorrect pointer.\n");
                    return false;
                }
                e = e -> next;
            }
            if (!(e -> prev) || (e -> next)) {
                fprintf(stderr, "Bigint contract fails, tail entry ");
                fprintf(stderr, "has an incorrect pointer.\n");
                return false;
            }
        }
    }
    return true;
}

/* We don't use the contract so we can destruct even ill-formed structs, and
    so we can use this in a 'just in case it's still around' case with no
    ill effects. */
struct Bigint* Bigint_destructor(struct Bigint* a) {
    if (a) {
        if (a -> head) {
            struct Entry_long* e = a -> head;
            a -> head = NULL;
            while (e) {
                if (e == a -> tail) {
                    a -> tail = NULL;
                }
                e = Entry_long_destructor(e);
            }
        }
        if (a -> tail) {
            struct Entry_long* e = a -> tail;
            struct Entry_long* f = e;
            while (f) {
                if (e -> prev) {f = e -> prev;}
                Entry_long_destructor(e);
            }
        }
        a -> head = NULL;
        a -> tail = NULL;
        a -> len = 0;
        if (MEM_LEAK_CHK) {
            fprintf(stderr, "free Bigint %li\n", (long) a);
        }
        free(a);
    }
    return NULL;
}

/* We don't use the full contract at the start because this can be used to 
    insert the very first Entry, and that test would fail then. */
void Bigint_enqueue(struct Bigint* a, unsigned long n) {
    if (a) {
        if (!(a -> sign)) {
            fprintf(stderr, 
                    "ERROR: Failed contract, Bigint_enqueue, sign is zero\n");
            return;
        }
        struct Entry_long* e = Entry_long_constructor(n);
        if (a -> tail) {
            a -> tail -> next = e;
            e -> prev = a -> tail;
            a -> tail = e;
            a -> len++;
        } else {
            a -> head = e;
            a -> tail = e;
            a -> len = 1;
        }
    } else {
        fprintf(stderr, "ERROR: Failed contract, Bigint_enqueue, a is NULL\n");
        return; 
    }
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, end of Bigint_enqueue\n");
    }
}

void Bigint_print(struct Bigint* a) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_print\n");
        return;
    }
    struct Entry_long* e = a -> head;
    if (a -> sign == -1) {
        printf("-");}
    while(e) {
        printf("%lu", e -> content);
        if (e -> next) {printf(", ");}
        e = e -> next;
    }
}

struct Bigint* Bigint_deepcopy(struct Bigint* a) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_deepcopy\n");
        return NULL;
    }
    struct Bigint* b = Bigint_constructor();
    struct Entry_long* e_a = a -> head;
    struct Entry_long* e_b_prev = NULL;
    struct Entry_long* e_b = NULL;
    while (e_a) {
        e_b = Entry_long_constructor(e_a -> content);
        e_b -> prev = e_b_prev;
        if (e_b_prev) {
            e_b_prev -> next = e_b;
        } else {
            b -> head = e_b;
        }
        e_a = e_a -> next;
        e_b_prev = e_b;
        b -> len++;
    }
    b -> tail = e_b;
    b -> sign = a -> sign;
    return b;
}

struct Bigint* Bigint_add(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_add, a\n");} 
    if (!b_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_add, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    if ((a -> sign == -1) && (b -> sign == 1)) {
        a -> sign = 1;
        struct Bigint* c = Bigint_subtract(b, a);
        a -> sign = -1;
        return c;
        return NULL;
    } else if ((a -> sign == 1) && (b -> sign == -1)) {
        b -> sign = 1;
        struct Bigint* c = Bigint_subtract(a, b);
        b -> sign = -1;
        return c;
        return NULL;
    } 
    struct Bigint* c = Bigint_constructor();
    if ((a -> sign == -1) && (b -> sign == -1)) {
        c -> sign = -1;
    }
    // We are now set up so that everything works as if a, b, c are positive.
    struct Entry_long* entry_a = a -> head;
    struct Entry_long* entry_b = b -> head;
    unsigned long digit_a;
    unsigned long digit_b;
    // Keep track of top digits to check for and deal with overflow
    unsigned long sum = 0, top_a, top_b, top_sum, tops;
    while ((entry_a) || (entry_b)) {
        if (entry_a) {
            // Separate top digit from rest of number
            digit_a = entry_a -> content;
            top_a = digit_a >> (sizeof(long)*8 - 1);
            digit_a = digit_a << 1 >> 1;
            entry_a = entry_a -> next;
        } else {
            digit_a = 0;
            top_a = 0;
        }
        if (entry_b) {
            digit_b = entry_b -> content;            
            top_b = digit_b >> (sizeof(long)*8 - 1);
            digit_b = digit_b << 1 >> 1;
            entry_b = entry_b -> next;
        } else {
            digit_b = 0;
            top_b = 0;
        }
        sum += digit_a + digit_b;
                top_sum = sum >> (sizeof(unsigned long)*8 - 1);
        tops = top_a + top_b + top_sum;
        if (tops > 1) {
            if (tops == 2) {
                // In this case, we have a 1 in the highest bit, but that 1 is
                //    already accounted for in tops, so drop it.
                sum = sum << 1 >> 1;
            }
            // if (tops == 3), we need 1s in both the highest and 1st place of
            //    the next 'digit' we leave the 1 that is in the highest place.
            Bigint_enqueue(c, sum);
            // Start the next sum with the 1 that regroups up to the 1st place
            //    of the next 'digit'
            sum = 1;
        } else {
            if (top_sum != 1) {
                sum += tops << (sizeof(unsigned long)*8 - 1);}
            Bigint_enqueue(c, sum);
            sum = 0;
        }
    }
    if (sum == 1) {
        Bigint_enqueue(c, 1);
    }
    return c;
}

struct Bigint* Bigint_subtract(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_subtract, a\n");}
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_subtract, a\n");}
    if (!a_ok || !b_ok) {return NULL;}
    // So we know that a and b are not NULL and have at least one entry each.
    if ((a -> sign == 1) && (b -> sign == -1)) {
        b -> sign = 1;
        struct Bigint* c = Bigint_add(a, b);
        b -> sign = -1;
        return c;
    }
    if ((a -> sign == -1) && (b -> sign == 1)) {
        a -> sign = 1;
        struct Bigint* c = Bigint_add(a, b);
        a -> sign = -1;
        c -> sign = -1;
        return c;
    }
    // Now we know that a and b have the same sign
    if (Bigint_equal(a, b)) {
        struct Bigint* c = Bigint_constructor();
        Bigint_enqueue(c, 0);
        return c;
    }
    if ((Bigint_lt(a, b) && (a -> sign == 1)) || 
            (Bigint_lt(b, a) && (a -> sign == -1))) {
        struct Bigint* c = Bigint_subtract(b, a);
        c -> sign *= -1;
        return c;
    }
    // So now we know that a and b have the same sign and |a| > |b|
    struct Bigint* c = Bigint_constructor();
    c -> sign = a -> sign;
    struct Entry_long* a_entry = a -> head;
    struct Entry_long* b_entry = b -> head;
    unsigned long regrouped_one = 0;
    while (a_entry && b_entry) {
        if (a_entry -> content < b_entry -> content) {
            // Notice that -x is actually 2^(sizeof(unsigned long)) - x.
            unsigned long n = -(b_entry -> content) + (a_entry -> content)
                    - regrouped_one;
            regrouped_one = 1;
            Bigint_enqueue(c, n);
        } else {
            unsigned long n = (a_entry -> content) - (b_entry -> content)
                    - regrouped_one;
            regrouped_one = 0;
            Bigint_enqueue(c, n);
        }
        a_entry = a_entry -> next;
        b_entry = b_entry -> next;
    }
    // Since we know that |a| > |b|, if there are any entries remaining, they
    //    are from a.  The only thing left to deal with is a regrouped 1.
    while (a_entry && regrouped_one) {
        if (a_entry -> content > 0) {
            Bigint_enqueue(c, a_entry -> content - 1);
            regrouped_one = 0;
            a_entry = a_entry -> next;
        } else {
            a_entry = a_entry -> next;
            Bigint_enqueue(c, -1);
        }
    }
    while (a_entry) {
        Bigint_enqueue(c, a_entry -> content);
        a_entry = a_entry -> next;
    }
    Bigint_eliminate_zeros(c);
    return c;
}

struct Bigint* Bigint_multiply(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_multiply, a\n");}
    if (!b_ok) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_multiply, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    struct Bigint* c = Bigint_constructor();
    long a_sign = a -> sign;
    long b_sign = b -> sign;
    long c_sign = (a -> sign) * (b -> sign);
    a -> sign = 1;
    b -> sign = 1;
    Bigint_enqueue(c, 0);
    /* The idea is that (n bits)*(n bits) has <= 2n bits.  So, break a and b
    in half so that each product of two halves fits inside the data type. 
    Do the 4 different multiplications of the halves, adjust them for place 
    value, and add those up. */
    unsigned long a_0, a_1, b_0, b_1, prod_0, prod_1, prod_2, prod_3;
    unsigned long places_a = 0, places_b;
    struct Bigint* c_0 = NULL;
    struct Bigint* c_1 = NULL;
    struct Bigint* c_2 = NULL;
    struct Bigint* c_3 = NULL;
    struct Bigint* d_0 = NULL;
    struct Bigint* d_1 = NULL;
    struct Bigint* d_2 = NULL;
    struct Bigint* d_3 = NULL;
    struct Entry_long* entry_a = a -> head;
    struct Entry_long* entry_b = b -> head;
    unsigned long digit_a = 0, digit_b = 0;
    while (entry_a) {
        digit_a = entry_a -> content;
        places_b = 0;
        while (entry_b) {
            c_0 = Bigint_constructor();
            c_1 = Bigint_constructor();
            c_2 = Bigint_constructor();
            c_3 = Bigint_constructor();
            digit_b = entry_b -> content;
            a_0 = digit_a << sizeof(long)*4 >> sizeof(long)*4;
            a_1 = digit_a >> sizeof(long)*4;
            b_0 = digit_b << sizeof(long)*4 >> sizeof(long)*4;
            b_1 = digit_b >> sizeof(long)*4;
            prod_0 = a_0 * b_0;
            prod_1 = a_0 * b_1;
            prod_2 = a_1 * b_0;
            prod_3 = a_1 * b_1;
            Bigint_enqueue(c_0, prod_0);
            d_0 = Bigint_bitshift_left(c_0, places_a + places_b);
            Bigint_enqueue(c_1, prod_1);
            d_1 = Bigint_bitshift_left(
                    c_1, sizeof(long)*4 + places_a + places_b);
            Bigint_enqueue(c_2, prod_2);
            d_2 = Bigint_bitshift_left(
                    c_2, sizeof(long)*4 + places_a + places_b);
            Bigint_enqueue(c_3, prod_3);
            d_3 = Bigint_bitshift_left(
                    c_3, sizeof(long)*8 + places_a + places_b);
            c_0 = Bigint_destructor(c_0);
            c_1 = Bigint_destructor(c_1);
            c_2 = Bigint_destructor(c_2);
            c_3 = Bigint_destructor(c_3);
            c_0 = Bigint_add(c, d_0);
            c_1 = Bigint_add(c_0, d_1);
            c_2 = Bigint_add(c_1, d_2);
            c_3 = Bigint_add(c_2, d_3);
            c = Bigint_destructor(c);
            c = c_3;
            entry_b = entry_b -> next;
            places_b += sizeof(long)*8;
            c_0 = Bigint_destructor(c_0);
            c_1 = Bigint_destructor(c_1);
            c_2 = Bigint_destructor(c_2);
            // Don't destroy c_3 because that is now c which we are keeping
            d_0 = Bigint_destructor(d_0);
            d_1 = Bigint_destructor(d_1);
            d_2 = Bigint_destructor(d_2);
            d_3 = Bigint_destructor(d_3);
        }
        entry_a = entry_a -> next;
        places_a += sizeof(long)*8;
        entry_b = b -> head;
    }
    a -> sign = a_sign;
    b -> sign = b_sign;
    c -> sign = c_sign;
    return c;
}

struct Bigint** Bigint_divmod(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_divmod, a\n");}
    if (!b_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_divmod, b\n");}
    Bigint_eliminate_zeros(b);
    if ((b -> len == 1) && (b -> head -> content == 0)) {
        b_ok = false;
        fprintf(stderr, "ERROR: Division by zero in Bigint_divmod.\n");
    }
    if (!a_ok || !b_ok) {return NULL;}
    long a_sign = a -> sign;
    long b_sign = b -> sign;
    a -> sign = 1;
    b -> sign = 1;
    struct Bigint* quotient = Bigint_constructor(); 
    Bigint_enqueue(quotient, 0);
    struct Bigint* residue = Bigint_constructor(); 
    struct Bigint* one = Bigint_from_long(1); 
    struct Bigint* partial_a = a;
    struct Bigint* shifted_b = NULL;
    struct Bigint* tmp_0 = NULL;
    struct Bigint* tmp_1 = NULL;
    // lnb = location of largest nonzero bit
    long lnb_a, lnb_b;
    if ((a -> len == 1) && (a -> head -> content == 0)) {
        Bigint_enqueue(residue, 0);
    } else if (Bigint_lt(a, b)) {
        struct Entry_long* e = a -> head;
        while (e) {
            Bigint_enqueue(residue, e -> content); 
            e = e -> next;
        }
    } else {
        lnb_a = Bigint_intlog2(a);
        lnb_b = Bigint_intlog2(b);
        while (lnb_b < lnb_a) {
            tmp_0 = shifted_b;
            shifted_b = Bigint_bitshift_left(b, lnb_a - lnb_b); 
            tmp_0 = Bigint_destructor(tmp_0);
            if (!Bigint_leq(shifted_b, partial_a)) {
                tmp_0 = shifted_b;
                shifted_b = Bigint_bitshift_right(shifted_b, 1); 
                tmp_0 = Bigint_destructor(tmp_0);
                lnb_a--;
            }
            tmp_0 = Bigint_bitshift_left(one, lnb_a - lnb_b);
            tmp_1 = quotient;
            quotient = Bigint_add(quotient, tmp_0);
            tmp_0 = Bigint_destructor(tmp_0);
            tmp_1 = Bigint_destructor(tmp_1);
            tmp_0 = partial_a;
            partial_a = Bigint_subtract(partial_a, shifted_b);
            if (tmp_0 != a) {tmp_0 = Bigint_destructor(tmp_0);}
            // Notice that Bigint_intlog2 includes Bigint_eliminate_zeros()
            lnb_a = Bigint_intlog2(partial_a);
        }
        if (Bigint_leq(b, partial_a)) {
            tmp_0 = residue;
            residue = Bigint_subtract(partial_a, b);
            tmp_0 = Bigint_destructor(tmp_0);
            tmp_0 = quotient;
            quotient = Bigint_add(quotient, one);
            tmp_0 = Bigint_destructor(tmp_0);
            if (partial_a != a) {partial_a = Bigint_destructor(partial_a);}
        } else {
            tmp_0 = residue;
            residue = partial_a;
            tmp_0 = Bigint_destructor(tmp_0);
        }
        shifted_b = Bigint_destructor(shifted_b);
    }
    one = Bigint_destructor(one);
    a -> sign = a_sign;
    b -> sign = b_sign;
    quotient -> sign = (a -> sign) * (b -> sign);
    residue -> sign = a -> sign;
    struct Bigint** quot_res = malloc(2*sizeof(struct Bigint*));
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Bigint_divmod %li\n", 
                (long) quot_res);
    }
    quot_res[0] = quotient;
    quot_res[1] = residue;
    return quot_res;
}

struct Bigint* Bigint_gcd(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_gcd, a\n");}
    if (!b_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_gcd, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    long a_sign = a -> sign, b_sign = b -> sign;
    a -> sign = 1; b -> sign = 1;
    if (Bigint_lt(a, b)) {return Bigint_gcd(b, a);}
    struct Bigint** divmod = Bigint_divmod(a, b);
    struct Bigint* quotient = divmod[0];
    struct Bigint* residue = divmod[1];
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "free Bigint_divmod %li\n", 
                (long) divmod);
    }
    free(divmod); divmod = NULL; 
    struct Bigint* zero = Bigint_from_long(0);
    struct Bigint* tmp = NULL;
    struct Bigint* prev_b = b;
    while (Bigint_gt(residue, zero)) {
        if (tmp != b) {
            tmp = Bigint_destructor(tmp);}
        divmod = Bigint_divmod(prev_b, residue);
        tmp = quotient;
        quotient = divmod[0];
        tmp = Bigint_destructor(tmp);
        tmp = prev_b;
        prev_b = residue;
        if (tmp != b) {
        tmp = Bigint_destructor(tmp);}
        residue = divmod[1];
        if (MEM_LEAK_CHK) {
            fprintf(stderr, "free Bigint_divmod %li\n", 
                    (long) divmod);
        }
        free(divmod); divmod = NULL;
    }
    zero = Bigint_destructor(zero); 
    quotient = Bigint_destructor(quotient);
    residue = Bigint_destructor(residue);
    a -> sign = a_sign; b -> sign = b_sign;
    if (prev_b == b) {
        prev_b = Bigint_deepcopy(b);
    }
    if (prev_b == a) {
        prev_b = Bigint_deepcopy(a);
    }
    return prev_b;
}

long Bigint_intlog2(struct Bigint* a) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_intlog2\n");
        return -2;
    }
    Bigint_eliminate_zeros(a);
    if ((a -> len == 1) && (a -> head -> content == 0)) {
        return -1;
    }
    /* Now we know that there is a 1 somewhere in the largest 'digit' of a. */
    struct Entry_long* e = a -> tail;
    long e_largest = e -> content;
    long n = sizeof(long) * 8 * ((a -> len) - 1);
    unsigned char loc = sizeof(long) * 8 - 1;
    while (!(e_largest >> loc)) {loc--;}
    return n + loc;
}

struct Bigint* Bigint_bitshift_left(struct Bigint* a, unsigned long n) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_bitshift_left, a\n");
        return NULL;
    }
    struct Bigint* b = Bigint_constructor();
    struct Entry_long* e = a -> head;
    if (n == 0) {
        // This does come up, so let's speed things up for when it does
        while (e) {
            Bigint_enqueue(b, e -> content);
            e = e -> next;
        }
    } else {
        unsigned long digits = n/64, bits = n%64;
        unsigned long n_0, n_1 = 0;
        for (long i=0; i<digits; i++) {
            Bigint_enqueue(b, 0);
        }
        while (e) {
            n_0 = (e -> content) << bits;
            // n_0 should have all 0s wherever n_1 could have 1s and vice versa.
            n_0 |= n_1;
            // if bits == 0, no shift happens because the number of places is 
            //   equal to the size of the data type.
            if (bits > 0) {
                n_1 = ((e -> content) >> (sizeof(long)*8 - bits));
            } else {
                n_1 = 0;
            }
            Bigint_enqueue(b, n_0);
            e = e -> next;
        }
        if (n_1) {
            Bigint_enqueue(b, n_1);
        }
        Bigint_eliminate_zeros(b);
    }
    b -> sign = a -> sign;
    return b;
}

struct Bigint* Bigint_bitshift_right(struct Bigint* a, unsigned long n) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_bitshift_right, a\n");
        return NULL;
    }
    struct Bigint* b = Bigint_constructor();
    struct Entry_long* e = a -> head;
    if (n == 0) {
        // This does come up, so let's speed things up for when it does
        while (e) {
            Bigint_enqueue(b, e -> content);
            e = e -> next;
        }
    } else {
        unsigned long digits = n/64, bits = n%64;
        unsigned long n_0, n_1 = 0;
        for (long i=0; i<digits; i++) {
            if (e) {
                e = e -> next;
            } else {
                Bigint_enqueue(b, 0);
                return b;
            }
        }
        while (e) {
            n_0 = (e -> content) >> bits;
            e = e -> next;
            if (e) {
                n_1 = (e -> content) << (sizeof(long)*8 - bits);
            } else {
                n_1 = 0;
            }
            Bigint_enqueue(b, n_0 | n_1);
        }
        if (!(b -> head)) {
            // Make sure b is certain to be nonempty.
            Bigint_enqueue(b, 0);
        }
    }
    b -> sign = a -> sign;
    return b;
}

void Bigint_eliminate_zeros(struct Bigint* a) {
    if (!Bigint_contract(a)) {
        fprintf(stderr, "ERROR: Failed contract, Bigint_eliminate_zeros, a\n");
        return;
    }
    struct Entry_long* e = a -> tail;
    while ((e -> content) == 0) {
        if (e -> prev) {
            a -> tail = e -> prev;
            a -> tail -> next = NULL;
            Entry_long_destructor(e);
            a -> len--;
            e = a -> tail;
        } else {
            break;
        }
    }
}

bool Bigint_equal(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_equal, a\n");}
    if (!b_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_equal, b\n");}
    if (!a_ok || !b_ok) {return false;}
    Bigint_eliminate_zeros(a);
    Bigint_eliminate_zeros(b);
    // 0 should test as equal to -0
    if (((a -> len == 1) && (b -> len == 1)) && (a -> head -> content == 0) 
            && (b -> head -> content == 0)) {
        return true;
    }
    if (a -> sign != b -> sign) {
        return false;
    }
    if (a -> len != b -> len) {
        return false;
    }
    struct Entry_long* entry_a = a -> head;
    struct Entry_long* entry_b = b -> head;
    while (entry_a && entry_b) {
        if (entry_a -> content != entry_b -> content) {
            return false;
        }
        entry_a = entry_a -> next;
        entry_b = entry_b -> next;
    }
    return true;
}

bool Bigint_lt(struct Bigint* a, struct Bigint* b) {
    bool a_ok = Bigint_contract(a);
    bool b_ok = Bigint_contract(b);
    if (!a_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_lt, a\n");}
    if (!b_ok) 
        {fprintf(stderr, "ERROR: Failed contract, Bigint_lt, b\n");}
    if (!a_ok || !b_ok) {return false;}
    if (a -> sign == -1 && b -> sign == 1) {
        return true;
    }
    if (a -> sign == 1 && b -> sign == -1) {
        return false;
    }
    Bigint_eliminate_zeros(a);
    Bigint_eliminate_zeros(b);
    if (a -> len < b -> len) {
        if (a -> sign == 1) {
            return true;
        } else {
            return false;
        }
    }
    if (a -> len > b -> len) {
        if (a -> sign == 1) {
            return false;
        } else {
            return true;
        }
    }
    struct Entry_long* entry_a = a -> tail;
    struct Entry_long* entry_b = b -> tail;
    while (entry_a && entry_b) {
        if (entry_a -> content < entry_b -> content) {
            if (a -> sign == 1) {
                return true;
            } else {
                return false;
            }
        }
        if (entry_a -> content > entry_b -> content) {
            if (a -> sign == 1) {
                return false;
            } else {
                return true;
            }
        }
        entry_a = entry_a -> prev;
        entry_b = entry_b -> prev;
    }
    return false;
}

bool Bigint_leq(struct Bigint* a, struct Bigint* b) {
    if (Bigint_equal(a, b) || Bigint_lt(a, b)) {
        return true;
    }
    return false;
}

bool Bigint_gt(struct Bigint* a, struct Bigint* b) {
    return Bigint_lt(b, a);
}

bool Bigint_geq(struct Bigint* a, struct Bigint* b) {
    return Bigint_leq(b, a);
}

// Functions for Entry_long
struct Entry_long* Entry_long_constructor(unsigned long n) {
    struct Entry_long* e = malloc(sizeof(struct Entry_long));
    e -> content = n;
    e -> next = NULL;
    e -> prev = NULL;
    if (MEM_LEAK_CHK) {
        fprintf(stderr, "malloc Entry_long %li\n", (long) e);
    }
    return e;
}

struct Entry_long* Entry_long_destructor(struct Entry_long* e) {
    struct Entry_long* next = NULL;
    if (e) {
        next = e -> next;
        if (MEM_LEAK_CHK) {
            fprintf(stderr, "free Entry_long %li\n", (long) e);
        }
        free(e);
    }
    e = NULL;
    return next;
}
