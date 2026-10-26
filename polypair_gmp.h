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
        PolyPair(mpz_class m, mpz_class n);
        PolyPair(mpz_class n, Polynomial& new_b);
        PolyPair(Polynomial& new_a, mpz_class n);
        PolyPair(Polynomial& new_a, Polynomial& new_b);
        // This is a test to see if addition/subtracton is possible.
        bool common_factor(const PolyPair& qq) const;
        bool common_factor(const Polynomial& q) const;
        bool common_factor(const mpz_class& n) const;
    friend bool operator== (const PolyPair& pp, const mpz_class& n);
    friend bool operator== (const mpz_class& n, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const PolyPair& qq);
    friend bool operator!= (const PolyPair& pp, const mpz_class& n);
    friend bool operator!= (const mpz_class& n, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const PolyPair& qq);
    // All uses of +/- operators will crash if .common_factor() is false.
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