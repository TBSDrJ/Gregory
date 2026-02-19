from __future__ import annotations
from fractions import Fraction

from polynomial import Polynomial
from polypair import PolyPair

polynomial__eq__old = Polynomial.__eq__
polynomial__add__old = Polynomial.__add__
polynomial__sub__old = Polynomial.__sub__
polynomial__mul__old = Polynomial.__mul__

def polynomial__eq__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> bool:
    if isinstance(other, RationalFn):
        # delegate to RationalFn.__eq__()
        return other == self
    return polynomial__eq__old(self, other)

def polynomial__add__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to RationalFn.__add__()
        return other + self
    return polynomial__add__old(self, other)

def polynomial__sub__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to PolyPair.__add__()
        return (-1*other) + self
    return polynomial__sub__old(self, other)

def polynomial__mul__new(self, 
        other: RationalFn | PolyPair | Polynomial | int | Fraction
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to PolyPair.__mul__()
        return other * self
    return polynomial__mul__old(self, other)

Polynomial.__eq__ = polynomial__eq__new
Polynomial.__add__ = polynomial__add__new
Polynomial.__sub__ = polynomial__sub__new
Polynomial.__mul__ = polynomial__mul__new

polypair__eq__old = PolyPair.__eq__
polypair__add__old = PolyPair.__add__
polypair__sub__old = PolyPair.__sub__
polypair__mul__old = PolyPair.__mul__

def polypair__eq__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> bool:
    if isinstance(other, RationalFn):
        # delegate to RationalFn.__eq__()
        return other == self
    return polypair__eq__old(self, other)

def polypair__add__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to RationalFn.__add__()
        return other + self
    return polypair__add__old(self, other)

def polypair__sub__new(self, other: RationalFn | PolyPair | Polynomial | int
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to PolyPair.__add__()
        return (-1*other) + self
    return polypair__sub__old(self, other)

def polypair__mul__new(self, 
        other: RationalFn | PolyPair | Polynomial | int | Fraction
        ) -> RationalFn | PolyPair | Polynomial:
    if isinstance(other, RationalFn):
        # delegate to PolyPair.__mul__()
        return other * self
    return polypair__mul__old(self, other)

PolyPair.__eq__ = polypair__eq__new
PolyPair.__add__ = polypair__add__new
PolyPair.__sub__ = polypair__sub__new
PolyPair.__mul__ = polypair__mul__new


class RationalFn:
    def __init__(self, *args):
        polynomials = []
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
                        polynomials.append(arg.a)
                        polynomials.append(arg.b)
                else:
                    msg_1 = f"Received argument {arg} of type {type(arg)}.\n"
                    return TypeError(msg_0 + msg_1)
            else:
                msg_2 = f"Received too many arguments: {args}\n"
                raise TypeError(msg_0 + msg_2)
        while len(polynomials) < 4:
            if len(polynomials) == 0:
                polynomials.append(Polynomial(0))
            else:
                polynomials.append(Polynomial(1))
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
        if ("a" in dir(self) and isinstance(self.a, Polynomial) and
                "b" in dir(self) and isinstance(self.b, Polynomial)):
            return PolyPair(self.a, self.b)
        msg = "Cannot produce PolyPair attribute ab because "
        if "a" not in dir(self):
            msg += "a is not assigned "
        if "a" in dir(self) and not isinstance(self.a, Polynomial):
            msg += "a is not a Polynomial "
        if "b" not in dir(self):
            msg += "b is not assigned "
        if "b" in dir(self) and not isinstance(self.b, Polhynomial):
            msg += "b is not a Polynomial "
        raise ValueError(msg)

    @property
    def cd(self):
        if ("c" in dir(self) and isinstance(self.c, Polynomial) and
                "d" in dir(self) and isinstance(self.d, Polynomial)):
            return PolyPair(self.c, self.d)
        msg = "Cannot produce PolyPair attribute cd because "
        if "c" not in dir(self):
            msg += "c is not assigned "
        if "c" in dir(self) and not isinstance(self.c, Polynomial):
            msg += "c is not a Polynomial "
        if "d" not in dir(self):
            msg += "d is not assigned "
        if "d" in dir(self) and not isinstance(self.d, Polhynomial):
            msg += "d is not a Polynomial "
        raise ValueError(msg)

    @a.setter
    def a(self, new_a: Polynomial | int):
        new_a = self.validate_polynomial(new_a)
        self._a = new_a

    @b.setter
    def b(self, new_b: Polynomial | int):
        new_b = self.validate_polynomial(new_b)
        self._b = new_b

    @c.setter
    def c(self, new_c: Polynomial | int):
        new_c = self.validate_polynomial(new_c)
        self._c = new_c

    @d.setter
    def d(self, new_d: Polynomial | int):
        new_d = self.validate_polynomial(new_d)
        self._d = new_d

    @ab.setter
    def ab(self, new_ab: PolyPair | Polynomial | int):
        # Let PolyPair constructor do error handling
        if not isinstance(new_ab, PolyPair):
            new_ab = PolyPair(new_ab)
        self._a = new_ab.a
        self._b = new_ab.b
    
    @cd.setter
    def cd(self, new_cd: PolyPair | Polynomial | int):
        # Let PolyPair constructor do error handling
        if not isinstance(new_cd, PolyPair):
            new_cd = PolyPair(new_cd)
        self._c = new_cd.a
        self._d = new_cd.b

    def validate_polynomial(self, p: Polynomial | int) -> Polynomial:
        """Make sure that a and b attributes are Polynomials."""
        if isinstance(p, int):
            p = Polynomial(p)
        elif not isinstance(p, Polynomial):
            msg = "self.a, b, c, d in RationalFn must be either int "
            msg += f"or Polynomial.  Found {p}, {type(p)} instead."
            raise TypeError(msg)
        p.eliminate_zeros()
        return p

    def validate_polypair(self, p: Polynomial | int) -> Polynomial:
        """Make sure that a and b attributes are Polynomials."""
        if isinstance(p, int) or isinstance(p, Polynomial):
            p = PolyPair(p)
        elif not isinstance(p, PolyPair):
            msg = "self.ab, self.cd in RationalFn must be int, Polynomial "
            msg += f"or PolyPair.  Found {p}, {type(p)} instead."
            raise TypeError(msg)
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
        factor_b = self.b.proportional(other.b)
        factor_c = self.c.proportional(other.c)
        factor_d = self.d.proportional(other.d)
        if factor_a and factor_b and factor_c and factor_d:
            if (factor_a * factor_b)/(factor_c * factor_d) == 1:
                return True 
        return False

    def __add__(self, other: RationalFn | int | Polynomial | PolyPair
            ) -> RationalFn | None:
        if (isinstance(other, int) or isinstance(other, Polynomial) or 
                isinstance(other, PolyPair)):
            other = RationalFn(other)
        if self == 0:
            return other
        if other == 0:
            return self
        result = RationalFn()
        result_found = False
        # First, all the easy cases where there are exact common factors
        if self.a == other.a and self.c == other.c and self.d == other.d:
            result.a = self.a
            result.b = self.b + other.b
            result.c = self.c
            result.d = self.d
            result.b.eliminate_zeros()
            result_found = True
        elif self.b == other.b and self.c == other.c and self.d == other.d:
            result.a = self.a + other.a
            result.b = self.b
            result.c = self.c
            result.d = self.d
            result.a.eliminate_zeros()
            result_found = True
        # Separate later cases to avoid the calculation of factors 
        #   if not needed.
        if not result_found:
            factor_a = self.a.proportional(other.a)
            factor_b = self.b.proportional(other.b)
            if self.c == other.c and self.d == other.d and factor_a:
                result = RationalFn(self.ab + other.ab, self.cd)
                result_found = True
            elif self.c == other.c and self.d == other.d and factor_b:
                result = RationalFn(self.ab + other.ab, self.cd)
                result_found = True
        # if not result_found:

        if result.a == Polynomial() or result.b == Polynomial():
            result.a = Polynomial()
            result.b = Polynomial()
            result.c = Polynomial(1)
            result.d = Polynomial(1)
        if not result_found:
            return None
        return result

    def common_denominator(self, other: RationalFn
            ) -> (RationalFn, RationalFn):
        """If a common denominator can be found, return both with it.
        
        A common denominator will be found if, for each of c & d, one of the
            two RationalFns has deg = 0 there or the pair is proportional.
        Returns None if no common denominator of this kind can be found.
        Does not modify the inputs in place, creates new RationalFn instances
            with the common denominator if one can be found."""
        # Get simplest case out of the way
        if self.c == other.c and self.d == other.d:
            return self, other
        if self.c.deg == 0 or other.c.deg == 0:
            if self.d == other.d:
                new_self = RationalFn(other.c * self.a, self.b, 
                        self.c * other.c, self.d)
                new_other = RationalFn(self.c * other.a, self.b, 
                        self.c * other.c, other.d)
                return new_self, new_other
        if self.d.deg == 0 or other.d.deg == 0:
            if self.c == other.c:
                new_self = RationalFn(self.a, other.d * self.b,
                        self.c, other.d * self.d)
                new_other = RationalFn(other.a, self.d * other.b,
                        other.c, self.d * other.d)
                return new_self, new_other
        if ((factor_c := self.c.proportional(other.c))
            and (factor_d := self.d.proportional(other.d))):
                new_self = RationalFn(
                        factor_c.numerator*self.a, factor_d.numerator*self.b, 
                        factor_c.numerator*self.c, factor_d.numerator*self.d)
                new_other = RationalFn(
                        factor_c.denominator*other.a, 
                        factor_d.denominator*other.b, 
                        factor_c.denominator*other.c, 
                        factor_d.denominator*other.d)
                return new_self, new_other
        return None