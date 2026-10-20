#include<iostream>
#include<sstream>
#include<algorithm>
#include<gmpxx.h>
#include"polypair_gmp.h"
#include"polynomial_gmp.h"

using namespace std;

PolyPair::PolyPair() {
    Polynomial a = Polynomial();
    Polynomial b = Polynomial();
    this -> a = a;
    this -> b = b;
}

PolyPair::PolyPair(Polynomial& new_a, Polynomial& new_b) {
    this -> a = new_a;
    this -> b = new_b;
}

ostream& operator<< (ostream& os, PolyPair& ab) {
    string s;
    stringstream t;
    t << ab.b;
    s = t.str();
    replace(s.begin(), s.end(), 'x', 'L');
    os << "(" << ab.a << ")(" << s << ")";
    return os;
}

int main() {
    Polynomial p_0 = Polynomial();
    vector<mpz_class> p_1_coeffs = {1};
    Polynomial p_1 = Polynomial(p_1_coeffs);
    vector<mpz_class> p_2_coeffs = {1, 1};
    Polynomial p_2 = Polynomial(p_2_coeffs);
    vector<mpz_class> p_3_coeffs = {-1, -1};
    Polynomial p_3 = Polynomial(p_3_coeffs);
    vector<mpz_class> p_4_coeffs = {-6, 5, -4, 3, -2, 1};
    Polynomial p_4 = Polynomial(p_4_coeffs);
    vector<mpz_class> p_5_coeffs = {0, 0, 0, 0, 0, 1};
    Polynomial p_5 = Polynomial(p_5_coeffs);

    PolyPair pp_00 = PolyPair();
    cout << "pp_00 = " << pp_00 << endl;
    PolyPair pp_12 = PolyPair(p_1, p_2);
    cout << "pp_12 = " << pp_12 << endl;
    PolyPair pp_21 = PolyPair(p_2, p_1);
    cout << "pp_21 = " << pp_21 << endl;
    PolyPair pp_23 = PolyPair(p_2, p_3);
    cout << "pp_23 = " << pp_23 << endl;
    PolyPair pp_32 = PolyPair(p_3, p_2);
    cout << "pp_32 = " << pp_32 << endl;
    PolyPair pp_24 = PolyPair(p_2, p_4);
    cout << "pp_24 = " << pp_24 << endl;
    PolyPair pp_42 = PolyPair(p_4, p_2);
    cout << "pp_42 = " << pp_42 << endl;
    PolyPair pp_25 = PolyPair(p_2, p_5);
    cout << "pp_25 = " << pp_25 << endl;
    PolyPair pp_52 = PolyPair(p_5, p_2);
    cout << "pp_52 = " << pp_52 << endl;
}