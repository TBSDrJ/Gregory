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
        Polynomial(std::vector<mpz_class> new_coeffs);
        int deg() const;
        void eliminate_zeros();
        mpz_class eval(mpz_class x);
        Polynomial& der();
        mpq_class& proportional(const Polynomial& q);
    friend std::ostream& operator<< (std::ostream& os, const Polynomial& p);
    friend bool operator== (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator+ (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator- (const Polynomial& p, const Polynomial& q);
    friend Polynomial& operator* (const Polynomial& p, const Polynomial& q);
};


#endif