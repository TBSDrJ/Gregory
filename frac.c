#include"frac.h"

#define DEBUG 1

// Functions for Bigint
struct Bigint* construct_Bigint() {
    struct Bigint* a = malloc(sizeof(struct Bigint));
    a -> sign = 1;
    a -> head = NULL;
    a -> tail = NULL;
    a -> len = 0;
    return a;
}

/* This represents the contract for just about all of the functions -- 
    the Bigint struct exists, has len at least 1, a nonzero sign, and that
    an Entry is attached. We don't check for content because it could be 0, 
    and we don't check for next/prev in the Entry because these are NULL if 
    there is only one Entry. 
    if DEBUG is on, we check that each Entry is well-formed also. */
bool check_Bigint_isok(struct Bigint* a) {
    if (!a || !(a -> len) || !(a -> sign) || !(a -> head) || !(a -> tail)) {
        return false;
    }
    if (DEBUG) {
        if (a -> len == 1) {
            if ((a -> head -> prev) || (a -> head -> next) || 
                    (a -> head != a -> tail)) {
                return false;
            }
        } else {
            struct Entry_long* e = a -> head;
            if ((e -> prev) || !(e -> next) || (e != e -> next -> prev)) {
                return false;
            }
            e = e -> next;
            while(e != a -> tail) {
                if (!(e -> prev) || !(e -> next) || (e != e -> next -> prev)) {
                    return false;
                }
                e = e -> next;
            }
            if (!(e -> prev) || (e -> next)) {
                return false;
            }
        }
    }
    return true;
}

/* We don't use the contract so we can destruct even ill-formed structs, and
    so we can use this in a 'just in case it's still around' case with no
    ill effects. */
struct Bigint* destruct_Bigint(struct Bigint* a) {
    if (a) {
        if (a -> head) {
            struct Entry_long* e = a -> head;
            a -> head = NULL;
            while (e) {
                if (e == a -> tail) {
                    a -> tail = NULL;
                }
                e = destruct_Entry_long(e);
            }
        }
        if (a -> tail) {
            struct Entry_long* e = a -> tail;
            struct Entry_long* f = e;
            while (f) {
                if (e -> prev) {f = e -> prev;}
                destruct_Entry_long(e);
            }
        }
        a -> head = NULL;
        a -> tail = NULL;
        a -> len = 0;
        free(a);
    }
    return NULL;
}

/* We don't use the full contract at the start because this can be used to 
    insert the very first Entry, and that test would fail then. */
void enqueue_to_Bigint(struct Bigint* a, unsigned long n) {
    if (a) {
        if (DEBUG && !(a -> sign)) {
            printf("ERROR: Failed contract, enqueue, sign is zero\n");
        }
        struct Entry_long* e = construct_Entry_long(n);
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
    } else if (DEBUG) {printf("ERROR: Failed contract, enqueue, a is NULL\n");}
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, end of enqueue\n");}
    }
}

void print_Bigint(struct Bigint* a) {
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, print\n");}
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

struct Bigint* add_Bigints(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, add, a\n");} 
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, add, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    if ((a -> sign == -1) && (b -> sign == 1)) {
        a -> sign = 1;
        // struct Bigint* c = subtract_Bigints(b, a);
        a -> sign = -1;
        // return c;
        return NULL;
    } else if ((a -> sign == 1) && (b -> sign == -1)) {
        b -> sign = 1;
        // struct Bigint* c = subtract_Bigints(a, b);
        b -> sign = -1;
        // return c;
        return NULL;
    } 
    struct Bigint* c = construct_Bigint();
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
            enqueue_to_Bigint(c, sum);
            // Start the next sum with the 1 that regroups up to the 1st place
            //    of the next 'digit'
            sum = 1;
        } else {
            sum += tops << (sizeof(unsigned long)*8 - 1);
            enqueue_to_Bigint(c, sum);
            sum = 0;
        }
    }
    if (sum == 1) {
        enqueue_to_Bigint(c, 1);
    }
    return c;
}

