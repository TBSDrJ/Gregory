import math
from typing import Callable
from fractions import Fraction

class Polynomial:
    def __init__(self, coeffs: list[int] = None):
        # My intent is that coeffs[n] is the degree n term.  Notice that this
        #   means that a degree n polynomial has coeffs list of length n+1.  I 
        #   had some fun bugs in C by forgetting this.
        if coeffs is None:
            coeffs = [0]
        self.coeffs = coeffs

    def __str__(self) -> str:
        # I'm actually undecided about printing terms whose coeff is 0.
        ret_str = ""
        for n in range(1, len(self.coeffs)-1):
            ret_str += f"{self.coeffs[-n]}x^{len(self.coeffs) - n} + "
        if len(self.coeffs) > 1:
            ret_str += f"{self.coeffs[1]}x + "
        if len(self.coeffs) > 0:
            ret_str += f"{self.coeffs[0]}"
        return ret_str
        
    def __repr__(self) -> str:
        return f"Polynomial({self.coeffs})"

    def __eq__(self, other: "Polynomial") -> bool:
        if isinstance(other, int):
            other = Polynomial(other)
        elif not isinstance(other, Polynomial):
            return False
        self.eliminate_zeros()
        other.eliminate_zeros()
        return self.coeffs == other.coeffs

    def __bool__(self) -> bool:
        if self == 0:
            return False
        else:
            return True

    @property
    def coeffs(self):
        return self._coeffs
    
    @coeffs.setter
    def coeffs(self, new_coeffs: list[int]):
        """Validation: Input needs to be a list of ints or a single int."""
        if isinstance(new_coeffs, int):
            # If input is a single integer, assume it is degree 0 term.
            new_coeffs = [new_coeffs]
        elif not isinstance(new_coeffs, list):
            msg = "Polynomial() expects list of ints as its only argument.\n"
            msg += f"Received input: {new_coeffs} which is not a list."
            raise ValueError(msg)
        else:
            # If any entry is not an integer, throw ValueError.
            for i, entry in enumerate(new_coeffs):
                if not isinstance(entry, int):
                    msg = "All input list entries must be of type int.\n"
                    msg += f"Entry {i} of input list is {entry}, which "
                    msg += "is not an int."
                    raise ValueError(msg)
        self._coeffs = new_coeffs

    @property
    def deg(self):
        return len(self.coeffs) - 1

    def _add_sub(self, operation: Callable, other: "Polynomial"
            ) -> "Polynomial":
        """Combine work for __add__ and __sub__ to avoid repitition."""
        if operation not in [int.__add__, int.__sub__]:
            msg = "Method Polynomial._add_sub expected either int.__add__ or "
            msg += "int.__sub__."
            raise ValueError(msg)
        result = Polynomial()
        result.coeffs = []
        for n in range(min(len(self.coeffs), len(other.coeffs))):
            result.coeffs.append(operation(self.coeffs[n], other.coeffs[n]))
        n += 1
        # One or the other or both of these will have an empty range
        for m in range(n, len(self.coeffs)):
            result.coeffs.append(self.coeffs[m])
        for m in range(n, len(other.coeffs)):
            if operation == int.__add__:
                result.coeffs.append(other.coeffs[m])
            else:
                result.coeffs.append(-other.coeffs[m])
        return result

    def __add__(self, other: "Polynomial | int") -> "Polynomial":
        if isinstance(other, int):
            other = Polynomial(other)
        if isinstance(other, Polynomial):
            return self._add_sub(int.__add__, other)
        else:
            msg = "+ with Polynomial only defined for int, Polynomial."
            raise ValueError(msg)

    def __radd__(self, other: "Polynomial | int") -> "Polynomial":
        return self + other

    def __sub__(self, other: "Polynomial | int") -> "Polynomial":
        if isinstance(other, Polynomial):
            return self._add_sub(int.__sub__, other)
        elif isinstance(other, int):
            return self._add_sub(int.__sub__, Polynomial(other))
        else:
            msg = "- with Polynomial only defined for int, Polynomial."
            raise ValueError(msg)

    def __rsub__(self, other: "Polynomial | int") -> "Polynomial":
        return (-1)*(self - other)

    def __mul__(self, other: "Polynomial | int | Fraction") -> "Polynomial":
        result = Polynomial()
        if isinstance(other, int) or isinstance(other, Fraction):
            result.coeffs = []
            for i in range(len(self.coeffs)):
                prod = self.coeffs[i] * other
                if int(prod) != prod:
                    msg = "Can only multiply a Polynomial by a Fraction when "
                    msg += "when the product in each coefficient is an int."
                    raise ValueError(msg)
                result.coeffs.append(int(prod))
            return result
        elif not isinstance(other, Polynomial):
            msg = "* with Polynomial only defined for int, Fraction, "
            msg += "or Polynomial."
            raise ValueError(msg)
        result.coeffs = [0] * (len(self.coeffs) + len(other.coeffs) - 1)
        for i in range(len(self.coeffs)):
            for j in range(len(other.coeffs)):
                result.coeffs[i+j] += self.coeffs[i] * other.coeffs[j]
        return result

    def __rmul__(self, other: "Polynomial | int | Fraction") -> "Polynomial":
        return self * other

    def __pos__(self) -> "Polynomial":
        return self

    def __neg__(self) -> "Polynomial":
        return (-1)*self

    def eliminate_zeros(self) -> None:
        """Remove any zero terms above the largest non-zero term.
        
        If largest non-zero term is degree zero, keep it even if it's zero."""
        if self.deg > 0:
            while (self.coeffs[-1] == 0):
                self.coeffs.pop(-1)
                if self.deg == 0:
                    break

    def subs(self, val: int) -> int:
        """Substitute val into self."""
        if not isinstance(val, int):
            msg = "Polynomial.subs() only accepts int as input."
            raise ValueError(msg)
        result = self.coeffs[0]
        for i in range(1, len(self.coeffs)):
            result += self.coeffs[i] * val**i
        return result
    
    def der(self) -> "Polynomial":
        """Calculate the derivative of self."""
        d = Polynomial()
        d.coeffs = []
        for i in range(1, len(self.coeffs)):
            d.coeffs.append(i*self.coeffs[i])
        if len(d.coeffs) == 0:
            d.coeffs = [0]
        return d

    def proportional(self, other: "Polynomial | int") -> Fraction:
        """Determine if self is proportional to other by a Fraction.
        TODO? Maybe handle some simple polynomial factors.
        
        Return factor such that self * factor = other. If they are not, then 
        return None.  So this works as you would hope/expect:
            if (factor := p.proportional(q)):"""
        # If they are not the same degree, they are not proportional.
        self.eliminate_zeros()
        other.eliminate_zeros()
        if self.deg != other.deg:
            return None
        # Get the largest integer can be factored out of each.
        # NB: math.gcd() is always non-neg
        factor_self = math.gcd(*self.coeffs)
        factor_other = math.gcd(*other.coeffs)
        if factor_self == 0 or factor_other == 0:
            return None
        # Need to check if they are different by a negative factor.
        # So find lowest degree where they are both nonzero, and compare.
        # If they are proportional, it doesn't matter which degree you
        #   pick, they are all different by the same factor.  
        # If there is no degree where they are both nonzero, then they are
        #   certainly not proportional, so if search fails, return False.
        found_nonzero_pair = False
        for i in range(len(self.coeffs)):
            if self.coeffs[i] != 0 and other.coeffs[i] != 0:
                found_nonzero_pair = True
                if ((self.coeffs[i] > 0 and other.coeffs[i] < 0) or 
                        (self.coeffs[i] < 0 and other.coeffs[i] > 0)):
                    factor_self *= -1
                break
        if not found_nonzero_pair:
            return None
        # If they are proportional, Fraction(factor_other, factor_self) is it.
        for i in range(len(self.coeffs)):
            if self.coeffs[i] // factor_self != other.coeffs[i] // factor_other:
                return None
        return Fraction(factor_other, factor_self)
