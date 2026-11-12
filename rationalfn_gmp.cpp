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

Polynomial RationalFn::get_const_a() const {
    return this -> ab.a;
}

Polynomial RationalFn::get_const_b() const {
    return this -> ab.b;
}

Polynomial RationalFn::get_const_c() const {
    return this -> cd.a;
}

Polynomial RationalFn::get_const_d() const {
    return this -> cd.b;
}

RationalFn::RationalFn(const PolyPair& pp, const PolyPair& qq) {
    this -> ab = pp;
    this -> cd = qq;
    // TODO: if qq == 0, throw error
}

bool RationalFn::can_add_sub(const RationalFn& rf_1) {
    return (this -> ab*rf_1.cd).common_factor(this -> cd*rf_1.ab);
}

bool operator== (const RationalFn& rf_0, const mpz_class& n) {
    if (rf_0.get_const_a().deg() != 0) {return false;}
    if (rf_0.get_const_b().deg() != 0) {return false;}
    if (rf_0.get_const_c().deg() != 0) {return false;}
    if (rf_0.get_const_d().deg() != 0) {return false;}
    mpz_class num = rf_0.get_const_a().coeffs[0] * rf_0.get_const_b().coeffs[0],
            den = rf_0.get_const_c().coeffs[0] * rf_0.get_const_d().coeffs[0];
    if (num % den != 0) {return false;}
    return (num/den) == n;
}

bool operator== (const RationalFn& rf_0, const RationalFn& rf_1) {  
    // Do some quick and easy checks before doing more computation.  
    if ((rf_0.ab == rf_1.ab) and (rf_0.cd == rf_1.cd)) {return true;}
    if (rf_0.get_const_a().deg() != rf_1.get_const_a().deg()) {return false;}
    if (rf_0.get_const_b().deg() != rf_1.get_const_b().deg()) {return false;}
    if (rf_0.get_const_c().deg() != rf_1.get_const_c().deg()) {return false;}
    if (rf_0.get_const_d().deg() != rf_1.get_const_d().deg()) {return false;}
    // These all require calculating the gcd of the coeffs of two polynomials. 
    mpq_class a = rf_0.get_const_a().proportional(rf_1.get_const_a());
    if (a == 0) {return false;}
    mpq_class b = rf_0.get_const_b().proportional(rf_1.get_const_b());
    if (b == 0) {return false;}
    mpq_class c = rf_0.get_const_c().proportional(rf_1.get_const_c());
    if (c == 0) {return false;}
    mpq_class d = rf_0.get_const_d().proportional(rf_1.get_const_d());
    if (d == 0) {return false;}
    // Cross-multiply to see if cancellation happens.
    // This is because mpq_class == doesn't recognize equivalent fractions.
    mpz_class num = a.get_num() * b.get_num() * c.get_den() * d.get_den();
    mpz_class den = a.get_den() * b.get_den() * c.get_num() * d.get_num();
    return num == den;
}

bool operator!= (const RationalFn& rf_0, const mpz_class& n) {
    return not(rf_0 == n);
}

bool operator!= (const RationalFn& rf_0, const RationalFn& rf_1) {
    return not(rf_0 == rf_1);
}

RationalFn& operator+ (const RationalFn& rf, const mpz_class& n) {
    return (rf + RationalFn(n));
}

RationalFn& operator+ (const mpz_class& n, const RationalFn& rf) {
    return (rf + RationalFn(n));
}

RationalFn& operator+ (const RationalFn& rf, const Polynomial& p) {
    return (rf + RationalFn(p));
}

RationalFn& operator+ (const Polynomial& p, const RationalFn& rf) {
    return (rf + RationalFn(p));
}

RationalFn& operator+ (const RationalFn& rf, const PolyPair& pp) {
    return (rf + RationalFn(pp));
}

RationalFn& operator+ (const PolyPair& pp, const RationalFn& rf) {
    return (rf + RationalFn(pp));
}

