#include<iostream>
#include<sstream>
#include<algorithm>
#include<gmpxx.h>
#include"polypair_gmp.h"
#include"polynomial_gmp.h"

using namespace std;

PolyPair::PolyPair() {
    Polynomial a = *(new Polynomial());
    Polynomial b = *(new Polynomial());
    this -> a = a;
    this -> b = b;
}

PolyPair::PolyPair(mpz_class m, mpz_class n) {
    if ((m == 0) or (n == 0)) {
        this -> a = *(new Polynomial());
        this -> b = *(new Polynomial());
    } else {
        this -> a = *(new Polynomial(m));
        this -> b = *(new Polynomial(n));
    }
}

PolyPair::PolyPair(mpz_class n, Polynomial& new_b) {
    Polynomial * q = new Polynomial(n);
    this -> a = *q;
    this -> b = new_b;
    this -> b.eliminate_zeros();
    if (this -> b == Polynomial()) {
        this -> a = *(new Polynomial());
    }
}

PolyPair::PolyPair(Polynomial& new_a, mpz_class n) {
    Polynomial * q = new Polynomial(n);
    this -> b = *q;
    this -> a = new_a;
    this -> a.eliminate_zeros();
    if (this -> a == Polynomial()) {
        this -> b = *(new Polynomial());
    }
}

PolyPair::PolyPair(Polynomial& new_a, Polynomial& new_b) {
    this -> a = new_a;
    this -> b = new_b;
    Polynomial zero = Polynomial();
    if (this -> a == zero) {
        this -> b = *(new Polynomial());
    } else if (this -> b == zero) {
        this -> a = *(new Polynomial());
    } else {
        this -> a.eliminate_zeros();
        this -> b.eliminate_zeros();
    }
}

bool PolyPair::common_factor(const PolyPair& qq) const {
    if ((this -> a.deg() == 0) and (qq.a.deg() == 0)) {
        return true;
    }
    if ((this -> b.deg() == 0) and (qq.b.deg() == 0)) {
        return true;
    }
    if (this -> a.proportional(qq.a)) {
        return true;
    }
    if (this -> b.proportional(qq.b)) {
        return true;
    }
    return false;
}

bool PolyPair::common_factor(const Polynomial& q) const {
    if (this -> b.deg() == 0) {
        return true;
    }
    if (q.deg() == 0) {
        return this -> common_factor(q.coeffs[0]);
    }
    return false;
}

bool PolyPair::common_factor(const mpz_class& n) const {
    if ((this -> a.deg() == 0) or (this -> b.deg() == 0)) {
        return true;
    }
    return false;
}


bool operator== (const PolyPair& pp, const mpz_class& n) {
    if (pp.a.deg() > 0 or pp.b.deg() > 0) {
        return false;
    }
    if (pp.a * pp.b == n) {
        return true;
    }
    return false;
}

bool operator== (const mpz_class& n, const PolyPair& pp) {
    return pp == n;
}

bool operator== (const PolyPair& pp, const Polynomial& p) {
    if (pp.b.deg() == 0 and pp.a * pp.b == p) {
        return true;
    }
    return false;
}

bool operator== (const Polynomial& p, const PolyPair& pp) {
    return pp == p;
}

