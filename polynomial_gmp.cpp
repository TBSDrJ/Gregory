#include<iostream>
#include<vector>
#include<stdexcept>
#include<gmp.h>
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

Polynomial& Polynomial::der() {
    int deg = this -> deg();
    vector<mpz_class> q_coeffs(deg > 0 ? deg : 1);
    for (int i=0; i<deg; i++) {
        q_coeffs[i] = this -> coeffs[i+1] * (i+1);
    }
    if (not deg) {
        q_coeffs[0] = 0;
    }
    Polynomial * q = new Polynomial(q_coeffs);
    return *q;
}

mpq_class& Polynomial::proportional(const Polynomial& q) {
    mpq_class* prop = new mpq_class(0);
    mpz_class m,n; // = gcd of the collection of coeffs of this, q resp.
    int p_deg = this -> deg(), q_deg = q.deg();
    if (p_deg != q_deg) {
        return *prop;
    }
    mpz_class rolling_gcd = this -> coeffs[p_deg];
    p_deg--;
    while (p_deg > 0) {
        mpz_gcd(m.get_mpz_t(), this -> coeffs[p_deg].get_mpz_t(), 
            rolling_gcd.get_mpz_t());
        p_deg--;
    }
    rolling_gcd = q.coeffs[q_deg];
    q_deg--;
    while (q_deg >= 0) {
        mpz_gcd(n.get_mpz_t(), q.coeffs[q_deg].get_mpz_t(), 
            rolling_gcd.get_mpz_t());
        q_deg--;
    }
    if (m == 0 or n == 0) {
        return *prop; // Still = 0 from initialization
    }
    if ((this -> coeffs[this -> deg()] < 0 and q.coeffs[q.deg()] > 0) or 
            (this -> coeffs[this -> deg()] > 0 and q.coeffs[q.deg()] < 0)) {
        n *= -1;
    }
    cout << endl << m << " " << n << endl;
    for (int i=0; i<this -> deg(); i++) {
        if ((this -> coeffs[i] / m) != (q.coeffs[i] / n)) {
            return *prop;
        }
    }
    *prop = mpq_class(n, m);
    return *prop;
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
            if (p.coeffs[i] == -1 and i > 0) {
                os << "-";
            } else if (p.coeffs[i] != 1 or i == 0) {
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

bool operator== (const Polynomial& p, const mpz_class& n) {
    if ((p.deg() == 0) and p.coeffs[0] == n) {
        return true;
    }
    return false;
}

bool operator== (const mpz_class& n, const Polynomial& p) {
    return p == n;
}

bool operator== (const Polynomial& p, const Polynomial& q) {
    return p.coeffs == q.coeffs;
}

bool operator!= (const Polynomial& p, const mpz_class& n) {
    return not(p == n);
}


bool operator!= (const mpz_class& n, const Polynomial& p) {
    return not(p == n);
}


bool operator!= (const Polynomial& p, const Polynomial& q) {
    return not(p == q);
}

Polynomial& operator+ (const Polynomial& p, const mpz_class& n) {
    vector<mpz_class> r_coeffs(p.deg() + 1);
    Polynomial * r = new Polynomial(r_coeffs);
    for (int i=1; i<p.deg() + 1; i++) {
        r -> coeffs[i] = p.coeffs[i];
    }
    r -> coeffs[0] = p.coeffs[0] + n;
    return *r;
}

Polynomial& operator+ (const mpz_class& n, const Polynomial& p) {
    return p + n;
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

Polynomial& operator- (const Polynomial& p, const mpz_class& n) {
    return p + (-n);
}

Polynomial& operator- (const mpz_class& n, const Polynomial& p) {
    return p + (-n);
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

Polynomial& operator* (const Polynomial& p, const mpq_class& f) {
    vector<mpq_class> rq_coeffs(p.deg() + 1);
    vector<mpz_class> r_coeffs(p.deg() + 1);
    Polynomial * r = new Polynomial(r_coeffs);
    for (int i=0; i<p.deg() + 1; i++) {
        rq_coeffs[i] = f * p.coeffs[i];
        // Coerce fraction to integer which truncates if denominator != 0
        r -> coeffs[i] = rq_coeffs[i];
        if (r -> coeffs[i] != rq_coeffs[i]) {
            string msg;
            msg += "Can only mutiply a Polynomial by a fraction if the ";
            msg += "product is\nan integer in all places.\n";
            throw runtime_error(msg);
        }
    }
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
    Polynomial s = q-q, t = r-r;
    cout << "(q-q).coeffs.size() before: " << s.coeffs.size() << endl;
    s.eliminate_zeros();
    cout << "(q-q).coeffs.size() after: " << s.coeffs.size() << endl;
    cout << "(r-r).coeffs.size() before: " << t.coeffs.size() << endl;
    t.eliminate_zeros();
    cout << "(r-r).coeffs.size() after: " << t.coeffs.size() << endl;
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

    cout << endl << "Testing .der()" << endl;
    cout << "p'(x) = " << p.der() << endl; // 0
    cout << "q'(x) = " << q.der() << endl; // 4x^3 + 6x^2 + 6x + 4
    cout << "r'(x) = " << r.der() << endl; // 2x
    cout << "(q*q)'(x) = " << (q*q).der() << endl; 
        // 8x^7 + 28x^6 + 60x^5 + 100x^4 + 140x^3 + 132x^2 + 92x + 40
    cout << "(r*r)'(x) = " << (r*r).der() << endl; // 4x^3 + 4x
    cout << "(q*r)'(x) = " << (q*r).der() << endl; 
        // 6x^5 + 10x^4 + 16x^3 + 18x^2 + 16x + 4

    cout << endl << "Testing .proportional()" << endl;
    cout << "p.proportional(q) = " << p.proportional(q) << endl;
    vector<mpz_class> s_coeffs(1), t_coeffs(1);
    s_coeffs[0] = -3;
    t_coeffs[0] = 5;
    s.coeffs = s_coeffs;
    t.coeffs = t_coeffs;
    cout << "(3q).proportional(5q) = " << (s*r).proportional(t*r) << endl;
    cout << s*r << endl << t*r << endl;

    cout << endl << "Testing ==" << endl;
    cout << "p == p: " << (p == p) << endl;
    cout << "p == q: " << (p == q) << endl;
    cout << "q == q: " << (q == q) << endl;
    cout << "p == Polynomial(): " << (p == Polynomial()) << endl;
    mpz_class n = mpz_class(5);
    cout << "t == mpz_class(5): " << (t == n) << endl;
    cout << "mpz_class(5) == t: " << (n == t) << endl;
    // Checking if bare integer gets automatically promoted to mpz_class
    cout << "t == 5: " << (t == 5) << endl;
    cout << "5 == t: " << (5 == t) << endl;
    // Yes it does.  Sweet, less boilerplate.

    cout << endl << "Testing !=" << endl;
    cout << "p != p: " << (p != p) << endl;
    cout << "p != q: " << (p != q) << endl;
    cout << "q != p: " << (q != p) << endl;
    cout << "p != 0: " << (p != 0) << endl;
    cout << "q != 0: " << (q != 0) << endl;   

    s.coeffs[0] = 1;
    s.coeffs.push_back(1);
    t.coeffs[0] = -1;
    t.coeffs.push_back(-1);
    cout << endl << "Testing + with mpz_class" << endl;
    cout << "p: " << p << " p+5: " << p+5 << endl;
    cout << "q: " << q << " q+5: " << q+5 << endl;
    cout << "r: " << r << " r+5: " << r+5 << endl;
    cout << "s: " << s << " s+5: " << s+5 << endl;
    cout << "t: " << t << " t+5: " << t+5 << endl;

    cout << endl << "Testing - with mpz_class" << endl;
    cout << "p: " << p << " p-5: " << p-5 << endl;
    cout << "q: " << q << " q-5: " << q-5 << endl;
    cout << "r: " << r << " r-5: " << r-5 << endl;
    cout << "s: " << s << " s-5: " << s-5 << endl;
    cout << "t: " << t << " t-5: " << t-5 << endl;


    cout << endl << "Testing * with integers" << endl;
    cout << "p: " << p << " p*5: " << p*5 << endl;
    cout << "q: " << q << " q*5: " << q*5 << endl;
    cout << "r: " << r << " r*5: " << r*5 << endl;
    cout << "s: " << s << " s*5: " << s*5 << endl;
    cout << "t: " << t << " t*5: " << t*5 << endl;

    cout << endl << "Testing * with mpq_class" << endl;
    mpq_class f(1,2);
    cout << "p: " << p << " p*4*½: " << p*4*f << endl;
    cout << "q: " << q << " q*4*½: " << q*4*f << endl;
    cout << "r: " << r << " r*4*½: " << r*4*f << endl;
    cout << "s: " << s << " s*4*½: " << s*4*f << endl;
    cout << "t: " << t << " t*4*½: " << t*4*f << endl;
    cout << "This one should crash:" << endl;
    cout << "q: " << q << " q*5*½: " << flush << q*5*f << endl;

}