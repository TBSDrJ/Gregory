"""With Harmonic Series out to 10k terms, the C portion took almost 3 1/4 hours 
to run.  UPDATE: I changed the fraction addition and subtraction algorithms to
remember a and b instead of finding the common denominator at the beginning 
and then reducing at the end, and cutting out the fraction reducing helped 
quite a bit: down to 2 hours, 36 minutes, which is about a 20% speed increase.  

Then it occurred to me to use the -O2 flag (not sure why I didn't do this 
earlier) and that sped it up to under 20 minutes!  (18:45.3). This is a more 
than 8x speed-up, which is spectacular.  Then, I plugged my computer in, which
switches it to 'High Power' mode, and this reduced it 17:47.5.

The numerator and denominator, in lowest terms, are 14,438-bit 
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
print("Estimate using alternating harmonic series: ", m/n)
print("Actual using python math library: ", math.log(2))