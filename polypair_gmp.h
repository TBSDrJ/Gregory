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
    friend std::ostream& operator<< (std::ostream& os, PolyPair& ab);

};

#endif