struct Bigint* subtract_Bigints(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, subtract, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, subtract, a\n");}
    if (!a_ok || !b_ok) {return NULL;}
    // So we know that a and b are not NULL and have at least one entry each.
    if ((a -> sign == 1) && (b -> sign == -1)) {
        b -> sign = 1;
        struct Bigint* c = add_Bigints(a, b);
        b -> sign = -1;
        return c;
    }
    if ((a -> sign == -1) && (b -> sign == 1)) {
        a -> sign = 1;
        struct Bigint* c = add_Bigints(a, b);
        a -> sign = -1;
        c -> sign = -1;
        return c;
    }
    // Now we know that a and b have the same sign
    if (equal_Bigint(a, b)) {
        struct Bigint* c = construct_Bigint();
        enqueue_to_Bigint(c, 0);
        return c;
    }
    if ((lt_Bigint(a, b) && (a -> sign == 1)) || 
            (lt_Bigint(b, a) && (a -> sign == -1))) {
        struct Bigint* c = subtract_Bigints(b, a);
        c -> sign *= -1;
        return c;
    }
    // So now we know that a and b have the same sign and |a| > |b|
    struct Bigint* c = construct_Bigint();
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
            enqueue_to_Bigint(c, n);
        } else {
            unsigned long n = (a_entry -> content) - (b_entry -> content)
                    - regrouped_one;
            regrouped_one = 0;
            enqueue_to_Bigint(c, n);
        }
        a_entry = a_entry -> next;
        b_entry = b_entry -> next;
    }
    // Since we know that |a| > |b|, if there are any entries remaining, they
    //    are from a.  The only thing left to deal with is a regrouped 1.
    while (a_entry && regrouped_one) {
        if (a_entry -> content > 0) {
            a_entry -> content--;
            regrouped_one = 0;
        } else {
            a_entry = a_entry -> next;
        }
    }
    eliminate_zeros(c);
    return c;
}

