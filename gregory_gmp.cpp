#include<iostream>
#include<vector>

#include<gmp.h>
#include<gmpxx.h>

#include"polynomial_gmp.h"
#include"polypair_gmp.h"
#include"rationalfn_gmp.h"

using namespace std;

#define EXP 8

int main() {
    vector<mpz_class> coeffs;
    for (int i=0; i<EXP; i++) {
        coeffs.push_back(mpz_class(0));
    }
    coeffs.push_back(mpz_class(1));
    Polynomial g = Polynomial(coeffs);
    RationalFn gregory = RationalFn(g, 1, 1, g), d_0, d_1;
    cout << gregory << endl;
    vector<RationalFn> g_n = {gregory}, after = {};
    mpq_class gregory_number, q;
    RationalFn denoms;
    PolyPair addl;
    mpz_class zero = mpz_class(0);
    bool success;
    cout << 0 << " " << gregory.lhôpitals_0_over_0(success, zero) << endl;
    for (int n=1; n<10; n++) {
        cout << n << " ";
        gregory_number = mpq_class(0, 1);
        for (RationalFn rf: g_n) {
            d_0 = rf.der_0();
            d_1 = rf.der_1();
            denoms = RationalFn(d_0.cd, d_1.cd);
            denoms.eliminate_zeros();
            addl = d_1.ab * denoms.ab;
            d_0.ab = d_0.ab * denoms.cd;
            d_0.cd = d_0.cd * denoms.cd;
            success = false;
            q = d_0.lhôpitals_0_over_0(success, zero, addl);
            cout << q << " ";
            gregory_number += q;
            if (not success) {
                cout << endl << "[ERROR] Failed lhôpitals_0_over_0()!! " 
                        << EXP << " " << n << endl << endl;
            }
            d_0.eliminate_zeros();
            after.push_back(d_0);
            after.push_back(d_1);
        }
        cout << gregory_number << endl;
        g_n.swap(after);
        after.clear();
        after.reserve(g_n.size() * 2);
    }
}