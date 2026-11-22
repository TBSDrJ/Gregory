#include<iostream>
#include<vector>
#include<string>

#include<gmp.h>
#include<gmpxx.h>

#include"polynomial_gmp.h"
#include"polypair_gmp.h"
#include"rationalfn_gmp.h"

using namespace std;

int main(int argc, char** argv) {
    string * exp = new string();
    *exp = *argv[1];
    int EXP = atoi((*exp).c_str());
    cout << "Gregory Numbers of degree " << EXP << endl;
    vector<mpz_class> coeffs;
    for (int i=0; i<EXP; i++) {
        coeffs.push_back(mpz_class(0));
    }
    coeffs.push_back(mpz_class(1));
    Polynomial g = Polynomial(coeffs);
    RationalFn gregory = RationalFn(g, 1, 1, g), d_0, d_1;
    cout << gregory << endl;
    vector<RationalFn> g_n = {gregory}, after = {};
    vector<PolyPair> numers = {gregory.ab}, denoms = {gregory.cd};
    mpq_class gregory_number, q;
    RationalFn two_denoms;
    PolyPair addl;
    mpz_class zero = mpz_class(0);
    bool success;
    cout << 0 << " " << lhôpitals_0_over_0(success, zero, numers, denoms[0]) 
            << endl;
    for (int n=1; n<100; n++) {
        cout << n << " ";
        numers.clear();
        denoms.clear();
        numers.reserve(g_n.size() * 2);
        denoms.reserve(g_n.size());
        for (RationalFn rf: g_n) {
            d_0 = rf.der_0();
            d_1 = rf.der_1();
            two_denoms = RationalFn(d_0.cd, d_1.cd);
            two_denoms.eliminate_zeros();
            numers.push_back(d_1.ab * two_denoms.ab);
            numers.push_back(d_0.ab * two_denoms.cd);
            denoms.push_back(d_0.cd * two_denoms.cd);
            success = false;
            after.push_back(d_0);
            after.push_back(d_1);
        }
        for (int i=0; i<g_n.size()-1; i++) {
            two_denoms = RationalFn(denoms[i], denoms[i+1]);
            two_denoms.eliminate_zeros();
            denoms[i+1] = two_denoms.ab * denoms[i+1];
            numers[2*i] = numers[2*i] * two_denoms.cd;
            numers[2*i+1] = numers[2*i+1] * two_denoms.cd;
            numers[2*i+2] = numers[2*i+2] * two_denoms.ab;
            numers[2*i+3] = numers[2*i+3] * two_denoms.ab;
        }
        success = false;
        gregory_number = lhôpitals_0_over_0(success, zero, numers, 
                denoms[g_n.size()-1]);
        if (success) {
            cout << gregory_number << endl;
        } else {
            cout << "FAILED CALCULATION" << endl;
        }
        g_n.swap(after);
        after.clear();
        after.reserve(g_n.size() * 2);
    }
}