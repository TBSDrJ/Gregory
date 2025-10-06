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
        if a == Polynomial() or b == Polynomial():
            a = Polynomial()
            b = Polynomial()

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
        