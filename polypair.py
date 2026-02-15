import math
from typing import Callable
from fractions import Fraction

from polynomial import Polynomial

polynomial__eq__old = Polynomial.__eq__
polynomial__add__old = Polynomial.__add__
polynomial__sub__old = Polynomial.__sub__
polynomial__mul__old = Polynomial.__mul__

def polynomial__eq__new(self, other: "PolyPair | Polynomial | int") -> bool:
    if isinstance(other, PolyPair):
        return other == self
    return polynomial__eq__old(self, other)

def polynomial__add__new(self, other: "PolyPair | Polynomial | int"
        ) -> "Polynomial | PolyPair":
    if isinstance(other, PolyPair):
        # delegate to PolyPair.__add__()
        return other + self
    return polynomial__add__old(self, other)

def polynomial__sub__new(self, other: "PolyPair | Polynomial | int"
        ) -> "Polynomial | PolyPair":
    if isinstance(other, PolyPair):
        # delegate to PolyPair.__add__()
        return (-1*other) + self
    return polynomial__sub__old(self, other)

def polynomial__mul__new(self, 
        other: "PolyPair | Polynomial | int | Fraction"
        ) -> "Polynomial | PolyPair":
    if isinstance(other, PolyPair):
        # delegate to PolyPair.__mul__()
        return other * self
    return polynomial__mul__old(self, other)

Polynomial.__eq__ = polynomial__eq__new
Polynomial.__add__ = polynomial__add__new
Polynomial.__sub__ = polynomial__sub__new
Polynomial.__mul__ = polynomial__mul__new

