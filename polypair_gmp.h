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
        PolyPair(const mpz_class m, const mpz_class n);
        PolyPair(const mpz_class n, const Polynomial& new_b);
        PolyPair(const Polynomial& new_a, const mpz_class n);
        PolyPair(const Polynomial& new_a, const Polynomial& new_b);
        // This is a test to see if addition/subtracton is possible.
        bool common_factor(const int& n) const;
        bool common_factor(const long& n) const;
        bool common_factor(const mpz_class& n) const;
        bool common_factor(const Polynomial& q) const;
        bool common_factor(const PolyPair& qq) const;
        // We only do this for n=0 because ln(n+1) is only an int there.
        mpz_class eval(mpz_class n);
    friend bool operator== (const PolyPair& pp, const int& n);
    friend bool operator== (const int& n, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const long& n);
    friend bool operator== (const long& n, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const mpz_class& n);
    friend bool operator== (const mpz_class& n, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const PolyPair& pp);
    friend bool operator== (const PolyPair& pp, const PolyPair& qq);
    friend bool operator!= (const PolyPair& pp, const int& n);
    friend bool operator!= (const int& n, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const long& n);
    friend bool operator!= (const long& n, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const mpz_class& n);
    friend bool operator!= (const mpz_class& n, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const PolyPair& pp);
    friend bool operator!= (const PolyPair& pp, const PolyPair& qq);
    // All uses of +/- operators will crash if .common_factor() is false.
    friend PolyPair& operator+ (const PolyPair& pp, const int& n);
    friend PolyPair& operator+ (const int& n, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const long& n);
    friend PolyPair& operator+ (const long& n, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const mpz_class& n);
    friend PolyPair& operator+ (const mpz_class& n, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const Polynomial& p);
    friend PolyPair& operator+ (const Polynomial& p, const PolyPair& pp);
    friend PolyPair& operator+ (const PolyPair& pp, const PolyPair& qq);
    friend PolyPair& operator- (const PolyPair& pp, const int& n);
    friend PolyPair& operator- (const int& n, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const long& n);
    friend PolyPair& operator- (const long& n, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const mpz_class& n);
    friend PolyPair& operator- (const mpz_class& n, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const Polynomial& p);
    friend PolyPair& operator- (const Polynomial& p, const PolyPair& pp);
    friend PolyPair& operator- (const PolyPair& pp, const PolyPair& qq);
    friend PolyPair& operator* (const PolyPair& pp, const int& n);
    friend PolyPair& operator* (const int& n, const PolyPair& pp);
    friend PolyPair& operator* (const PolyPair& pp, const long& n);
    friend PolyPair& operator* (const long& n, const PolyPair& pp);
    friend PolyPair& operator* (const PolyPair& pp, const mpz_class& n);
    friend PolyPair& operator* (const mpz_class& n, const PolyPair& pp);
    friend PolyPair& operator* (const PolyPair& pp, const Polynomial& p);
    friend PolyPair& operator* (const Polynomial& p, const PolyPair& pp);
    friend PolyPair& operator* (const PolyPair& pp, const PolyPair& qq);
    friend std::ostream& operator<< (std::ostream& os, const PolyPair& pp);
};

#endif