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
    vector<mpz_class> p_2_coeffs = {0, 0, 0, 0, 0, 1};
    Polynomial p_2 = Polynomial(p_2_coeffs);

    PolyPair pp_00 = PolyPair();
    cout << "pp_00 = " << pp_00 << endl;
    PolyPair pp_12 = PolyPair(p_1, p_2);
    cout << "pp_12 = " << pp_12 << endl;
    PolyPair pp_21 = PolyPair(p_2, p_1);
    cout << "pp_21 = " << pp_21 << endl;
}