class PolyPair:
    """Represents a pair a(x)b(L), where L = ln(1+x)
    
    This does *not* represent a(x)b(x), if you want that just use a*b
        or (a, b).
    We don't take derivative here because the result is not a PolyPair. We'll
        tackle that in the RationalFn library because the derivative of a 
        RationalFn is another RationalFn.
    We use Polynomial() as the first default and Polynomial(1) as the
        second so we can easily handle a single input, which is assigned to a;
        then ab = a if b is empty.  If both are empty, we want the result to 
        be zero, which is handled by the default for a being Polynomial().
    """
    def __init__(self, a: Polynomial | int = Polynomial(), 
            b: Polynomial | int = Polynomial(1)):
        """Constructor for Polypair.
        
        Will only accept Polynomial or int for each argument.
        After construction, self.a and self.b are both of type Polynomial."""
        self.a = a
        self.b = b
        # Multiplying by zero makes everything else zero.
        if self.a == Polynomial() or self.b == Polynomial():
            self.a = Polynomial()
            self.b = Polynomial()

    def validate_input(self, p: Polynomial | int) -> Polynomial:
        """Make sure that a and b attributes are Polynomials."""
        if isinstance(p, int):
            p = Polynomial(p)
        elif not isinstance(p, Polynomial):
            msg = "Arguments in the PolyPair constructor must be either int "
            msg += f"or Polynomial.  Received {p} instead."
            raise ValueError(msg)
        return p

    @property
    def a(self):
        return self._a

    @property
    def b(self):
        return self._b

    @a.setter
    def a(self, new_a):
        new_a = self.validate_input(new_a)
        self._a = new_a

    @b.setter
    def b(self, new_b):
        new_b = self.validate_input(new_b)
        self._b = new_b

    def __str__(self) -> str:
        return f"PolyPair: ({self.a})({str(self.b).replace('x', 'L')})"

    def __repr__(self) -> str:
        s = f"PolyPair(Polynomial({self.a.coeffs}), "
        s += f"Polynomial({self.b.coeffs}))"
        return s

    def __eq__(self, other):
        if isinstance(other, int):
            if (self.a.deg == 0 and self.b.deg == 0 and 
                    other == self.a.coeffs[0]*self.b.coeffs[0]):
                return True
            else:
                return False
        elif isinstance(other, Polynomial):
            if self.b.deg > 0:
                return False
            return True if self.a * self.b == other else False
        elif not isinstance(other, PolyPair):
            return False
        self.a.eliminate_zeros()
        self.b.eliminate_zeros()
        other.a.eliminate_zeros()
        other.b.eliminate_zeros()
        if self.a == other.a and self.b == other.b:
            return True
        # If either factor of a pair is zero, make both zero.
        if self.a == Polynomial() or self.b == Polynomial():
            self.a = Polynomial()
            self.b = Polynomial()
        if other.a == Polynomial or other.b == Polynomial():
            other.a = Polynomial()
            other.b = Polynomial()
        # If both self and other are zero
        if (self.a == other.a and self.b == other.b and self.a ==
                Polynomial() and self.b == Polynomial()):
            return True
        # If one is zero and the other is not.
        if (self.a == Polynomial() and other.a != Polynomial()):
            return False
        if (self.a != Polynomial() and other.a == Polynomial()):
            return False
        # (-1)*(-1) cancels
        elif self.a == (-1)*other.a and self.b ==(-1)*other.b:
            return True
        # If self == other then factor_a, factor_b are reciprocals.
        if ((factor_a := self.a.proportional(other.a)) and 
                (factor_b := self.b.proportional(other.b))):
            if (factor_a * factor_b == 1):
                return True
        else:
            return False

    def __bool__(self) -> bool:
        if self == 0:
            return False
        else:
            return True

    def _add_sub(self, operation: Callable, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair":
        """Combine work from __add__, __sub__ to avoid repitition."""
        if operation not in [int.__add__, int.__sub__]:
            msg = "Method PolyPair._add_sub expected either "
            msg += "int.__add__ or int.__sub__."
            raise ValueError(msg)
        result = None
        if isinstance(other, int) or isinstance(other, Polynomial):
            other = PolyPair(other)
        if isinstance(other, PolyPair):
            if self.a == 0 or self.b == 0:
                if operation == int.__add__: 
                    result = other
                else:
                    result = (-1)*other
            elif other.a == 0 or other.b == 0:
                result = self
            elif self.a == other.a:
                result = PolyPair(self.a, self.b._add_sub(operation, other.b))
            elif self.b == other.b:
                result = PolyPair(self.a._add_sub(operation, other.a), self.b)
            elif (factor := self.a.proportional(other.a)):
                # We know that factor.denominator divides all coeffs of self.a
                result = PolyPair(Fraction(1, factor.denominator)*self.a, 
                        (factor.denominator*self.b)._add_sub(operation, 
                        factor.numerator*other.b))
            elif (factor := self.b.proportional(other.b)):
                # We know that factor.denominator divides all coeffs of self.b
                result = PolyPair((factor.denominator*self.a)._add_sub(
                        operation, factor.numerator*other.a), Fraction(1, 
                        factor.denominator)*self.b)
            if result is not None:
                if result.a == 0 or result.b == 0:
                    result.a = Polynomial()
                    result.b = Polynomial()
            if result is None:
                if operation == int.__add__:
                    return [self, other]           
                else:
                    return [self, (-1)*other]
            return result
        else:
            msg = "Addition/Subtraction for PolyPair only defined for "
            msg += "PolyPair, int, or Polynomial."
            raise ValueError(msg)

    def __add__(self, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair | list[PolyPair]":
        """Add if a's or b's are proportional by a constant, else return list.
        
        Returns [self, other] if neither a nor b is a common factor.
        Returns a PolyPair otherwise."""
        if isinstance(other, Polynomial) or isinstance(other, int):
            other = PolyPair(other)
        return self._add_sub(int.__add__, other)

    def __radd__(self, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair | list[PolyPair]":
        return self + other

    def __sub__(self, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair | list[PolyPair]":
        """Add if a's and b's are proportional by a constant, else return list.
        
        Returns [self, other] if neither a nor b is a common factor.
        Returns a PolyPair otherwise."""
        if isinstance(other, Polynomial) or isinstance(other, int):
            other = PolyPair(other)
        return self._add_sub(int.__sub__, other)

    def __rsub__(self, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair | list[PolyPair]":
        return (-1)*self + other

    def __mul__(self, other:  "PolyPair | Polynomial | int | Fraction"
            ) -> "PolyPair":
        if isinstance(other, int) or isinstance(other, Fraction):
            factor_a = math.gcd(*self.a.coeffs)
            factor_b = math.gcd(*self.b.coeffs)
            if factor_a * factor_b % other.denominator == 0:
                gcd_a = math.gcd(factor_a, other.denominator)
            result_a = Fraction(other.numerator, gcd_a) * self.a
            result_b = Fraction(1, other.denominator // gcd_a) * self.b
            return PolyPair(result_a, result_b)
        if isinstance(other, Polynomial):
            return PolyPair(self.a * other, self.b)
        if isinstance(other, PolyPair):
            return PolyPair(self.a * other.a, self.b * other.b)
        else:
            msg = "Multiplication for PolyPair only defined for "
            msg += "PolyPair, int, Fraction, or Polynomial."
            raise ValueError(msg)

    def __rmul__(self, other: "PolyPair | Polynomial | int | Fraction"
            ) -> "PolyPair":
        return self * other
    
    def __pos__(self) -> "PolyPair":
        return self
        
    def __neg__(self) -> "PolyPair":
        return (-1)*self
