#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"polynomials.h"
#include"queue_2.h"

#define n 1

// If you have p(x)/q(x) with p(0) = 0 and q(0) = 0, cancel x until 
// either p(0) != 0 or q(0) != 0.
void Polynomial_cancel(struct Polynomial* p, struct Polynomial* q) {
    long p_0, q_0;
    p_0 = Polynomial_subs(p, 0);
    q_0 = Polynomial_subs(q, 0);
    while ((p_0 == 0) && (q_0 == 0)) {
        // Notice that p,q -> coeffs[0] = 0 if we're here.
        for (int i=0; i<(p -> degree); i++) {
            p -> coeffs[i] = p -> coeffs[i+1];
        }
        p -> coeffs[p -> degree] = 0;
        for (int i=0; i<(q -> degree); i++) {
            q -> coeffs[i] = q -> coeffs[i+1];
        }
        q -> coeffs[q -> degree] = 0;
        p = Polynomial_reduce(p);
        q = Polynomial_reduce(q);
        p_0 = Polynomial_subs(p, 0);
        q_0 = Polynomial_subs(q, 0);
    }
}

// This asPolynomial_addes that we have a(x)b(L) / c(x)d(L), where L = ln(x+1), and
//    that we get 0/0 when we Polynomial_substitute x=0.
long l_hôpitals (struct Polynomial* a, struct Polynomial* b, 
        struct Polynomial* c, struct Polynomial* d) {
    long numer_eval = 0, denom_eval = 0;
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
    x_plus_1 -> coeffs[0] = 1;
    x_plus_1 -> coeffs[1] = 1;
    while ((numer_eval == 0) && (denom_eval == 0)) {
        e = numer_in -> head;
        while (e) {
            db = Polynomial_der(e -> b);
            da = Polynomial_multiply(x_plus_1, Polynomial_der(e -> a));
            printf("Num:%5li%5li%5li%5li\n", Polynomial_subs(e -> a, 0), Polynomial_subs(db, 0), 
                    Polynomial_subs(da, 0), Polynomial_subs(e -> b, 0));
            numer_eval += Polynomial_subs(e -> a, 0) * Polynomial_subs(db, 0);
            numer_eval += Polynomial_subs(da, 0) * Polynomial_subs(e -> b, 0);
            Queue_2_enqueue(numer_out, e -> a, db);
            Queue_2_enqueue(numer_out, da, e -> b);
            e = e -> next;
        }
        e = denom_in -> head;
        while (e) {
            db = Polynomial_der(e -> b);
            da = Polynomial_multiply(x_plus_1, Polynomial_der(e -> a));
            printf("Den:%5li%5li%5li%5li\n", Polynomial_subs(e -> a, 0), Polynomial_subs(db, 0), 
                    Polynomial_subs(da, 0), Polynomial_subs(e -> b, 0));
            denom_eval += Polynomial_subs(e -> a, 0) * Polynomial_subs(db, 0);
            denom_eval += Polynomial_subs(da, 0) * Polynomial_subs(e -> b, 0);
            Queue_2_enqueue(denom_out, e -> a, Polynomial_der(e -> b));
            Queue_2_enqueue(denom_out, Polynomial_multiply(x_plus_1, Polynomial_der(e -> a)), e -> b);
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
    printf("%5li%5li\n", numer_eval, denom_eval);
    return 0;
}

int main() {
    // Initialize: a=x^n, b=1, c=1, d=x^n
    // a and c are polynomials in x
    // b and d are polynomials in L = ln(x+1)
    // We start with (a(x)b(L))/(c(x)d(L))
    struct Polynomial* a; 
    struct Polynomial* b;
    struct Polynomial* c;
    struct Polynomial* d; 
    struct Polynomial* da_0; 
    struct Polynomial* db_0; 
    struct Polynomial* dc_0;
    struct Polynomial* dd_0;
    struct Polynomial* da_1; 
    struct Polynomial* db_1; 
    struct Polynomial* dc_1;
    struct Polynomial* dd_1;
    struct Polynomial* x_plus_1;
    a = Polynomial_constructor(n);
    a -> coeffs[n] = 1;
    for (int i=0; i<n; i++) {a -> coeffs[i] = 0;}
    b = Polynomial_constructor(0);
    b -> coeffs[0] = 1;
    c = Polynomial_constructor(0);
    c -> coeffs[0] = 1;
    d = Polynomial_constructor(n);
    d -> coeffs[n] = 1;
    for (int i=0; i<n; i++) {d -> coeffs[i] = 0;}
    x_plus_1 = Polynomial_constructor(1);
    x_plus_1 -> coeffs[0] = 1;
    x_plus_1 -> coeffs[1] = 1;
    // Calculate first derivative
    da_0 = Polynomial_add(Polynomial_multiply(Polynomial_der(a), c), Polynomial_neg(Polynomial_multiply(a, Polynomial_der(c))));
    db_0 = b;
    dc_0 = Polynomial_multiply(c, c);
    dd_0 = d;
    da_1 = a;
    db_1 = Polynomial_add(Polynomial_multiply(Polynomial_der(b), d), Polynomial_neg(Polynomial_multiply(b, Polynomial_der(d))));
    dc_1 = Polynomial_multiply(x_plus_1, c);
    dd_1 = Polynomial_multiply(d, d);
    Polynomial_cancel(da_0, dc_0);
    Polynomial_cancel(db_0, dd_0);
    Polynomial_cancel(da_1, dc_1);
    Polynomial_cancel(db_1, dd_1);
    printf("da_0: "); Polynomial_print(da_0); printf("\n");
    printf("db_0: "); Polynomial_print(db_0); printf("\n");
    printf("dc_0: "); Polynomial_print(dc_0); printf("\n");
    printf("dd_0: "); Polynomial_print(dd_0); printf("\n");
    printf("da_1: "); Polynomial_print(da_1); printf("\n");
    printf("db_1: "); Polynomial_print(db_1); printf("\n");
    printf("dc_1: "); Polynomial_print(dc_1); printf("\n");
    printf("dd_1: "); Polynomial_print(dd_1); printf("\n");
    printf("term0: %li/%li\n", Polynomial_subs(da_0, 0)*Polynomial_subs(db_0, 0), Polynomial_subs(dc_0, 0)*Polynomial_subs(dd_0, 0));
    printf("term1: %li/%li\n", Polynomial_subs(da_1, 0)*Polynomial_subs(db_1, 0), Polynomial_subs(dc_1, 0)*Polynomial_subs(dd_1, 0));
    
}