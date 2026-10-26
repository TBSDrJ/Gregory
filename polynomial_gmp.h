#ifndef POLYNOMIAL_GMP_H
#define POLYNOMIAL_GMP_H

#include<iostream>
#include<vector>
#include<gmp.h>
#include<gmpxx.h>

class Polynomial {
    public:
        std::vector<mpz_class> coeffs;
        Polynomial();
        Polynomial(mpz_class n);
        Polynomial(std::vector<mpz_class> new_coeffs);
        int deg() const;
        void eliminate_zeros();
        mpz_class eval(mpz_class x);
        Polynomial& der();
        // .proportional returns n/m where m divides all coeffs of p, and n
        //  divides all coeffs of q.
        mpq_class& proportional(const Polynomial& q) const;
    friend std::ostream& operator<< (std::ostream& os, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const int& n);
    friend bool operator== (const int& n, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const long& n);
    friend bool operator== (const long& n, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const mpz_class& n);
    friend bool operator== (const mpz_class& n, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const Polynomial& q);
    friend bool operator!= (const Polynomial& p, const int& n);
    friend bool operator!= (const int& n, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const long& n);
    friend bool operator!= (const long& n, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const mpz_class& n);
    friend bool operator!= (const mpz_class& n, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator+ (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator+ (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator+ (const Polynomial& p, const int& n);
    friend Polynomial& operator+ (const int& n, const Polynomial& p);
    friend Polynomial& operator+ (const Polynomial& p, const long& n);
    friend Polynomial& operator+ (const long& n, const Polynomial& p);
    friend Polynomial& operator+ (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator- (const Polynomial& p, const int& n);
    friend Polynomial& operator- (const int& n, const Polynomial& p);
    friend Polynomial& operator- (const Polynomial& p, const long& n);
    friend Polynomial& operator- (const long& n, const Polynomial& p);
    friend Polynomial& operator- (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator- (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator- (const Polynomial& p, const Polynomial& q);
    // Note: Using mpq_class here allows multiplication by fraction if all
    //  terms will end up as integers.  Will crash the program if any non-
    //  integer coefficients remain after multiplication. 
    // This is generally intented to be used with a fraction found using
    //  frac = p.proportional(q).  In this case p*frac will always be safe.
    friend Polynomial& operator* (const Polynomial& p, const int& n);
    friend Polynomial& operator* (const int& n, const Polynomial& p);
    friend Polynomial& operator* (const Polynomial& p, const long& n);
    friend Polynomial& operator* (const long& n, const Polynomial& p);
    friend Polynomial& operator* (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator* (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator* (const Polynomial& p, const mpq_class& f);
    friend Polynomial& operator* (const mpq_class& f, const Polynomial& p);
    friend Polynomial& operator* (const Polynomial& p, const Polynomial& q);
};


#endif