struct Bigint* multiply_Bigints(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, mulitply, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, multiply, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    struct Bigint* c = construct_Bigint();
    long a_sign = a -> sign;
    long b_sign = b -> sign;
    long c_sign = (a -> sign) * (b -> sign);
    a -> sign = 1;
    b -> sign = 1;
    enqueue_to_Bigint(c, 0);
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
            c_0 = construct_Bigint();
            c_1 = construct_Bigint();
            c_2 = construct_Bigint();
            c_3 = construct_Bigint();
            digit_b = entry_b -> content;
            a_0 = digit_a << sizeof(long)*4 >> sizeof(long)*4;
            a_1 = digit_a >> sizeof(long)*4;
            b_0 = digit_b << sizeof(long)*4 >> sizeof(long)*4;
            b_1 = digit_b >> sizeof(long)*4;
            prod_0 = a_0 * b_0;
            prod_1 = a_0 * b_1;
            prod_2 = a_1 * b_0;
            prod_3 = a_1 * b_1;
            enqueue_to_Bigint(c_0, prod_0);
            d_0 = bitshift_left_Bigint(c_0, places_a + places_b);
            enqueue_to_Bigint(c_1, prod_1);
            d_1 = bitshift_left_Bigint(
                    c_1, sizeof(long)*4 + places_a + places_b);
            enqueue_to_Bigint(c_2, prod_2);
            d_2 = bitshift_left_Bigint(
                    c_2, sizeof(long)*4 + places_a + places_b);
            enqueue_to_Bigint(c_3, prod_3);
            d_3 = bitshift_left_Bigint(
                    c_3, sizeof(long)*8 + places_a + places_b);
            c_0 = destruct_Bigint(c_0);
            c_1 = destruct_Bigint(c_1);
            c_2 = destruct_Bigint(c_2);
            c_3 = destruct_Bigint(c_3);
            c_0 = add_Bigints(c, d_0);
            c_1 = add_Bigints(c_0, d_1);
            c_2 = add_Bigints(c_1, d_2);
            c_3 = add_Bigints(c_2, d_3);
            c = destruct_Bigint(c);
            c = c_3;
            entry_b = entry_b -> next;
            places_b += sizeof(long)*8;
            c_0 = destruct_Bigint(c_0);
            c_1 = destruct_Bigint(c_1);
            c_2 = destruct_Bigint(c_2);
            // Don't destroy c_3 because that is now c which we are keeping
            d_0 = destruct_Bigint(d_0);
            d_1 = destruct_Bigint(d_1);
            d_2 = destruct_Bigint(d_2);
            d_3 = destruct_Bigint(d_3);
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

struct Bigint** divmod_Bigints(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, divide, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, divide, b\n");}
    if ((b -> len == 1) && (b -> head -> content == 0)) {
        b_ok = false;
        if (DEBUG) {printf("ERROR: Division by zero in divmod.\n");}
    }
    if (!a_ok || !b_ok) {return NULL;}
    long a_sign = a -> sign;
    long b_sign = b -> sign;
    a -> sign = 1;
    b -> sign = 1;
    struct Bigint* quotient = construct_Bigint(); 
    enqueue_to_Bigint(quotient, 0);
    struct Bigint* residue = construct_Bigint(); 
    struct Bigint* one = construct_Bigint(); 
    enqueue_to_Bigint(one, 1);
    struct Bigint* partial_a = a;
    struct Bigint* shifted_b = NULL;
    struct Bigint* tmp_0 = NULL;
    struct Bigint* tmp_1 = NULL;
    // largest nonzero bit
    long lnb_a, lnb_b;
    if ((a -> len == 1) && (a -> head -> content == 0)) {
        enqueue_to_Bigint(residue, 0);
    } else if (lt_Bigint(a, b)) {
        struct Entry_long* e = a -> head;
        while (e) {
            enqueue_to_Bigint(residue, e -> content); 
            e = e -> next;
        }
    } else {
        lnb_a = largest_nonzero_bit(a);
        lnb_b = largest_nonzero_bit(b);
        while (lnb_b < lnb_a) {
            tmp_0 = shifted_b;
            shifted_b = bitshift_left_Bigint(b, lnb_a - lnb_b); 
            tmp_0 = destruct_Bigint(tmp_0);
            if (!leq_Bigint(shifted_b, partial_a)) {
                tmp_0 = shifted_b;
                shifted_b = bitshift_right_Bigint(shifted_b, 1); 
                tmp_0 = destruct_Bigint(tmp_0);
                lnb_a--;
            }
            tmp_0 = bitshift_left_Bigint(one, lnb_a - lnb_b);
            tmp_1 = quotient;
            quotient = add_Bigints(quotient, tmp_1);
            tmp_0 = destruct_Bigint(tmp_0);
            tmp_1 = destruct_Bigint(tmp_1);
            tmp_0 = partial_a;
            partial_a = subtract_Bigints(partial_a, shifted_b);
            if (tmp_0 != a) {tmp_0 = destruct_Bigint(tmp_0);}
            // Notice that largest_nonzero_bit includes eliminate_zeros()
            lnb_a = largest_nonzero_bit(partial_a);
        }
        if (leq_Bigint(b, partial_a)) {
            tmp_0 = residue;
            residue = subtract_Bigints(partial_a, b);
            tmp_0 = destruct_Bigint(tmp_0);
            tmp_0 = quotient;
            quotient = add_Bigints(quotient, one);
            tmp_0 = destruct_Bigint(tmp_0);
            if (partial_a != a) {partial_a = destruct_Bigint(partial_a);}
        } else {
            tmp_0 = residue;
            residue = partial_a;
            tmp_0 = destruct_Bigint(tmp_0);
        }
        shifted_b = destruct_Bigint(shifted_b);
        one = destruct_Bigint(one);
    }
    a -> sign = a_sign;
    b -> sign = b_sign;
    quotient -> sign = (a -> sign) * (b -> sign);
    residue -> sign = a -> sign;
    struct Bigint** quot_res = malloc(2*sizeof(struct Bigint*));
    quot_res[0] = quotient;
    quot_res[1] = residue;
    return quot_res;
}

// Still a Memory leak in GCD.  Def none in divmod.
struct Bigint* gcd_Bigints(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a); //printf("440 CONSTRUCT:A %lu\n", (unsigned long) a);
    bool b_ok = check_Bigint_isok(b); //printf("440 CONSTRUCT:B %lu\n", (unsigned long) b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, gcd, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, gcd, b\n");}
    if (!a_ok || !b_ok) {return NULL;}
    if (lt_Bigint(a, b)) {return gcd_Bigints(b, a);}
    struct Bigint** divmod = divmod_Bigints(a, b); //printf("446 CONSTRUCT %lu\n", (unsigned long) divmod);
    struct Bigint* quotient = divmod[0]; //printf("447 CONSTRUCT %lu\n", (unsigned long) quotient);
    struct Bigint* residue = divmod[1]; //printf("448 CONSTRUCT %lu\n", (unsigned long) residue);
    // printf("\t449 DESTRUCT %lu\n", (unsigned long) divmod); 
    free(divmod); divmod = NULL; 
    struct Bigint* zero = construct_Bigint(); //printf("450 CONSTRUCT %lu\n", (unsigned long) zero);
    enqueue_to_Bigint(zero, 0);
    struct Bigint* tmp = NULL;
    struct Bigint* prev_b = b;
    while (gt_Bigint(residue, zero)) {
        if (tmp != b) {//printf("\t455 DESTRUCT %lu\n", (unsigned long) tmp); 
            tmp = destruct_Bigint(tmp);}
        divmod = divmod_Bigints(prev_b, residue); //printf("456 CONSTRUCT %lu\n", (unsigned long) divmod);
        tmp = quotient;
        quotient = divmod[0]; //printf("458 CONSTRUCT %lu\n", (unsigned long) divmod[0]);
        //printf("\t459 DESTRUCT %lu\n", (unsigned long) tmp); 
        tmp = destruct_Bigint(tmp);
        tmp = prev_b;
        prev_b = residue;
        if (tmp != b) {//printf("\t462 DESTRUCT %lu\n", (unsigned long) tmp); 
        tmp = destruct_Bigint(tmp);}
        residue = divmod[1]; //printf("463 CONSTRUCT %lu\n", (unsigned long) divmod[1]);
        //printf("\t464 DESTRUCT %lu\n", (unsigned long) divmod); 
        free(divmod); divmod = NULL;
    }
    // printf("466 DESTRUCT %lu\n", (unsigned long) zero); 
    zero = destruct_Bigint(zero); 
    // printf("467 DESTRUCT %lu\n", (unsigned long) quotient); 
    quotient = destruct_Bigint(quotient);
    // printf("468 DESTRUCT %lu\n", (unsigned long) residue); 
    residue = destruct_Bigint(residue);
    // printf("469 KEEP %lu\n", (unsigned long) prev_b);
    return prev_b;
}

