#ifndef POLYNOMIAL_GMP_H
#define POLYNOMIAL_GMP_H

#include<iostream>
#include<vector>
#include<gmp.h>
#include<gmpxx.h>

class Polynomial {
    public:
        std::vector<mpz_class> coeffs;
        Polynomial(): Polynomial(0){}
        Polynomial(mpz_class n);
        Polynomial(std::vector<mpz_class> new_coeffs);
        int deg() const;
        void eliminate_zeros();
        mpz_class eval(mpz_class x);
        Polynomial& der();
        mpz_class& gcd() const;
        // .proportional returns n/m where m divides all coeffs of p, and n
        //  divides all coeffs of q.
        mpq_class& proportional(const Polynomial& q) const;
    friend std::ostream& operator<< (std::ostream& os, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const mpz_class& n);
    friend bool operator== (const mpz_class& n, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const Polynomial& q);
    friend bool operator!= (const Polynomial& p, const mpz_class& n);
    friend bool operator!= (const mpz_class& n, const Polynomial& p);
    friend bool operator!= (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator+ (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator+ (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator+ (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator- (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator- (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator- (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator* (const Polynomial& p, const mpz_class& n);
    friend Polynomial& operator* (const mpz_class& n, const Polynomial& p);
    friend Polynomial& operator* (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator/ (const Polynomial& p, const mpz_class n);
    friend Polynomial& operator/ (const Polynomial& p, const Polynomial& q);
};


#endif