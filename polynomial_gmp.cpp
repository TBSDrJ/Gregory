#include<iostream>
#include<vector>
#include<gmpxx.h>
#include"polynomial_gmp.h"

using namespace std;

Polynomial::Polynomial() {
    vector<mpz_class> coeffs = {0};
    this -> coeffs = coeffs;
}

Polynomial::Polynomial(vector<mpz_class> new_coeffs) {
    this -> coeffs = new_coeffs;
}

int Polynomial::deg() {
    int deg = this -> coeffs.size() - 1;
    while (deg > 0 and this -> coeffs[deg] == 0) {
        deg--;
    }
    return deg;
}

ostream& operator<< (ostream& os, const Polynomial& p) {
    bool first = true;
    for (int i=p.coeffs.size()-1; i>=0; i--) {
        if (p.coeffs[i] != 0) {
            if (not first) {
                os << " + ";
            } else {
                first = false;
            }
            if (p.coeffs[i] != 1 or i == 0) {
                os << p.coeffs[i];
            }
            if (i > 0) {
                os << "x";
                if (i > 1) {
                    os << "^" << i;
                }
            }
        }
    }
    if (p.coeffs.size() == 1 and p.coeffs[0] == 0) {
        os << "0";
    }
    return os;
}

int main() {
    cout << "Testing constructor and << overload" << endl;
    Polynomial p;
    cout << "p = " << p << endl;
    vector<mpz_class> q_coeffs = {5, 4, 3, 2, 1};
    Polynomial q(q_coeffs);
    cout << "q = " << q << endl;
    vector<mpz_class> r_coeffs = {1, 0, 1, 0, 0, 0};
    Polynomial r(r_coeffs);
    cout << "r = " << r << endl;
    
    cout << endl << "Testing .deg()" << endl;
    cout << "p.deg()= " << p.deg() << endl;
    cout << "q.deg()= " << q.deg() << endl;
    cout << "r.deg()= " << r.deg() << endl;
}