long largest_nonzero_bit(struct Bigint* a) {
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, largest nonzero bit\n");}
        return 0;
    }
    eliminate_zeros(a);
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

struct Bigint* bitshift_left_Bigint(struct Bigint* a, unsigned long n) {
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, bitshift_left, a\n");}
        return NULL;
    }
    struct Bigint* b = construct_Bigint();
    struct Entry_long* e = a -> head;
    if (n == 0) {
        // This does come up, so let's speed things up for when it does
        while (e) {
            enqueue_to_Bigint(b, e -> content);
            e = e -> next;
        }
    } else {
        unsigned long digits = n/64, bits = n%64;
        unsigned long n_0, n_1 = 0;
        for (long i=0; i<digits; i++) {
            enqueue_to_Bigint(b, 0);
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
            enqueue_to_Bigint(b, n_0);
            e = e -> next;
        }
        if (n_1) {
            enqueue_to_Bigint(b, n_1);
        }
        eliminate_zeros(b);
    }
    return b;
}

struct Bigint* bitshift_right_Bigint(struct Bigint* a, unsigned long n) {
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, bitshift_right, a\n");}
        return NULL;
    }
    struct Bigint* b = construct_Bigint();
    struct Entry_long* e = a -> head;
    if (n == 0) {
        // This does come up, so let's speed things up for when it does
        while (e) {
            enqueue_to_Bigint(b, e -> content);
            e = e -> next;
        }
    } else {
        unsigned long digits = n/64, bits = n%64;
        unsigned long n_0, n_1 = 0;
        for (long i=0; i<digits; i++) {
            e = e -> next;
        }
        while (e) {
            n_0 = (e -> content) >> bits;
            e = e -> next;
            if (e) {
                n_1 = (e -> content) << (sizeof(long)*8 - bits);
            } else {
                n_1 = 0;
            }
            enqueue_to_Bigint(b, n_0 | n_1);
        }
        if (!(b -> head)) {
            // Make sure b is certain to be nonempty.
            enqueue_to_Bigint(b, 0);
        }
    }
    return b;
}

