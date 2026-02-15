from fractions import Fraction

from polynomial import Polynomial
from polypair import PolyPair

class RationalFn:
    def __init__(self, *args):
        polynomials = []
        polypairs = []
        msg_0 = "RationalFn() takes up to 4 inputs.  These can be "
        msg_0 += "int, Polynomial or PolyPair (counts as 2).\n"
        for arg in args:
            if len(polynomials) < 4:
                if isinstance(arg, int):
                    polynomials.append(Polynomial(arg))
                elif isinstance(arg, Polynomial):
                    polynomials.append(arg)
                elif isinstance(arg, PolyPair):
                    if len(polynomials) < 3:
                        if len(polynomials) == 1:
                            polynomials.append(Polynomial(1))
                            polypairs.append(PolyPair(polynomials[0], 
                                    polynomials[1]))
                        polynomials.append(arg.a)
                        polynomials.append(arg.b)
                        polypairs.append(arg)
                else:
                    msg_1 = f"Received argument {arg} of type {type(arg)}.\n"
                    raise ValueError(msg_0 + msg_1)
                if len(polynomials) == 2 and len(polypairs) == 0: 
                    polypairs.append(PolyPair(polynomials[0], 
                            polynomials[1]))
                elif len(polynomials) == 4 and len(polypairs) == 1:
                    polypairs.append(PolyPair(polynomials[2], 
                            polynomials[3]))
            else:
                msg_2 = f"Received too many arguments: {args}\n"
                raise ValueError(msg_0 + msg_2)
        while len(polynomials) < 4:
            if len(polynomials) == 0:
                polynomials.append(Polynomial(0))
            else:
                polynomials.append(Polynomial(1))
            if len(polynomials) == 2 and len(polypairs) == 0: 
                polypairs.append(PolyPair(polynomials[0], 
                        polynomials[1]))
            elif len(polynomials) == 4 and len(polypairs) == 1:
                polypairs.append(PolyPair(polynomials[2], 
                        polynomials[3]))
        self.ab = polypairs[0]
        self.cd = polypairs[1]
        self.a = polynomials[0]
        self.b = polynomials[1]
        self.c = polynomials[2]
        self.d = polynomials[3]
        if self.a == Polynomial() or self.b == Polynomial():
            self.a = Polynomial()
            self.b = Polynomial()
            self.c = Polynomial(1)
            self.d = Polynomial(1)
        if self.cd == PolyPair():
            msg = "PolyPair in the denominator == 0."
            raise ZeroDivisionError(msg)

    @property
    def a(self):
        return self._a
    
    @property
    def b(self):
        return self._b

    @property
    def c(self):
        return self._c

    @property
    def d(self):
        return self._d

    @property
    def ab(self):
        return self._ab

    @property
    def cd(self):
        return self._cd

    @a.setter
    def a(self, new_a):
        new_a = self.validate_polynomial(new_a)
        self._a = new_a

    @b.setter
    def b(self, new_b):
        new_b = self.validate_polynomial(new_b)
        self._b = new_b

    @c.setter
    def c(self, new_c):
        new_c = self.validate_polynomial(new_c)
        self._c = new_c

    @d.setter
    def d(self, new_d):
        new_d = self.validate_polynomial(new_d)
        self._d = new_d

    @ab.setter
    def ab(self, new_ab):
        new_ab = self.validate_polypair(new_ab)
        self._ab = new_ab

    @cd.setter
    def cd(self, new_cd):
        new_cd = self.validate_polypair(new_cd)
        self._cd = new_cd

    def validate_polynomial(self, p: Polynomial | int) -> Polynomial:
        """Make sure that a and b attributes are Polynomials."""
        if isinstance(p, int):
            p = Polynomial(p)
        elif not isinstance(p, Polynomial):
            msg = "self.a, b, c, d in RationalFn must be either int "
            msg += f"or Polynomial.  Found {p}, {type(p)} instead."
            raise ValueError(msg)
        return p


    def validate_polypair(self, p: Polynomial | int) -> Polynomial:
        """Make sure that a and b attributes are Polynomials."""
        if isinstance(p, int) or isinstance(p, Polynomial):
            p = PolyPair(p)
        elif not isinstance(p, PolyPair):
            msg = "self.ab, self.cd in RationalFn must be int, Polynomial "
            msg += f"or PolyPair.  Found {p}, {type(p)} instead."
            raise ValueError(msg)
        return p

    def __str__(self) -> str:
        b = str(self.b).replace("x", "L")
        d = str(self.d).replace("x", "L")
        s = f"RationalFn: ({self.a})({b}) / ({str(self.c)})({d})"
        return s

    def __repr__(self) -> str:
        s = f"RationalFn(Polynomial({self.a.coeffs}), "
        s += f"Polynomial({self.b.coeffs}), Polynomial({self.c.coeffs}), "
        s += f"Polynomial({self.d.coeffs}))"
        return s

    def __eq__(self, other) -> bool:
        if (isinstance(other, int) or isinstance(other, Polynomial) or 
                isinstance(other, PolyPair)):
            other = RationalFn(other)
        if self.ab == 0 and other.ab == 0:
            return True
        if self.ab == other.ab and self.cd == other.cd:
            return True
        factor_a = self.a.proportional(other.a)
        factor_b = self.a.proportional(other.b)
        factor_c = self.a.proportional(other.c)
        factor_d = self.a.proportional(other.d)
        if factor_a and factor_b and factor_c and factor_d:
            if (factor_a * factor_b)/(factor_c * factor_d) == 1:
                return True 
        return False

    def __add__(self, other: "RationalFn | int | Polynomial | PolyPair"
            ) -> "RationalFn | list[RationalFn]":
        if (isinstance(other, int) or isinstance(other, Polynomial) or 
                isinstance(other, PolyPair)):
            other = RationalFn(other)
        if self == 0:
            return other
        if other == 0:
            return self
        result = RationalFn()
        factor_a = self.a.proportional(other.a)
        factor_b = self.b.proportional(other.b)
        if self.a == other.a and self.c == other.c and self.d == other.d:
            result.a = self.a
            result.b = self.b + other.b
            result.c = self.c
            result.d = self.d
            result.b.eliminate_zeros()
        elif self.b == other.b and self.c == other.c and self.d == other.d:
            result.a = self.a + other.a
            result.b = self.b
            result.c = self.c
            result.d = self.d
            result.a.eliminate_zeros()
        elif self.c == other.c and self.d == other.d and factor_a:
            result = RationalFn(self.ab + other.ab, self.cd)
        elif self.c == other.c and self.d == other.d and factor_b:
            result = RationalFn(self.ab + other.ab, self.cd)
        if result.a == Polynomial() or result.b == Polynomial():
            result.a = Polynomial()
            result.b = Polynomial()
            result.c = Polynomial(1)
            result.d = Polynomial(1)
        result.ab = PolyPair(result.a, result.b)
        result.cd = PolyPair(result.c, result.d)
        return result