bool operator== (const PolyPair& pp, const PolyPair& qq) {
    if ((pp.a.deg() != qq.a.deg()) or (pp.b.deg() != qq.b.deg())) {
        return false;
    }
    if ((pp.a == qq.a) and (pp.b == qq.b)) {
        return true;
    }
    // Check all ways that one or more polynomials are zero.
    const Polynomial zero = Polynomial();
    if ((pp.a == zero) and ((qq.a != zero) and (qq.b != zero))) {
        return false;
    }
    if ((pp.b == zero) and ((qq.a != zero) and (qq.b != zero))) {
        return false;
    }
    if ((qq.a == zero) and ((pp.a != zero) and (pp.b != zero))) {
        return false;
    }
    if ((qq.b == zero) and ((pp.a != zero) and (pp.b != zero))) {
        return false;
    }
    if (((pp.a == zero) or (pp.b == zero)) and 
            ((qq.a == zero) or (qq.b == zero))) {
        return true;
    }
    // Now we know all four polynomials are nonzero.
    // Gotta check for proportional cases, e.g. (2x)(3L) == (6x)(L)
    // I'm surprised but, e.g. mpq_class(3,3) == 1 comes up false.
    // Also (factor_a * factor_b).get_num() fails.  Hence the construction
    //  of the third condition below.
    mpq_class factor_a = pp.a.proportional(qq.a);
    mpq_class factor_b = pp.b.proportional(qq.b);
    mpq_class prod = factor_a * factor_b;
    if ((pp.a * factor_a == qq.a) and (pp.b * factor_b == qq.b) and 
            (prod.get_num() == prod.get_den())) {
        return true;
    }
    return false;
}


bool operator!= (const PolyPair& pp, const mpz_class& n) {
    return not(pp == n);
}

bool operator!= (const mpz_class& n, const PolyPair& pp) {
    return not(n == pp);
}

bool operator!= (const PolyPair& pp, const Polynomial& p) {
    return not(pp == p);
}

bool operator!= (const Polynomial& p, const PolyPair& pp) {
    return not(p == pp);
}

bool operator!= (const PolyPair& pp, const PolyPair& qq) {
    return not(pp == qq);
}


ostream& operator<< (ostream& os, const PolyPair& pp) {
    string s;
    stringstream t;
    t << pp.b;
    s = t.str();
    replace(s.begin(), s.end(), 'x', 'L');
    os << "(" << pp.a << ")(" << s << ")";
    return os;
}