void eliminate_zeros(struct Bigint* a) {
    if (!check_Bigint_isok(a)) {
        if (DEBUG) {printf("ERROR: Failed contract, eliminate_zeros, a\n");}
        return;
    }
    struct Entry_long* e = a -> tail;
    while ((e -> content) == 0) {
        if (e -> prev) {
            a -> tail = e -> prev;
            a -> tail -> next = NULL;
            destruct_Entry_long(e);
            a -> len--;
            e = a -> tail;
        } else {
            break;
        }
    }
}

bool equal_Bigint(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, equal, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, equal, b\n");}
    if (!a_ok || !b_ok) {return false;}
    if (a -> sign != b -> sign) {
        return false;
    }
    eliminate_zeros(a);
    eliminate_zeros(b);
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

bool lt_Bigint(struct Bigint* a, struct Bigint* b) {
    bool a_ok = check_Bigint_isok(a);
    bool b_ok = check_Bigint_isok(b);
    if (!a_ok && DEBUG) {printf("ERROR: Failed contract, lt, a\n");}
    if (!b_ok && DEBUG) {printf("ERROR: Failed contract, lt, b\n");}
    if (!a_ok || !b_ok) {return false;}
    if (a -> sign == -1 && b -> sign == 1) {
        return true;
    }
    if (a -> sign == 1 && b -> sign == -1) {
        return false;
    }
    eliminate_zeros(a);
    eliminate_zeros(b);
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

bool leq_Bigint(struct Bigint* a, struct Bigint* b) {
    if (equal_Bigint(a, b) || lt_Bigint(a, b)) {
        return true;
    }
    return false;
}

bool gt_Bigint(struct Bigint* a, struct Bigint* b) {
    return lt_Bigint(b, a);
}

bool geq_Bigint(struct Bigint* a, struct Bigint* b) {
    return leq_Bigint(b, a);
}


// Functions for Entry_long
struct Entry_long* construct_Entry_long(unsigned long n) {
    struct Entry_long* e = malloc(sizeof(struct Entry_long));
    e -> content = n;
    e -> next = NULL;
    e -> prev = NULL;
    return e;
}

struct Entry_long* destruct_Entry_long(struct Entry_long* e) {
    struct Entry_long* next = NULL;
    if (e) {
        next = e -> next;
        free(e);
    }
    e = NULL;
    return next;
}

// Functions for Fraction
struct Fraction* construct_frac() {
    struct Bigint* numerator = construct_Bigint();
    struct Bigint* denominator = construct_Bigint();
    struct Fraction* f = malloc(sizeof(struct Fraction));
    f -> numerator = numerator;
    f -> denominator = denominator;
    return f;
}

int main() {
    struct Bigint* a = construct_Bigint();
    struct Bigint* b = construct_Bigint();
    enqueue_to_Bigint(a, (unsigned long) 1375747077639718046);
    enqueue_to_Bigint(a, (unsigned long) 35971873);
    enqueue_to_Bigint(b, (unsigned long) 2044235926460639054);
    enqueue_to_Bigint(b, (unsigned long) 34468510);
    struct Bigint* c = NULL;
    struct Bigint* d = NULL;
    struct Bigint* e = NULL;
    struct Bigint* f = NULL;
    for (long i=0; i<1 << 20; i++) {
        c = gcd_Bigints(a, b);
        c = destruct_Bigint(c);
    }
    c = gcd_Bigints(a, b);
    printf("a: "); print_Bigint(a); printf("\n");
    printf("b: "); print_Bigint(b); printf("\n");
    printf("gcd(a,b): "); print_Bigint(c); printf("\n");
    a = destruct_Bigint(a);
    b = destruct_Bigint(b);
    c = destruct_Bigint(c);
    d = destruct_Bigint(d);
    e = destruct_Bigint(e);
    f = destruct_Bigint(f);
} 