RationalFn& operator+ (const RationalFn& rf_0, const RationalFn& rf_1) {
    RationalFn * result = new RationalFn(); 
    bool found_result = false;
    PolyPair rf_0_ab = PolyPair(rf_0.get_const_a(), rf_0.get_const_b()),
        rf_0_cd = PolyPair(rf_0.get_const_c(), rf_0.get_const_d()),
        rf_1_ab = PolyPair(rf_1.get_const_a(), rf_1.get_const_b()),
        rf_1_cd = PolyPair(rf_1.get_const_c(), rf_1.get_const_d());
    if (rf_0_cd == rf_1_cd) {
        *result = RationalFn(rf_0_ab + rf_1_ab, rf_0_cd);
        found_result = true;
    }
    if (not found_result) {
        *result = RationalFn(rf_0_ab*rf_1_cd + rf_0_cd*rf_1_ab, 
                rf_0_cd*rf_1_cd);
    }
    if (result -> ab == 0) {result -> cd = PolyPair(1, 1);}
    return *result;
}

RationalFn& operator- (const RationalFn& rf, const mpz_class& n) {
    return rf + (-1)*n;
}

RationalFn& operator- (const mpz_class& n, const RationalFn& rf) {
    return (-1)*rf + n;
}

RationalFn& operator- (const RationalFn& rf, const Polynomial& p) {
    return rf + (-1)*p;
}

RationalFn& operator- (const Polynomial& p, const RationalFn& rf) {
    return (-1)*rf + p;
}

RationalFn& operator- (const RationalFn& rf, const PolyPair& pp) {
    return rf + (-1)*pp;
}

RationalFn& operator- (const PolyPair& pp, const RationalFn& rf) {
    return (-1)*rf + pp;
}

RationalFn& operator- (const RationalFn& rf_0, const RationalFn& rf_1) {
    return rf_0 + (-1)*rf_1;
}

RationalFn& operator* (const RationalFn& rf, const mpz_class& n) {
    RationalFn * result = new RationalFn();
    result -> ab = n * rf.ab;
    result -> cd = rf.cd;
    return (*result);
}

RationalFn& operator* (const mpz_class& n, const RationalFn& rf) {
    return rf * n;
}

RationalFn& operator* (const RationalFn& rf, const Polynomial& p) {
    RationalFn * result = new RationalFn();
    result -> ab = p * rf.ab;
    result -> cd = rf.cd;
    return (*result);
}

RationalFn& operator* (const Polynomial& p, const RationalFn& rf) {
    return rf * p;
}

RationalFn& operator* (const RationalFn& rf, const PolyPair& pp) {
    RationalFn * result = new RationalFn();
    result -> ab = pp * rf.ab;
    result -> cd = rf.cd;
    return (*result);
}

RationalFn& operator* (const PolyPair& pp, const RationalFn& rf) {
    return rf * pp;
}

RationalFn& operator* (const RationalFn& rf_0, const RationalFn& rf_1) {
    RationalFn * result = new RationalFn();
    result -> ab = rf_0.ab * rf_1.ab;
    result -> cd = rf_0.cd * rf_1.cd;
    return (*result);
}

ostream& operator<< (ostream& os, RationalFn& rf) {
    os << rf.ab << " / " << rf.cd;
    return os;
}