int main() {
    cout << "Testing constructor and << overload" << endl;
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
    PolyPair pp_04 = PolyPair(p_0, p_4);
    cout << "pp_04 = " << pp_04 << endl;
    PolyPair pp_40 = PolyPair(p_4, p_0);
    cout << "pp_40 = " << pp_40 << endl;
    PolyPair pp_11 = PolyPair(p_1, p_1);
    cout << "pp_11 = " << pp_11 << endl;
    PolyPair pp_12 = PolyPair(p_1, p_2);
    cout << "pp_12 = " << pp_12 << endl;
    PolyPair pp_13 = PolyPair(p_1, p_3);
    cout << "pp_13 = " << pp_13 << endl;
    PolyPair pp_14 = PolyPair(p_1, p_4);
    cout << "pp_14 = " << pp_14 << endl;
    PolyPair pp_15 = PolyPair(p_1, p_5);
    cout << "pp_15 = " << pp_15 << endl;
    PolyPair pp_21 = PolyPair(p_2, p_1);
    cout << "pp_21 = " << pp_21 << endl;
    PolyPair pp_22 = PolyPair(p_2, p_2);
    cout << "pp_22 = " << pp_22 << endl;
    PolyPair pp_23 = PolyPair(p_2, p_3);
    cout << "pp_23 = " << pp_23 << endl;
    PolyPair pp_24 = PolyPair(p_2, p_4);
    cout << "pp_24 = " << pp_24 << endl;
    PolyPair pp_25 = PolyPair(p_2, p_5);
    cout << "pp_25 = " << pp_25 << endl;
    PolyPair pp_31 = PolyPair(p_3, p_1);
    cout << "pp_31 = " << pp_31 << endl;
    PolyPair pp_32 = PolyPair(p_3, p_2);
    cout << "pp_32 = " << pp_32 << endl;
    PolyPair pp_33 = PolyPair(p_3, p_3);
    cout << "pp_33 = " << pp_33 << endl;
    PolyPair pp_35 = PolyPair(p_3, p_5);
    cout << "pp_35 = " << pp_35 << endl;
    PolyPair pp_41 = PolyPair(p_4, p_1);
    cout << "pp_41 = " << pp_41 << endl;
    PolyPair pp_42 = PolyPair(p_4, p_2);
    cout << "pp_42 = " << pp_42 << endl;
    PolyPair pp_43 = PolyPair(p_4, p_3);
    cout << "pp_43 = " << pp_43 << endl;
    PolyPair pp_44 = PolyPair(p_4, p_4);
    cout << "pp_44 = " << pp_44 << endl;
    PolyPair pp_45 = PolyPair(p_4, p_5);
    cout << "pp_45 = " << pp_45 << endl;
    PolyPair pp_51 = PolyPair(p_5, p_1);
    cout << "pp_51 = " << pp_51 << endl;
    PolyPair pp_52 = PolyPair(p_5, p_2);
    cout << "pp_52 = " << pp_52 << endl;
    PolyPair pp_53 = PolyPair(p_5, p_3);
    cout << "pp_53 = " << pp_53 << endl;
    PolyPair pp_54 = PolyPair(p_5, p_4);
    cout << "pp_54 = " << pp_54 << endl;
    PolyPair pp_55 = PolyPair(p_5, p_5);
    cout << "pp_55 = " << pp_55 << endl;

    cout << endl << "Testing ==" << endl;
    cout << "Should all be true: " << endl;
    cout << "pp_00 == pp_00: " << (pp_00 == pp_00) << endl;
    cout << "pp_00 == pp_04: " << (pp_00 == pp_04) << endl;
    cout << "pp_40 == pp_00: " << (pp_40 == pp_00) << endl;
    cout << "pp_11 == p_1: " << (pp_11 == p_1) << endl;
    cout << "p_1 == pp_11: " << (p_1 == pp_11) << endl;
    cout << "pp_11 == 1: " << (pp_11 == 1) << endl;
    cout << "1 == pp_11: " << (1 == pp_11) << endl;
    cout << "pp_51 == p_5: " << (pp_51 == p_5) << endl;
    cout << "p_5 == pp_51: " << (p_5 == pp_51) << endl;
    cout << "pp_23 == pp_32: " << (pp_23 == pp_32) << endl;
    cout << "(3p_2)(p_2) == (p_3)(3p_3): " <<
            (PolyPair(3*p_2, p_2) == PolyPair(p_3, 3*p_3)) << endl;
    cout << "(-3p_2(x))(-2p_5(L)) == (6p_2(x))(p_5(L)): " << 
            (PolyPair(-3*p_2, -2*p_5) == PolyPair(6*p_2, p_5)) << endl;
    cout << "Should all be False: " << endl;
    cout << "pp_12 == pp_23: " << (pp_12 == pp_23) << endl;
    cout << "pp_12 == p_2: " << (pp_12 == p_2) << endl;
    cout << "p_2 == pp_12: " << (p_2 == pp_12) << endl;
    cout << "1 == pp_12: " << (1 == pp_12) << endl;
    cout << "pp_12 == 1: " << (pp_12 == 1) << endl;
    cout << "pp_22 == pp_23: " << (pp_22 == pp_23) << endl;
    cout << "pp_12 == pp_21: " << (pp_12 == pp_21) << endl;


    cout << endl << "Testing !=" << endl;
    cout << "Should all be false: " << endl;
    cout << "pp_00 != pp_00: " << (pp_00 != pp_00) << endl;
    cout << "pp_00 != pp_04: " << (pp_00 != pp_04) << endl;
    cout << "pp_40 != pp_00: " << (pp_40 != pp_00) << endl;
    cout << "pp_11 != p_1: " << (pp_11 != p_1) << endl;
    cout << "p_1 != pp_11: " << (p_1 != pp_11) << endl;
    cout << "pp_11 != 1: " << (pp_11 != 1) << endl;
    cout << "1 != pp_11: " << (1 != pp_11) << endl;
    cout << "pp_51 != p_5: " << (pp_51 != p_5) << endl;
    cout << "p_5 != pp_51: " << (p_5 != pp_51) << endl;
    cout << "pp_23 != pp_32: " << (pp_23 != pp_32) << endl;
    cout << "(3p_2)(p_2) != (p_3)(3p_3): " <<
            (PolyPair(3*p_2, p_2) != PolyPair(p_3, 3*p_3)) << endl;
    cout << "(-3p_2(x))(-2p_5(L)) != (6p_2(x))(p_5(L)): " << 
            (PolyPair(-3*p_2, -2*p_5) != PolyPair(6*p_2, p_5)) << endl;
    cout << "Should all be True: " << endl;
    cout << "pp_12 != pp_23: " << (pp_12 != pp_23) << endl;
    cout << "pp_12 != p_2: " << (pp_12 != p_2) << endl;
    cout << "p_2 != pp_12: " << (p_2 != pp_12) << endl;
    cout << "1 != pp_12: " << (1 != pp_12) << endl;
    cout << "pp_12 != 1: " << (pp_12 != 1) << endl;
    cout << "pp_22 != pp_23: " << (pp_22 != pp_23) << endl;
    cout << "pp_12 != pp_21: " << (pp_12 != pp_21) << endl;

    cout << endl << "Testing .common_factor():" << endl;
    cout << "Should all be true:" << endl;
    cout << "pp_00.common_factor(pp_00): " << pp_00.common_factor(pp_00) 
            << endl;
    cout << "pp_00.common_factor(p_1): " << pp_00.common_factor(p_1) << endl;
    cout << "pp_00.common_factor(5): " << pp_00.common_factor(5) << endl;
    cout << "pp_11.common_factor(pp_11): " << pp_11.common_factor(pp_11) 
            << endl;
    cout << "pp_11.common_factor(37*p_1): " << pp_11.common_factor(37*p_1) 
            << endl;
    cout << "pp_11.common_factor(5): " << pp_11.common_factor(5) << endl;
    cout << "pp_12.common_factor(pp_13): " << pp_12.common_factor(pp_13) 
            << endl;
    cout << "pp_12.common_factor(37*p_1): " << pp_12.common_factor(37*p_1) 
            << endl;
    cout << "pp_12.common_factor(5): " << pp_12.common_factor(5) << endl;
    cout << "pp_21.common_factor(pp_31): " << pp_21.common_factor(pp_31) 
            << endl;
    cout << "pp_21.common_factor(37*p_1): " << pp_21.common_factor(37*p_1) 
            << endl;
    cout << "pp_21.common_factor(5): " << pp_21.common_factor(5) << endl;
    cout << "pp_42.common_factor(pp_52): " << pp_42.common_factor(pp_52) 
            << endl;
    cout << "pp_42.common_factor(pp_53): " << pp_42.common_factor(pp_53) 
            << endl;
    cout << "pp_22.common_factor(pp_22): " << pp_22.common_factor(pp_22) 
            << endl;
    cout << "pp_22.common_factor(pp_23): " << pp_22.common_factor(pp_23) 
            << endl;
    cout << "pp_22.common_factor(pp_32): " << pp_22.common_factor(pp_33) 
            << endl;
    cout << "pp_22.common_factor(pp_33): " << pp_22.common_factor(pp_33) 
            << endl;
    cout << "pp_44.common_factor(pp_45): " << pp_44.common_factor(pp_45) 
            << endl;
    cout << "pp_55.common_factor(pp_45): " << pp_55.common_factor(pp_45) 
            << endl;
    cout << "Should all be false:" << endl;
    cout << "pp_25.common_factor(pp_53): " << pp_25.common_factor(pp_53)
            << endl;
    cout << "pp_24.common_factor(pp_42): " << pp_24.common_factor(pp_42)
            << endl;
    cout << "pp_35.common_factor(pp_43): " << pp_35.common_factor(pp_43)
            << endl;
    cout << "pp_45.common_factor(pp_54): " << pp_45.common_factor(pp_54)
            << endl;
    cout << "pp_54.common_factor(pp_45): " << pp_54.common_factor(pp_45)
            << endl;
    cout << "pp_55.common_factor(pp_44): " << pp_55.common_factor(pp_44)
            << endl;
}