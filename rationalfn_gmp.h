#ifndef RATIONALFN_GMP_H
#define RATIONALFN_GMP_H

class RationalFn {
    public:
        PolyPair ab, cd;
        Polynomial get_a();
        Polynomial get_b();
        Polynomial get_c();
        Polynomial get_d();
        RationalFn();
        RationalFn(PolyPair& pp, PolyPair& qq);
        RationalFn(mpz_class& n): 
                RationalFn(PolyPair(n, 1), PolyPair(1, 1)){}
        RationalFn(Polynomial& p): 
                RationalFn(PolyPair(p, 1), PolyPair(1, 1)){}
        RationalFn(PolyPair& pp): 
                RationalFn(pp, PolyPair(1, 1)){}
        RationalFn(mpz_class& m, mpz_class& n): 
                RationalFn(PolyPair(m, n), PolyPair(1, 1)){}
        RationalFn(mpz_class& n, Polynomial& p): 
                RationalFn(PolyPair(n, p), PolyPair(1, 1)){}
        RationalFn(mpz_class& n, PolyPair& pp): 
                RationalFn(PolyPair(n, 1), pp){}
        RationalFn(Polynomial& p, mpz_class& n): 
                RationalFn(PolyPair(p, n), PolyPair(1, 1)){}
        RationalFn(Polynomial& p, Polynomial& q):
                RationalFn(PolyPair(p, q), PolyPair(1, 1)){}
        RationalFn(Polynomial& p, PolyPair& pp): 
                RationalFn(PolyPair(p, 1), pp){}
        RationalFn(mpz_class& l, mpz_class& m, mpz_class& n):
                RationalFn(PolyPair(l, m), PolyPair(n, 1)){}
        RationalFn(mpz_class& m, mpz_class& n, Polynomial& p):
                RationalFn(PolyPair(m, n), PolyPair(p, 1)){}
        RationalFn(mpz_class& m, mpz_class& n, PolyPair pp):
                RationalFn(PolyPair(m, n), pp){}
        RationalFn(mpz_class& m, Polynomial& p, mpz_class& n):
                RationalFn(PolyPair(m, p), PolyPair(n, 1)){}
        RationalFn(mpz_class& n, Polynomial& p, Polynomial& q):
                RationalFn(PolyPair(n, p), PolyPair(q, 1)){}
        RationalFn(mpz_class& n, Polynomial& p, PolyPair& pp):
                RationalFn(PolyPair(n, p), pp){}
        RationalFn(Polynomial& p, mpz_class& m, mpz_class& n):
                RationalFn(PolyPair(p, m), PolyPair(n, 1)){}
        RationalFn(Polynomial& p, mpz_class& n, Polynomial& q):
                RationalFn(PolyPair(p, n), PolyPair(q, 1)){}
        RationalFn(Polynomial& p, mpz_class& n, PolyPair pp):
                RationalFn(PolyPair(p, n), pp){}
        RationalFn(Polynomial& p, Polynomial& q, mpz_class& n):
                RationalFn(PolyPair(p, q), PolyPair(n, 1)){}
        RationalFn(Polynomial& p, Polynomial& q, Polynomial& r):
                RationalFn(PolyPair(p, q), PolyPair(r, 1)){}
        RationalFn(Polynomial& p, Polynomial& q, PolyPair& pp):
                RationalFn(PolyPair(p, p), pp){}
        RationalFn(mpz_class& k, mpz_class& l, mpz_class& m, mpz_class& n):
                RationalFn(PolyPair(k, l), PolyPair(m, n)){}
       
        


};

#endif