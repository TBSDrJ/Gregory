from fractions import Fraction
import math, sys, os

os.environ['PYTHON_JIT'] = '1'
running_total = 0
a = Fraction(1, 1)
for n in range(2,10000):
    running_total += a
    if n % 2 == 1:
        a = Fraction(1, n)
    else:
        a = Fraction(-1, n)
sys.set_int_max_str_digits(int(math.log(running_total.denominator, 10)))
print(running_total)
print(float(running_total))