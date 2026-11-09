#include<iostream>
#include<vector>

#include<gmp.h>
#include<gmpxx.h>

#include"polynomial_gmp.h"
#include"polypair_gmp.h"
#include"rationalfn_gmp.h"

using namespace std;

Polynomial RationalFn::get_a() {
    return this -> ab.a;
}

Polynomial RationalFn::get_b() {
    return this -> ab.b;
}

Polynomial RationalFn::get_c() {
    return this -> cd.a;
}

Polynomial RationalFn::get_d() {
    return this -> cd.b;
}

RationalFn::RationalFn(const PolyPair& pp, const PolyPair& qq) {
    this -> ab = pp;
    this -> cd = qq;
    // TODO: if qq == 0, throw error
}

ostream& operator<< (ostream& os, RationalFn& rf) {
    os << rf.ab << " / " << rf.cd;
    return os;
}

int main() {
    vector<mpz_class> coeffs = {1, 1};
    Polynomial p_2 = Polynomial(coeffs);
    coeffs = {-1, -1};
    Polynomial p_3 = Polynomial(coeffs);
    coeffs = {-6, 5, -4, 3, -2, 1};
    Polynomial p_4 = Polynomial(coeffs);
    coeffs = {0, 0, 0, 0, 0, 1};
    Polynomial p_5 = Polynomial(coeffs);

    PolyPair pp_22 = PolyPair(p_2, p_2);
    PolyPair pp_33 = PolyPair(p_3, p_3);

    // I'll follow the constructors in order instead using subscript order
    // Main constructor
    RationalFn rf_2233 = RationalFn(pp_22, pp_33);
    cout << "rf_0000 = " << rf_2233 << endl;
    // No inputs
    RationalFn rf_0000 = RationalFn();
    cout << "rf_0000 = " << rf_0000 << endl;
    // One input
    RationalFn rf_1111 = RationalFn(1);
    cout << "rf_1111 = " << rf_1111 << endl;
    RationalFn rf_2111 = RationalFn(p_2);
    cout << "rf_2111 = " << rf_2111 << endl;
    RationalFn rf_2211 = RationalFn(pp_22);
    cout << "rf_2211 = " << rf_2211 << endl;
    // Two inputs
    RationalFn nrf_1111 = RationalFn(3, -5);
    cout << "nrf_1111 = " << nrf_1111 << endl;
    RationalFn rf_1211 = RationalFn(1, p_2);
    cout << "rf_1211 = " << rf_1211 << endl;
    RationalFn rf_1122 = RationalFn(1, pp_22);
    cout << "rf_1122 = " << rf_1122 << endl;
    RationalFn rf_3111 = RationalFn(p_3, 1);
    cout << "rf_3111 = " << rf_3111 << endl;
    RationalFn rf_2311 = RationalFn(p_2, p_3);
    cout << "rf_2311 = " << rf_2311 << endl;
    RationalFn rf_3122 = RationalFn(p_3,pp_22);
    cout << "rf_3122 = " << rf_3122 << endl;
    // Three inputs
    nrf_1111 = RationalFn(3, -5, 7);
    cout << "nrf_1111 = " << nrf_1111 << endl;
    RationalFn rf_1121 = RationalFn(1, 1, p_2);
    cout << "rf_1121 = " << rf_1121 << endl;
    RationalFn rf_1133 = RationalFn(1, 1, pp_33);
    cout << "rf_1133 = " << rf_1133 << endl;
    RationalFn rf_1411 = RationalFn(1, p_4, 1);
    cout << "rf_1411 = " << rf_1411 << endl;
    RationalFn rf_1221 = RationalFn(1, p_2, p_2);
    cout << "rf_1221 = " << rf_1221 << endl;
    RationalFn rf_1222 = RationalFn(1, p_2, pp_22);
    cout << "rf_1222 = " << rf_1222 << endl;
    RationalFn rf_4111 = RationalFn(p_4, 1, 1);
    cout << "rf_4111 = " << rf_4111 << endl;
    RationalFn rf_2121 = RationalFn(p_2, 1, p_2);
    cout << "rf_2121 = " << rf_2121 << endl;
    RationalFn rf_2122 = RationalFn(p_2, 1, pp_22);
    cout << "rf_2122 = " << rf_2122 << endl;
    RationalFn rf_3211 = RationalFn(p_3, p_2, 1);
    cout << "rf_3211 = " << rf_3211 << endl;
    RationalFn rf_2221 = RationalFn(p_2, p_2, p_2);
    cout << "rf_2221 = " << rf_2221 << endl;
    RationalFn rf_2222 = RationalFn(p_2, p_2, pp_22);
    cout << "rf_2222 = " << rf_2222 << endl;
    // Four inputs
    nrf_1111 = RationalFn(-2,-3,-5,-7);
    cout << "nrf_1111 = " << nrf_1111 << endl;
    RationalFn rf_5111 = RationalFn(p_5, 1, 1, 1);
    cout << "rf_5111 = " << rf_5111 << endl;
    RationalFn rf_1311 = RationalFn(1, p_3, 1, 1);
    cout << "rf_1311 = " << rf_1311 << endl;
    RationalFn rf_1131 = RationalFn(1, 1, p_3, 1);
    cout << "rf_1131 = " << rf_1131 << endl;
    RationalFn rf_1112 = RationalFn(1, 1, 1, p_2);
    cout << "rf_1112 = " << rf_1112 << endl;
    RationalFn rf_3311 = RationalFn(p_3, p_3, 1, 1);
    cout << "rf_3311 = " << rf_3311 << endl;
    RationalFn rf_2131 = RationalFn(p_2, 1, p_3, 1);
    cout << "rf_2131 = " << rf_2131 << endl;
    RationalFn rf_2112 = RationalFn(p_2, 1, 1, p_2);
    cout << "rf_2112 = " << rf_2112 << endl;
    RationalFn rf_1231 = RationalFn(1, p_2, p_3, 1);
    cout << "rf_1231 = " << rf_1231 << endl;
    RationalFn rf_1212 = RationalFn(1, p_2, 1, p_2);
    cout << "rf_1212 = " << rf_1212 << endl;
    RationalFn rf_1123 = RationalFn(1, 1, p_2, p_3);
    cout << "rf_1123 = " << rf_1123 << endl;
    RationalFn rf_1223 = RationalFn(1, p_2, p_2, p_3);
    cout << "rf_1223 = " << rf_1223 << endl;
    RationalFn rf_2123 = RationalFn(p_2, 1, p_2, p_3);
    cout << "rf_2123 = " << rf_2123 << endl;
    RationalFn rf_2212 = RationalFn(p_2, p_2, 1, p_2);
    cout << "rf_2212 = " << rf_2212 << endl;
    RationalFn rf_2231 = RationalFn(p_2, p_2, p_3, 1);
    cout << "rf_2231 = " << rf_2231 << endl;
    RationalFn rf_2223 = RationalFn(p_2, p_2, p_2, p_3);
    cout << "rf_2223 = " << rf_2223 << endl;
    RationalFn rf_5115 = RationalFn(p_5, 1, 1, p_5);
    cout << "rf_5115 = " << rf_5115 << endl;
}