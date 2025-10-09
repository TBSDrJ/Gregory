import math
from typing import Callable
from polynomial import Polynomial

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
            if (len(self.a.coeffs) == 1 and len(self.b.coeffs) == 1 and 
                    other == self.a.coeffs[0]*self.b.coeffs[0]):
                return True
            else:
                return False
        elif not isinstance(other, PolyPair):
            return False
        if self.a == other.a and self.b == other.b:
            return True
        # (-1)*(-1) cancels
        elif self.a == (-1)*other.a and self.b ==(-1)*other.b:
            return True
        else:
            return False

    def __bool__(self) -> bool:
        if self == 0:
            return False
        else:
            return True

    def _add_sub_prop(self, operation: Callable, other: "PolyPair"
            ) -> "PolyPair":
        """Case: Add and sub where self.a or b is proportional other.a or b"""
        if len(self.a.coeffs) == len(other.a.coeffs):
            factor_self = math.gcd(*self.a.coeffs)
            factor_other = math.gcd(*other.a.coeffs)
            proportional_a = True
            for i in range(len(self.a.coeffs)):
                if (abs(self.a.coeffs[i]) // factor_self != 
                        abs(other.a.coeffs[i]) // factor_other):
                    proportional_a = False
            if proportional_a:
                for i in range(len(self.a.coeffs)):
                    self.a.coeffs[i] //= factor_self
                    other.a.coeffs[i] //= factor_other
                # Might still be off by a negative; math.gcd on returns > 0
                print(self.a.coeffs, self.b.coeffs, factor_self)
                if self.a.coeffs[0] == -other.a.coeffs[0]:
                    factor_self *= -1
                    for i in range(len(self.a.coeffs)):
                        self.a.coeffs[i] *= -1
                    for i in range(len(self.b.coeffs)):
                        self.b.coeffs[i] *= -1
                print(self.a.coeffs, self.b.coeffs)
                self.b *= factor_self
                other.b *= factor_other
                return self + other
        if len(self.b.coeffs) == len(other.b.coeffs):
            factor_self = math.gcd(*self.b.coeffs)
            factor_other = math.gcd(*other.b.coeffs)
            proportional_b = True
            for i in range(len(self.b.coeffs)):
                if (self.a.coeffs[i] // factor_self !=
                        other.a.coeffs[i] // factor_other):
                    proportional_b = False
                if proportional_b:
                    for i in range(len(self.b.coeffs)):
                        self.b.coeffs[i] //= factor_self
                        other.b.coeffs[i] //= factor_other
                    self.a *= factor_self
                    self.b *= factor_other
                    return self + other
        return None

    def _add_sub(self, operation: Callable, other: "Polypair | RationalFn"
            ) -> "PolyPair | RationalFn":
        """Combine work from __add__, __sub__ to avoid repitition."""
        if operation not in [Polynomial.__add__, Polynomial.__sub__]:
            msg = "Method Polynomial._add_sub expected either "
            msg += "Polynomial.__add__ or Polynomial.__sub__."
            raise ValueError(msg)
        try:
            from rationalfn import RationalFn
        except ImportError:
            pass
        if isinstance(other, RationalFn):
            # delegate to RationalFn.__add__()
            if operation == Polynomial.__add__:
                return other + self
            else:
                return (-1)*other + self
        result = None
        if isinstance(other, PolyPair):
            if self.a == 0 or self.b == 0:
                result = other
            elif other.a == 0 or other.b == 0:
                result = self
            elif self.a == other.a:
                result = PolyPair(self.a, operation(self.b, other.b))
            elif self.b == other.b:
                result = PolyPair(operation(self.a, other.a), self.b)
            if result is None:
                result = self._add_sub_prop(operation, other)
            if result is None:
                if operation == Polynomial.__add__:
                    return [self, other]           
                else:
                    return [self, (-1)*other] 
            return result
        else:
            msg = "Addition/Subtraction for PolyPair only defined for "
            msg += "PolyPair, int, Polynomial or RationalFn."
            raise ValueError(msg)

    def __add__(self, other: "RationalFn | PolyPair | Polynomial | int"
            ) -> "RationalFn | PolyPair | list[PolyPair]":
        """Add if self.a == other.a or self.b == other.b, else return list.
        
        Returns RationalFn if other is a RationalFn. (via delegation)
        Returns [self, other] if neither a nor b is a common factor.
        Returns a PolyPair otherwise."""
        # TODO? If factor is off by a factor instead of exactly equal.
        if isinstance(other, Polynomial) or isinstance(other, int):
            other = PolyPair(other)
        return self._add_sub(Polynomial.__add__, other)

    def __radd__(self, other: "PolyPair | Polynomial | int"
            ) -> "PolyPair | list[PolyPair]":
        return self + other

    def __sub__(self, other: "RationalFn | PolyPair | Polynomial | int"
            ) -> "RationalFn | PolyPair | list[PolyPair]":
        """Add if self.a == other.a or self.b == other.b, else return list.
        
        Returns RationalFn if other is a RationalFn. (via delegation)
        Returns [self, other] if neither a nor b is a common factor.
        Returns a PolyPair otherwise."""
        # TODO? If factor is off by a factor instead of exactly equal.
        if isinstance(other, Polynomial) or isinstance(other, int):
            other = PolyPair(other)
        return self._add_sub(Polynomial.__sub__, other)

    def __rsub__(self, other: "RationalFn | PolyPair | Polynomial | int"
            ) -> "PolyPair | RationalFn | list[PolyPair]":
        return (-1)*self + other

    def __mul__(self, other: "RationalFn | PolyPair | Polynomial | int"
            ) -> "PolyPair | RationalFn":
        try: 
            from rationalfn import RationalFn
        except ImportError:
            pass
        if isinstance(other, int):
            return PolyPair(self.a * other, self.b)
        if isinstance(other, Polynomial):
            return PolyPair(self.a * other, self.b)
        if isinstance(other, PolyPair):
            return PolyPair(self.a * other.a, self.b * other.b)
        if isinstance(other, RationalFn):
            # Delegate to RationalFn.__mul__
            return other * self
        else:
            msg = "Multiplication for PolyPair only defined for "
            msg += "PolyPair, int, Polynomial or RationalFn."
            raise ValueError(msg)

    def __rmul__(self, other: "RationalFn | PolyPair | Polynomial | int"
            ) -> "PolyPair | RationalFn":
        return self * other