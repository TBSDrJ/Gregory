#include<iostream>

#include<gmp.h>
#include<gmpxx.h>

#include"polynomial_gmp.h"
#include"polypair_gmp.h"

RationalFn::Polynomial get_a() {
    return this -> ab.a;
}

RationalFn::Polynomial get_b() {
    return this -> ab.b;
}

RationalFn::Polynomial get_c() {
    return this -> cd.c;
}

RationalFn::Polynomial get_d() {
    return this -> cd.d;
}

RationalFn::RationalFn(PolyPair& pp, PolyPair& qq) {
    this -> ab == pp;
    this -> cd == qq;
}

ostream& operator<< (ostream& os; RationalFn& rf) {
    os << rf.ab << " / " << rf.cd;
    return os;
}