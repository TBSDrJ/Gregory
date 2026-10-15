#ifndef POLYNOMIAL_GMP_H
#define POLYNOMIAL_GMP_H

#include<iostream>
#include<vector>
#include<gmpxx.h>

class Polynomial {
    public:
        std::vector<mpz_class> coeffs;
        Polynomial();
        Polynomial(std::vector<mpz_class> new_coeffs);
        int deg();
    friend std::ostream& operator<< (std::ostream& os, const Polynomial& p);
    friend Polynomial& operator+ (const Polynomial& p, const Polynomial& q);
};


#endif