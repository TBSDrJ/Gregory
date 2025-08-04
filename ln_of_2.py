"""With Harmonic Series out to 10k terms, the C portion took almost 3 1/4 hours 
to run.  The numerator and denominator, in lowest terms, are 14,438-bit 
integers.  So the arbitrary precision integers were pretty necessary.
"""
import math

with open('ln_of_2.txt', 'r') as f:
    s = f.read()
s = s.split(" / ")[:2]
s_m = [int(i) for i in s[0].split(", ")]
s_n = [int(i) for i in s[1].split(", ")]
m = 0
n = 0
for i in range(len(s_m)):
    m += s_m[i] * 2**(64*i)
for i in range(len(s_n)):
    n += s_n[i] * 2**(64*i)
print("Estimate using alt harmonic series: ", m/n)
print("Actual using python math library: ", math.log(2))