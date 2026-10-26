/* 
NB: This represents a product/pair of the form a(x)b(L) where L=ln(1+x).
*/
#ifndef POLYPAIR_GMP_H
#define POLYPAIR_GMP_H

#include<iostream>

#include<gmpxx.h>
#include"polynomial_gmp.h"

class PolyPair {
    public:
        Polynomial a, b;
        PolyPair();
        PolyPair(Polynomial& new_a, Polynomial& new_b);
    friend bool operator== (const PolyPair& pp, const mpz_class& f);
    friend bool operator== (const mpz_class& f, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const PolyPair& qq);
    friend bool operator!= (const PolyPair& pp, const mpz_class& n);
    friend bool operator!= (const mpz_class& n, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const PolyPair& qq);
    friend PolyPair& operator+ (const PolyPair& pp, const mpz_class& n);
    friend PolyPair& operator+ (const mpz_class& n, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const Polynomial& p);
    friend PolyPair& operator+ (const Polynomial& p, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const PolyPair& qq);
    friend PolyPair& operator- (const PolyPair& pp, const mpz_class& n);
    friend PolyPair& operator- (const mpz_class& n, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const Polynomial& n);
    friend PolyPair& operator- (const Polynomial& n, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const PolyPair& qq);
    friend std::ostream& operator<< (std::ostream& os, const PolyPair& pp);
};

#endif