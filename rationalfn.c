#include"rationalfn.h"

// If you have p(x)/q(x) with p(0) = 0 and q(0) = 0, cancel x until 
// either p(0) != 0 or q(0) != 0.
/* TODO: This seems to still have issues.  For ex, I sent in p = -3x^2,
        q = x^6, and it reduced to -3x, x^5 instead of -3, x^4.
        Similarly, it took -4x^3, x^8, and only reduced once to -4x^2, x^7.    
*/
void Rationalfn_cancel_zeros(struct Polynomial* p, struct Polynomial* q) {
    struct Myint* p_at_0; struct Myint* q_at_0;
    struct Myint* tmp;
    struct Myint* zero = Myint_constructor();
    p_at_0 = Polynomial_subs(p, 0);
    q_at_0 = Polynomial_subs(q, 0);
    while (Myint_equal(p_at_0, zero) && Myint_equal(q_at_0, zero)) {
        // Notice that p,q -> coeffs[0] = 0 if we're here.
        tmp = p -> coeffs[0];
        for (int i=0; i<(p -> degree); i++) {
            p -> coeffs[i] = p -> coeffs[i+1];
        }
        tmp = Myint_destructor(tmp);
        p -> coeffs[p -> degree] = zero;
        tmp = q -> coeffs[0];
        for (int i=0; i<(q -> degree); i++) {
            q -> coeffs[i] = q -> coeffs[i+1];
        }
        tmp = Myint_destructor(tmp);
        q -> coeffs[q -> degree] = zero;
        Polynomial_reduce(p);
        Polynomial_reduce(q);
        tmp = p_at_0;
        p_at_0 = Polynomial_subs(p, 0);
        tmp = Myint_destructor(tmp);
        tmp = p_at_0;
        q_at_0 = Polynomial_subs(q, 0);
        tmp = Myint_destructor(tmp);
    }
}

// This assumes that we have a(x)b(L) / c(x)d(L), where L = ln(x+1), and
//    that we get 0/0 when we Polynomial_substitute x=0.
// TODO: This seems to have issues, I am getting segfaults when I use this.
struct Myint* Rationalfn_l_hôpitals_1 (struct Polynomial* a, 
        struct Polynomial* b, struct Polynomial* c, struct Polynomial* d) {
    struct Myint* numer_eval = Myint_constructor();
    struct Myint* denom_eval = Myint_constructor();
    struct Queue_2* numer_in = Queue_2_constructor();
    struct Queue_2* denom_in = Queue_2_constructor();
    struct Queue_2* numer_out = Queue_2_constructor();
    struct Queue_2* denom_out = Queue_2_constructor();
    struct Polynomial* da;
    struct Polynomial* db;
    struct Entry_2* e;
    Queue_2_enqueue(numer_in, a, b);
    Queue_2_enqueue(denom_in, c, d);
    struct Polynomial* x_plus_1 = Polynomial_constructor(1);
    struct Myint* one = Myint_constructor(); one -> my_long = 1;
    struct Myint* zero = Myint_constructor(); 
    struct Myint* myint_tmp = NULL;
    struct Myint* myint_tmp_2 = NULL;
    x_plus_1 -> coeffs[0] = one;
    x_plus_1 -> coeffs[1] = one;
    while ((Myint_equal(numer_eval, zero)) && (Myint_equal(denom_eval, zero))) {
        e = numer_in -> head;
        while (e) {
            db = Polynomial_der(e -> b);
            da = Polynomial_multiply(x_plus_1, Polynomial_der(e -> a));
            printf("Num: "); Myint_print(Polynomial_subs(e -> a, 0)); 
            printf(" "); Myint_print(Polynomial_subs(db, 0)); 
            printf(" "); Myint_print(Polynomial_subs(da, 0));  
            printf(" "); Myint_print(Polynomial_subs(e -> b, 0)); printf("\n");
            myint_tmp = Myint_multiply(Polynomial_subs(e -> a, 0), 
                    Polynomial_subs(db, 0));
            myint_tmp_2 = Myint_add(numer_eval, myint_tmp);
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp = numer_eval;
            numer_eval = myint_tmp_2;
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp_2 = NULL;
            myint_tmp = Myint_multiply(Polynomial_subs(da, 0), 
                    Polynomial_subs(e -> b, 0));
            myint_tmp_2 = Myint_add(numer_eval, myint_tmp);
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp = numer_eval;
            numer_eval = myint_tmp_2;
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp_2 = NULL;
            Queue_2_enqueue(numer_out, Polynomial_deepcopy(e -> a), db);
            Queue_2_enqueue(numer_out, da, Polynomial_deepcopy(e -> b));
            e = e -> next;
        }
        e = denom_in -> head;
        while (e) {
            db = Polynomial_der(e -> b);
            da = Polynomial_multiply(x_plus_1, Polynomial_der(e -> a));
            printf("Den: "); Myint_print(Polynomial_subs(e -> a, 0)); 
            printf(" "); Myint_print(Polynomial_subs(db, 0));
            printf(" "); Myint_print(Polynomial_subs(da, 0));
            printf(" "); Myint_print(Polynomial_subs(e -> b, 0)); printf("\n");
            myint_tmp = Myint_multiply(Polynomial_subs(e -> a, 0), 
                    Polynomial_subs(db, 0));
            myint_tmp_2 = Myint_add(denom_eval, myint_tmp);
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp = denom_eval;
            denom_eval = myint_tmp_2;
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp_2 = NULL;
            myint_tmp = Myint_multiply(Polynomial_subs(da, 0), 
                    Polynomial_subs(e -> b, 0));
            myint_tmp_2 = Myint_add(denom_eval, myint_tmp);
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp = denom_eval;
            denom_eval = myint_tmp_2;
            myint_tmp = Myint_destructor(myint_tmp);
            myint_tmp_2 = NULL;
            Queue_2_enqueue(denom_out, Polynomial_deepcopy(e -> a), 
                    Polynomial_der(e -> b));
            Queue_2_enqueue(denom_out, Polynomial_multiply(x_plus_1, 
                    Polynomial_der(e -> a)), Polynomial_deepcopy(e -> b));
            e = e -> next;
        }
        Queue_2_destructor(numer_in);
        Queue_2_destructor(denom_in);
        numer_in = numer_out;
        denom_in = denom_out;
        numer_out = Queue_2_constructor();
        denom_out = Queue_2_constructor();
    }
    Queue_2_destructor(numer_in);
    Queue_2_destructor(denom_in);
    Queue_2_destructor(numer_out);
    Queue_2_destructor(denom_out);
    Myint_print(numer_eval); printf(" / "); 
            Myint_print(denom_eval); printf("\n");
    return 0;
}
