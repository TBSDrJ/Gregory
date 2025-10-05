from polynomial import Polynomial

class PolyPair:
    """Represents a pair a(x)b(L), where L = ln(1+x)
    
    This does *not* represent a(x)b(x), if you want that just use a*b
    or (a, b)."""
    def __init__(self, a: Polynomial  = Polynomial(), 
            b: Polynomial = Polynomial()):
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
        else:
            return False