int main() {
    vector<mpz_class> coeffs = {1, 1};
    Polynomial p_1 = Polynomial(1);
    Polynomial p_2 = Polynomial(coeffs);
    coeffs = {-1, -1};
    Polynomial p_3 = Polynomial(coeffs);
    coeffs = {-6, 5, -4, 3, -2, 1};
    Polynomial p_4 = Polynomial(coeffs);
    coeffs = {0, 0, 0, 0, 0, 1};
    Polynomial p_5 = Polynomial(coeffs);

    PolyPair pp_11 = PolyPair(1, 1);
    PolyPair pp_22 = PolyPair(p_2, p_2);
    PolyPair pp_33 = PolyPair(p_3, p_3);

    // I'll follow the constructors in order instead using subscript order
    // Main constructor
    RationalFn rf_2233 = RationalFn(pp_22, pp_33);
    cout << "rf_2233 = " << rf_2233 << endl;
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
    cout << "rf_5115.ab = " << rf_5115.ab << "  rf_5115.cd = " << rf_5115.cd 
            << endl;
    cout << "rf_5115.get_a() = " << rf_5115.get_a() 
            << "  rf_5115.get_b() = " << rf_5115.get_b()
            << "  rf_5115.get_c() = " << rf_5115.get_c()
            << "  rf_5115.get_d() = " << rf_5115.get_d()
            << endl;

    cout << endl << "Testing Operator ==" << endl;
    cout << "Should all be true:" << endl;
    nrf_1111 = RationalFn(-2, -3, -5, -7);
    RationalFn nrf_1111_2 = RationalFn(6, 1, 35, 1);
    cout << nrf_1111 << " == " << nrf_1111_2 << ": " << (nrf_1111 == nrf_1111_2)
            << endl;
    RationalFn nrf_2522 = RationalFn(-3, -2*p_5, 3*p_2, p_2);
    RationalFn nrf_2533 = RationalFn(6, p_5, p_3, 3*p_3);
    cout << nrf_2522 << " == " << nrf_2533 << ": " 
            << (nrf_2522 == nrf_2533) << endl;
    nrf_1111 = RationalFn(-2, -3);
    cout << nrf_1111 << " == " << 6*pp_11 << ": " << (nrf_1111 == 6*pp_11) 
            << endl;
    cout << nrf_1111 << " == Polynomial(" << 6*p_1 << "): " << 
            (nrf_1111 == 6*p_1) << endl;
    cout << nrf_1111 << " == " << 6 << ": " << (nrf_1111 == 6) << endl;
    // These tests are failing.
    nrf_1111 = RationalFn(2, -3, 1, -2);
    cout << nrf_1111 << " == " << -3 << ": " << (nrf_1111 == 3) << endl;
    nrf_1111 = RationalFn(-12, -3, -30, -7);
    nrf_1111_2 = RationalFn(6, 1, 35, 1);
    cout << nrf_1111 << " == " << nrf_1111_2 << ": " << (nrf_1111 == nrf_1111_2)
            << endl;
    nrf_2533 = RationalFn(2, p_5, p_3, p_3);
    cout << nrf_2522 << " == " << nrf_2533 << ": " 
            << (nrf_2522 == nrf_2533) << endl;
    cout << "Should all be false:" << endl;
    cout << rf_5115 << " == " << rf_5111 << ": " << (rf_5115 == rf_5111) 
            << endl;
    RationalFn rf_5151 = RationalFn(p_5, 1, p_5, 1);
    cout << rf_5115 << " == " << rf_5151 << ": " << (rf_5115 == rf_5151) 
            << endl;
    cout << rf_2311 << " == " << rf_2211 << ": " << (rf_2311 == rf_2211) 
            << endl;
    RationalFn nrf_2522_2 = RationalFn(-3, -2*p_5, 3*p_2, 7*p_2);
    cout << nrf_2522 << " == " << nrf_2522_2 << ": " << (nrf_2522 ==
            nrf_2522_2) << endl;
    nrf_2522_2 = RationalFn(-3, -2*p_4, 3*p_2, p_2);
    cout << nrf_2522 << " == " << nrf_2522_2 << ": " << (nrf_2522 ==
            nrf_2522_2) << endl;

    cout << endl << "Testing Operator !=" << endl;
    cout << "Should all be false:" << endl;
    nrf_1111 = RationalFn(-2, -3, -5, -7);
    nrf_1111_2 = RationalFn(6, 1, 35, 1);
    cout << nrf_1111 << " != " << nrf_1111_2 << ": " << (nrf_1111 != nrf_1111_2)
            << endl;
    nrf_2522 = RationalFn(-3, -2*p_5, 3*p_2, p_2);
    nrf_2533 = RationalFn(6, p_5, p_3, 3*p_3);
    cout << nrf_2522 << " != " << nrf_2533 << ": " 
            << (nrf_2522 != nrf_2533) << endl;
    nrf_1111 = RationalFn(-2, -3);
    cout << nrf_1111 << " != " << 6*pp_11 << ": " << (nrf_1111 != 6*pp_11) 
            << endl;
    cout << nrf_1111 << " != Polynomial(" << 6*p_1 << "): " << 
            (nrf_1111 != 6*p_1) << endl;
    cout << nrf_1111 << " != " << 6 << ": " << (nrf_1111 != 6) << endl;
    // These tests are failing.
    nrf_1111 = RationalFn(2, -3, 1, -2);
    cout << nrf_1111 << " != " << -3 << ": " << (nrf_1111 != 3) << endl;
    nrf_1111 = RationalFn(-12, -3, -30, -7);
    nrf_1111_2 = RationalFn(6, 1, 35, 1);
    cout << nrf_1111 << " != " << nrf_1111_2 << ": " << (nrf_1111 != nrf_1111_2)
            << endl;
    nrf_2533 = RationalFn(2, p_5, p_3, p_3);
    cout << nrf_2522 << " != " << nrf_2533 << ": " 
            << (nrf_2522 != nrf_2533) << endl;
    cout << "Should all be true:" << endl;
    cout << rf_5115 << " != " << rf_5111 << ": " << (rf_5115 != rf_5111) 
            << endl;
    rf_5151 = RationalFn(p_5, 1, p_5, 1);
    cout << rf_5115 << " != " << rf_5151 << ": " << (rf_5115 != rf_5151) 
            << endl;
    cout << rf_2311 << " != " << rf_2211 << ": " << (rf_2311 != rf_2211) 
            << endl;
    nrf_2522_2 = RationalFn(-3, -2*p_5, 3*p_2, 7*p_2);
    cout << nrf_2522 << " != " << nrf_2522_2 << ": " << (nrf_2522 !=
            nrf_2522_2) << endl;
    nrf_2522_2 = RationalFn(-3, -2*p_4, 3*p_2, p_2);
    cout << nrf_2522 << " != " << nrf_2522_2 << ": " << (nrf_2522 !=
            nrf_2522_2) << endl;

    cout << endl << "Testing +:" << endl;
    cout << 1 << " + " << nrf_1111 << ": " << 1 + nrf_1111 << endl;
    cout << nrf_1111 << " + " << 1 << ": " << nrf_1111 + 1 << endl;
    cout << "(" << p_2 << ") + " << rf_1223 << ": " << p_2 + rf_1223 << endl;
    cout << rf_1223 << " + (" << p_2 << "): " << rf_1223 + p_2 << endl;
    cout << pp_11 << " + " << rf_1112 << ": " << pp_11 + rf_1112 << endl;
    cout << rf_1112 << " + " << pp_11 << ": " << rf_1112 + pp_11 << endl;
    cout << rf_2111 << " + " << rf_3111 << ": ";
    if ((rf_2111).can_add_sub(rf_3111)) {cout << rf_2111 + rf_3111;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_2111 << " + " << rf_2111 << ": ";
    if ((rf_2111).can_add_sub(rf_2111)) {cout << rf_2111 + rf_2111;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_2121 << " + " << rf_2131 << ": ";
    if ((rf_2121).can_add_sub(rf_2131)) {cout << rf_2121 + rf_2131;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_5151 << " + " << rf_2111 << ": ";
    if ((rf_5151).can_add_sub(rf_2111)) {cout << rf_5151 + rf_2111;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_5115 << " + " << rf_5111 << ": ";
    if ((rf_5115).can_add_sub(rf_5111)) {cout << rf_5115 + rf_5111;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_5115 << " + " << rf_5115 << ": ";
    if ((rf_5115).can_add_sub(rf_5115)) {cout << rf_5115 + rf_5115;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_2231 << " + " << rf_1311 << ": ";
    if ((rf_2231).can_add_sub(rf_1311)) {cout << rf_2231 + rf_1311;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_1223 << " + " << rf_1231 << ": ";
    if ((rf_1223).can_add_sub(rf_1231)) {cout << rf_1223 + rf_1231;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_1223 << " + " << rf_1311 << ": ";
    if ((rf_1223).can_add_sub(rf_1311)) {cout << rf_1223 + rf_1311;}
    else {cout << "Can't add these.";}
    cout << endl;
    cout << rf_1223 << " + " << rf_2231 << ": ";
    if ((rf_1223).can_add_sub(rf_2231)) {cout << rf_1223 + rf_2231;}
    else {cout << "Can't add these.";}
    cout << endl;
    RationalFn rf_4121 = RationalFn(p_4, 1, p_2);
    RationalFn nrf_5121 = RationalFn((-1)*p_5, 1, p_2);
    cout << rf_4121 << " + " << nrf_5121 << ": ";
    if ((rf_4121).can_add_sub(nrf_5121)) {cout << rf_4121 + nrf_5121;}
    else {cout << "Can't add these.";}
    cout << " (check that leading zeros don't get passed along) ";
    cout << " result.get_a().deg(): " << (rf_4121 + nrf_5121).get_a().deg();
    cout << endl;

    cout << endl << "Testing -:" << endl;
    cout << 1 << " - " << nrf_1111 << ": " << 1 - nrf_1111 << endl;
    cout << nrf_1111 << " - " << 1 << ": " << nrf_1111 - 1 << endl;
    cout << "(" << p_2 << ") - " << rf_1223 << ": " << p_2 - rf_1223 << endl;
    cout << rf_1223 << " - (" << p_2 << "): " << rf_1223 - p_2 << endl;
    cout << pp_11 << " - " << rf_1112 << ": " << pp_11 - rf_1112 << endl;
    cout << rf_1112 << " - " << pp_11 << ": " << rf_1112 - pp_11 << endl;
    cout << rf_2111 << " - " << rf_3111 << ": ";
    if ((rf_2111).can_add_sub(rf_3111)) {cout << rf_2111 - rf_3111;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_2111 << " - " << rf_2111 << ": ";
    if ((rf_2111).can_add_sub(rf_2111)) {cout << rf_2111 - rf_2111;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_2121 << " - " << rf_2131 << ": ";
    if ((rf_2121).can_add_sub(rf_2131)) {cout << rf_2121 - rf_2131;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_5151 << " - " << rf_2111 << ": ";
    if ((rf_5151).can_add_sub(rf_2111)) {cout << rf_5151 - rf_2111;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_5115 << " - " << rf_5111 << ": ";
    if ((rf_5115).can_add_sub(rf_5111)) {cout << rf_5115 - rf_5111;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_5115 << " - " << rf_5115 << ": ";
    if ((rf_5115).can_add_sub(rf_5115)) {cout << rf_5115 - rf_5115;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_2231 << " - " << rf_1311 << ": ";
    if ((rf_2231).can_add_sub(rf_1311)) {cout << rf_2231 - rf_1311;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_1223 << " - " << rf_1231 << ": ";
    if ((rf_1223).can_add_sub(rf_1231)) {cout << rf_1223 - rf_1231;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_1223 << " - " << rf_1311 << ": ";
    if ((rf_1223).can_add_sub(rf_1311)) {cout << rf_1223 - rf_1311;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    cout << rf_1223 << " - " << rf_2231 << ": ";
    if ((rf_1223).can_add_sub(rf_2231)) {cout << rf_1223 - rf_2231;}
    else {cout << "Can't subtract these.";}
    cout << endl;
    rf_4121 = RationalFn(p_4, 1, p_2);
    RationalFn rf_5121 = RationalFn(p_5, 1, p_2);
    cout << rf_4121 << " - " << rf_5121 << ": ";
    if ((rf_4121).can_add_sub(rf_5121)) {cout << rf_4121 - rf_5121;}
    else {cout << "Can't subtract these.";}
    cout << " (check that leading zeros don't get passed along) ";
    cout << " result.get_a().deg(): " << (rf_4121 - rf_5121).get_a().deg();
    cout << endl;

    cout << endl << "Testing *:" << endl;
    cout << "(" << 3 << ")*" << rf_2233 << ": " << 3*rf_2233 << endl;
    cout << rf_2233 << "*(" << 3 << "): " << 3*rf_2233 << endl;
    cout << "(" << p_3 << ")*" << rf_2233 << ": " << p_3*rf_2233 << endl;
    cout << rf_2233 << "*(" << p_3 << "): " << p_3*rf_2233 << endl;
    cout << pp_33 << "*" << rf_2233 << ": " << pp_33*rf_2233 << endl;
    cout << rf_2233 << "*" << pp_33 << ": " << pp_33*rf_2233 << endl;
    cout << rf_2233 << "*" << rf_5121 << ": " << rf_2233*rf_5121 << endl;
}