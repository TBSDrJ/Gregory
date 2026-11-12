#ifndef RATIONALFN_GMP_H
#define RATIONALFN_GMP_H

class RationalFn {
    public:
        PolyPair ab, cd;
        Polynomial get_a();
        Polynomial get_b();
        Polynomial get_c();
        Polynomial get_d();
        Polynomial get_const_a() const;
        Polynomial get_const_b() const;
        Polynomial get_const_c() const;
        Polynomial get_const_d() const;
        RationalFn(const PolyPair& pp, const PolyPair& qq);
        RationalFn():
                RationalFn(PolyPair(), PolyPair()){}
        RationalFn(const mpz_class& n): 
                RationalFn(PolyPair(n, 1), PolyPair(1, 1)){}
        RationalFn(const Polynomial& p): 
                RationalFn(PolyPair(p, 1), PolyPair(1, 1)){}
        RationalFn(const PolyPair& pp): 
                RationalFn(pp, PolyPair(1, 1)){}
        RationalFn(const mpz_class& m, const mpz_class& n): 
                RationalFn(PolyPair(m, n), PolyPair(1, 1)){}
        RationalFn(const mpz_class& n, Polynomial& p): 
                RationalFn(PolyPair(n, p), PolyPair(1, 1)){}
        RationalFn(const mpz_class& n, PolyPair& pp): 
                RationalFn(PolyPair(n, 1), pp){}
        RationalFn(Polynomial& p, const mpz_class& n): 
                RationalFn(PolyPair(p, n), PolyPair(1, 1)){}
        RationalFn(Polynomial& p, Polynomial& q):
                RationalFn(PolyPair(p, q), PolyPair(1, 1)){}
        RationalFn(Polynomial& p, PolyPair& pp): 
                RationalFn(PolyPair(p, 1), pp){}
        RationalFn(const mpz_class& l, const mpz_class& m, const mpz_class& n):
                RationalFn(PolyPair(l, m), PolyPair(n, 1)){}
        RationalFn(const mpz_class& m, const mpz_class& n, Polynomial& p):
                RationalFn(PolyPair(m, n), PolyPair(p, 1)){}
        RationalFn(const mpz_class& m, const mpz_class& n, PolyPair pp):
                RationalFn(PolyPair(m, n), pp){}
        RationalFn(const mpz_class& m, Polynomial& p, const mpz_class& n):
                RationalFn(PolyPair(m, p), PolyPair(n, 1)){}
        RationalFn(const mpz_class& n, Polynomial& p, Polynomial& q):
                RationalFn(PolyPair(n, p), PolyPair(q, 1)){}
        RationalFn(const mpz_class& n, Polynomial& p, PolyPair& pp):
                RationalFn(PolyPair(n, p), pp){}
        RationalFn(Polynomial& p, const mpz_class& m, const mpz_class& n):
                RationalFn(PolyPair(p, m), PolyPair(n, 1)){}
        RationalFn(Polynomial& p, const mpz_class& n, Polynomial& q):
                RationalFn(PolyPair(p, n), PolyPair(q, 1)){}
        RationalFn(Polynomial& p, const mpz_class& n, PolyPair pp):
                RationalFn(PolyPair(p, n), pp){}
        RationalFn(Polynomial& p, Polynomial& q, const mpz_class& n):
                RationalFn(PolyPair(p, q), PolyPair(n, 1)){}
        RationalFn(Polynomial& p, Polynomial& q, Polynomial& r):
                RationalFn(PolyPair(p, q), PolyPair(r, 1)){}
        RationalFn(Polynomial& p, Polynomial& q, PolyPair& pp):
                RationalFn(PolyPair(p, p), pp){}
        RationalFn(const mpz_class& k, const mpz_class& l, const mpz_class& m, 
                const mpz_class& n):
                RationalFn(PolyPair(k, l), PolyPair(m, n)){}
        RationalFn(Polynomial& p, const mpz_class& l, const mpz_class& m, 
                const mpz_class& n):
                RationalFn(PolyPair(p, l), PolyPair(m, n)){}
        RationalFn(const mpz_class& l, Polynomial& p, const mpz_class& m, 
                const mpz_class& n):
                RationalFn(PolyPair(l, p), PolyPair(m, n)){}
        RationalFn(const mpz_class& l, const mpz_class& m, Polynomial& p, 
                const mpz_class& n):
                RationalFn(PolyPair(l, m), PolyPair(p, n)){}
        RationalFn(const mpz_class& l, const mpz_class& m, const mpz_class& n, 
                Polynomial& p):
                RationalFn(PolyPair(l, m), PolyPair(n, p)){}
        RationalFn(Polynomial& p, Polynomial& q, const mpz_class& m, 
                const mpz_class& n):
                RationalFn(PolyPair(p, q), PolyPair(m, n)){}
        RationalFn(Polynomial& p, const mpz_class& m, Polynomial& q, 
                const mpz_class& n):
                RationalFn(PolyPair(p, m), PolyPair(q, n)){}
        RationalFn(Polynomial& p, const mpz_class& m, const mpz_class& n, 
                Polynomial& q):
                RationalFn(PolyPair(p, m), PolyPair(n, q)){}
        RationalFn(const mpz_class& m, Polynomial& p, Polynomial& q, 
                const mpz_class& n):
                RationalFn(PolyPair(m, p), PolyPair(q, n)){}
        RationalFn(const mpz_class& m, Polynomial& p, const mpz_class& n, 
                Polynomial& q):
                RationalFn(PolyPair(m, p), PolyPair(n, q)){}
        RationalFn(const mpz_class& m, const mpz_class& n, Polynomial& p, 
                Polynomial& q):
                RationalFn(PolyPair(m, n), PolyPair(p, q)){}
        RationalFn(const mpz_class& n, Polynomial& p, Polynomial& q, 
                Polynomial& r):
                RationalFn(PolyPair(n, p), PolyPair(q, r)){}
        RationalFn(Polynomial& p, const mpz_class& n, Polynomial& q, 
                Polynomial& r):
                RationalFn(PolyPair(p, n), PolyPair(q, r)){}
        RationalFn(Polynomial& p, Polynomial& q, const mpz_class& n, 
                Polynomial& r):
                RationalFn(PolyPair(p, q), PolyPair(n, r)){}
        RationalFn(Polynomial& p, Polynomial& q, Polynomial& r, 
                const mpz_class& n):
                RationalFn(PolyPair(p, q), PolyPair(r, n)){}
        RationalFn(Polynomial& p, Polynomial& q, Polynomial& r, Polynomial& s):
                RationalFn(PolyPair(p, q), PolyPair(r, s)){}

    friend bool operator== (const RationalFn& rf_0, const mpz_class& n);
    friend bool operator== (const RationalFn& rf_0, const RationalFn& rf_1);
    friend std::ostream& operator<< (std::ostream& os, RationalFn& rf);

};

#endif