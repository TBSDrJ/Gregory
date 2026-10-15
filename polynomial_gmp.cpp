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

int Polynomial::deg() const {
    int deg = this -> coeffs.size() - 1;
    while (deg > 0 and this -> coeffs[deg] == 0) {
        deg--;
    }
    return deg;
}

void Polynomial::eliminate_zeros() {
    int sz = this -> coeffs.size();
    while (sz > 1) {
        if (this -> coeffs[sz-1] == 0) {
            this -> coeffs.pop_back();
            sz--;
        } else {
            break;
        }
    }
}

mpz_class Polynomial::eval(mpz_class x) {
    mpz_class y(0);
    for (int i=this -> deg(); i>=0; i--) {
        y = y*x + this -> coeffs[i];
    }
    return y;
}

ostream& operator<< (ostream& os, const Polynomial& p) {
    bool first = true;
    for (int i=p.deg(); i>=0; i--) {
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
    if (p.deg() == 0 and p.coeffs[0] == 0) {
        os << "0";
    }
    return os;
}

Polynomial& operator+ (const Polynomial& p, const Polynomial& q) {
    int p_deg = p.deg(), q_deg = q.deg(), r_deg = p_deg > q_deg ? p_deg : q_deg;
    vector<mpz_class> r_coeffs(r_deg + 1);
    Polynomial * r = new Polynomial(r_coeffs);
    for (int i=0; i<r_deg + 1; i++) {
        if (i > p_deg) {
            r -> coeffs[i] = q.coeffs[i];
        } else if (i > q_deg) {
            r -> coeffs[i] = p.coeffs[i];
        } else {
            r -> coeffs[i] = p.coeffs[i] + q.coeffs[i];
        }
    }
    r -> eliminate_zeros();
    return *r;
}

Polynomial& operator- (const Polynomial& p, const Polynomial& q) {
    int p_deg = p.deg(), q_deg = q.deg(), r_deg = p_deg > q_deg ? p_deg : q_deg;
    vector<mpz_class> r_coeffs(r_deg + 1);
    Polynomial * r = new Polynomial(r_coeffs);
    for (int i=0; i<r_deg + 1; i++) {
        if (i > p_deg) {
            r -> coeffs[i] = -q.coeffs[i];
        } else if (i > q_deg) {
            r -> coeffs[i] = p.coeffs[i];
        } else {
            r -> coeffs[i] = p.coeffs[i] - q.coeffs[i];
        }
    }
    r -> eliminate_zeros();
    return *r;
}

Polynomial& operator* (const Polynomial& p, const Polynomial& q) {
    int p_deg = p.deg(), q_deg = q.deg(), r_deg = p_deg + q_deg;
    vector<mpz_class> r_coeffs(r_deg + 1);
    Polynomial * r = new Polynomial(r_coeffs);
    for (int i=0; i<r_deg + 1; i++) {
        r -> coeffs[i] == 0;
    }
    for (int i=0; i<p_deg + 1; i++) {
        for (int j=0; j<q_deg + 1; j++) {
            r -> coeffs[i+j] += p.coeffs[i] * q.coeffs[j];
        }
    }
    r -> eliminate_zeros();
    return *r;
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

    cout << endl << "Testing +" << endl;
    cout << "p+p = " << p+p << endl;
    cout << "p+q = " << p+q << endl;
    cout << "p+r = " << p+r << endl;
    cout << "q+p = " << q+p << endl;
    cout << "q+q = " << q+q << endl;
    cout << "q+r = " << q+r << endl;
    cout << "r+p = " << r+p << endl;
    cout << "r+q = " << r+q << endl;
    cout << "r+r = " << r+r << endl;

    cout << endl << "Testing -" << endl;
    cout << "p-p = " << p-p << endl;
    cout << "p-q = " << p-q << endl;
    cout << "p-r = " << p-r << endl;
    cout << "q-p = " << q-p << endl;
    cout << "q-q = " << q-q << endl;
    cout << "q-r = " << q-r << endl;
    cout << "r-p = " << r-p << endl;
    cout << "r-q = " << r-q << endl;
    cout << "r-r = " << r-r << endl;

    cout << endl << "Testing *" << endl;
    cout << "p*p = " << p*p << endl;
    cout << "p*q = " << p*q << endl;
    cout << "p*r = " << p*r << endl;
    cout << "q*p = " << q*p << endl;
    cout << "q*q = " << q*q << endl;
    cout << "q*r = " << q*r << endl;
    cout << "r*p = " << r*p << endl;
    cout << "r*q = " << r*q << endl;
    cout << "r*r = " << r*r << endl;

    cout << endl << "Testing .eliminate_zeros()" << endl;
    Polynomial s = q-q;
    cout << "(q-q).coeffs.size() before: " << s.coeffs.size() << endl;
    s.eliminate_zeros();
    cout << "(q-q).coeffs.size() after: " << s.coeffs.size() << endl;
    s = r-r;
    cout << "(r-r).coeffs.size() before: " << s.coeffs.size() << endl;
    s.eliminate_zeros();
    cout << "(r-r).coeffs.size() after: " << s.coeffs.size() << endl;
    cout << "Now that eliminate_zeros is built into - operation, this does "
        << "not show anything." << endl;

    cout << endl << "Testing .eval()" << endl;
    cout << "p(0) = " << p.eval(0) << endl; // 0
    cout << "p(1) = " << p.eval(1) << endl; // 0
    cout << "p(-1) = " << p.eval(-1) << endl; // 0
    cout << "q(0) = " << q.eval(0) << endl; // 5
    cout << "q(1) = " << q.eval(1) << endl; // 15
    cout << "q(-1) = " << q.eval(-1) << endl; // 3
    cout << "r(0) = " << r.eval(0) << endl; // 1
    cout << "r(1) = " << r.eval(1) << endl; // 2
    cout << "r(-1) = " << r.eval(-1) << endl; // 2
}