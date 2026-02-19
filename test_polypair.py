import unittest
from fractions import Fraction

from polynomial import Polynomial
from polypair import PolyPair

class TestPolyPair(unittest.TestCase):
    def setUp(self):
        num_polys = 6
        self.p_0 = Polynomial()
        self.p_1 = Polynomial(1)
        self.p_2 = Polynomial([1, 1])
        self.p_3 = Polynomial([-1, -1])
        self.p_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.p_5 = Polynomial([0, 0, 0, 0, 0, 1])
        self.p_exs = []
        for i in range(num_polys):
            self.p_exs.append(eval(f"self.p_{i}"))
        # p_0 paired with anything should just produce 0 so I only need two
        #   of these.
        self.pp_00 = PolyPair()
        self.pp_04 = PolyPair(self.p_0, self.p_4)
        self.pp_exs = [self.pp_00, self.pp_04]
        for i in range(1, num_polys):
            for j in range(1, num_polys):
                setattr(self, f"pp_{i}{j}", PolyPair(
                        getattr(self, f"p_{i}"), getattr(self, f"p_{j}")))
                self.pp_exs.append(getattr(self, f"pp_{i}{j}"))

    def test_dunder_init(self):
        for pp in self.pp_exs:
            self.assertIsInstance(pp, PolyPair)
            self.assertIsInstance(pp.a, Polynomial)
            self.assertIsInstance(pp.b, Polynomial)
        self.assertIsInstance(PolyPair(1, 4).a, Polynomial)
        self.assertIsInstance(PolyPair(1, 4).b, Polynomial)
        self.assertIsInstance(PolyPair(6, 7), PolyPair)
        self.assertIsInstance(PolyPair(37, self.p_5), PolyPair)
        self.assertIsInstance(PolyPair(self.p_5, 37), PolyPair)
        self.assertIsInstance(PolyPair(1), PolyPair)
        self.assertIsInstance(PolyPair(self.p_2), PolyPair)
        self.assertRaises(TypeError, PolyPair, 1.1, 1)
        self.assertRaises(TypeError, PolyPair, 1, 1.1)
        self.assertRaises(TypeError, PolyPair, "x", Polynomial())
        self.assertRaises(TypeError, PolyPair, Polynomial(), "x")

    def test_dunder_str(self):
        for pp in self.pp_exs:
            self.assertIsInstance(str(pp), str)
        self.assertEqual(str(self.pp_15), 
                "PolyPair: (1)(1L^5 + 0L^4 + 0L^3 + 0L^2 + 0L + 0)")
        self.assertEqual(str(self.pp_51), 
                "PolyPair: (1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 0)(1)")
        self.assertEqual(str(self.pp_24), 
                "PolyPair: (1x + 1)(-6L^5 + 5L^4 + -4L^3 + 3L^2 + -2L + 1)")
        self.assertEqual(str(self.pp_42),
                "PolyPair: (-6x^5 + 5x^4 + -4x^3 + 3x^2 + -2x + 1)(1L + 1)")
        self.assertEqual(str(self.pp_33), "PolyPair: (-1x + -1)(-1L + -1)")

    def test_dunder_repr(self):
        for pp in self.pp_exs:
            self.assertIsInstance(repr(pp), str)
            self.assertIsInstance(eval(repr(pp)), PolyPair)
            self.assertEqual(eval(repr(pp)), pp)
        self.assertEqual(repr(self.pp_15), 
                "PolyPair(Polynomial([1]), Polynomial([0, 0, 0, 0, 0, 1]))")
        repr_str = "PolyPair(Polynomial([1, 1]), "
        repr_str += "Polynomial([1, -2, 3, -4, 5, -6]))"
        self.assertEqual(repr(self.pp_24), repr_str)
        self.assertEqual(repr(self.pp_33), 
                "PolyPair(Polynomial([-1, -1]), Polynomial([-1, -1]))")
        repr_str = "PolyPair(Polynomial([1, -2, 3, -4, 5, -6]), "
        repr_str += "Polynomial([1, 1]))"
        self.assertEqual(repr(self.pp_42), repr_str)
        self.assertEqual(repr(self.pp_51), 
                "PolyPair(Polynomial([0, 0, 0, 0, 0, 1]), Polynomial([1]))")

    def test_dunder_eq(self):
        """Test __eq__.
        
        NB: Exs with a Polynomial on the left are actually testing
        Polynomial.__eq__() not PolyPair.__eq__()."""
        # A: Easy test: Every pre-made test PolyPair is equal to itself.
        for i in range(len(self.pp_exs)):
            self.assertEqual(self.pp_exs[i], self.pp_exs[i])
            # The converse is not always true, for ex: 
            #   (-x-1)(-L-1) = (x+1)(L+1)
        # B: Any PolyPair with a 0 as one factor should be equal to 0.
        self.assertEqual(0, self.pp_00)
        self.assertEqual(0, self.pp_04)
        # C: 1 as an int or Polynomial should be equal to 1 as a PolyPair.
        self.assertEqual(1, self.pp_11)
        self.assertEqual(self.pp_11, self.p_1)
        self.assertEqual(self.p_1, self.pp_11)
        # D: PolyPair(Polynomial, 1) should == Polynomial
        for p in self.p_exs:
            self.assertEqual(PolyPair(p, 1), p)
            self.assertEqual(p, PolyPair(p, 1))
        # E: Some exs that should be not equal.
        self.assertNotEqual(37, self.pp_11)
        self.assertNotEqual(1.0, self.pp_11)
        self.assertNotEqual("x + 1", self.pp_21)
        # F: If different only by a negative in both, then also equal
        self.assertEqual(self.pp_22, self.pp_33)
        self.assertEqual(self.pp_23, self.pp_32)
        # G: Check on multiplication by 1 explicitly vs implicitly
        self.assertEqual(PolyPair(1), PolyPair(1, 1))
        self.assertEqual(PolyPair(self.p_2), PolyPair(self.p_2, 1))
        # H: Equal if only different by trivial terms
        self.assertEqual(self.pp_23, PolyPair(Polynomial([1, 1, 0, 0, 0]), 
                Polynomial([-1, -1, 0, 0])))
        # I: Also equal if different by a numerical factor moved from one
        #   side to the other
        # Move common factor left -> right
        self.assertEqual(PolyPair(Polynomial([4, 6]), Polynomial([8, -10])), 
                PolyPair(Polynomial([2, 3]), Polynomial([16, -20])))
        # Move common factor right -> left
        self.assertEqual(PolyPair(Polynomial([4, 6]), Polynomial([8, -10])), 
                PolyPair(Polynomial([8, 12]), Polynomial([4, -5])))
        # J: Off in only one spot
        self.assertNotEqual(PolyPair(Polynomial([4, 6]), Polynomial([8, -12])),
                PolyPair(Polynomial([2, 3]), Polynomial([16, -20])))
        # K: Has zero terms
        self.assertEqual(PolyPair(4*self.p_5, 3*self.p_5), PolyPair(6*self.p_5,
                2*self.p_5))
        # L: Has zero terms, factor is negative
        self.assertEqual(PolyPair(4*self.p_5, -3*self.p_5), PolyPair(6*self.p_5,
                -2*self.p_5))
        # M: PolyPair has second poly const
        self.assertEqual(PolyPair(self.p_5, 5), 5*self.p_5)
        self.assertEqual(5*self.p_5, PolyPair(self.p_5, 5))
        # Check to make sure none of the examples were modified
        self.assertEqual(self.p_0, Polynomial())
        self.assertEqual(self.p_1, Polynomial(1))
        self.assertEqual(self.p_2, Polynomial([1, 1]))
        self.assertEqual(self.p_3, Polynomial([-1, -1]))
        self.assertEqual(self.p_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.p_exs) - 1):
            for j in range(len(self.p_exs) - 1):
                self.assertEqual(PolyPair(self.p_exs[i+1], self.p_exs[j+1]), 
                        self.pp_exs[2 + i*(len(self.p_exs)-1) + j])

    def test_dunder_bool(self):
        self.assertFalse(PolyPair())
        self.assertTrue(PolyPair(1))

    def test_dunder_add(self):
        """Includes __radd__"""
        # A: Sum of two PolyPairs
        self.assertEqual(self.pp_23 + self.pp_13, PolyPair(
                Polynomial([2, 1]), self.p_3))
        self.assertEqual(self.pp_32 + self.pp_31, PolyPair(
                self.p_3, Polynomial([2, 1])))
        self.assertEqual(self.pp_00 + self.pp_13, self.pp_13)
        # B: PolyPair + Polynomial and reverse
        self.assertEqual(self.pp_23 + self.p_2, PolyPair(
                self.p_2, self.p_3 + 1))
        self.assertEqual(self.p_4 + self.pp_43, PolyPair(
                self.p_4, self.p_3 + 1))
        # C: Check addition of pairs with no common factor.
        self.assertEqual(self.pp_23 + self.pp_45, [self.pp_23, self.pp_45])
        # D: PolyPair + int and reverse
        # Notice that PolyPair(2) = (2)(1), so self.pp_15 + 2 does not share
        #       a common factor (see below for handling of that)
        self.assertEqual(self.pp_15 + 1, PolyPair(1, self.p_5 + 1))
        self.assertEqual(2 + self.pp_31, PolyPair(self.p_3 + 2, 1))
        # Don't have to worry about common factor if int is zero
        self.assertEqual(self.pp_43 + 0, self.pp_43)
        self.assertEqual(0 + self.pp_43, self.pp_43)
        # E: self.a = n * other.a or n * self.a = other.a
        # PolyPair + PolyPair 
        self.assertEqual(self.pp_25 + PolyPair(5*self.p_2, self.p_4), 
                PolyPair(self.p_2, self.p_5 + 5*self.p_4))
        self.assertEqual(PolyPair(5*self.p_2, self.p_4) + self.pp_25, 
                PolyPair(self.p_2, self.p_5 + 5*self.p_4))
        self.assertEqual(self.pp_34 + self.pp_25, PolyPair(self.p_3,
                self.p_4 - self.p_5))
        self.assertEqual(self.pp_23 + self.pp_32, PolyPair(self.p_2, 
                2*self.p_3))
        self.assertEqual(self.pp_52 + PolyPair(self.p_4, 5*self.p_2), 
                PolyPair(self.p_5 + 5*self.p_4, self.p_2))
        self.assertEqual(PolyPair(self.p_4, 5*self.p_2) + self.pp_52, 
                PolyPair(self.p_5 + 5*self.p_4, self.p_2))
        self.assertEqual(self.pp_43 + self.pp_52, PolyPair(self.p_4 - self.p_5,
                self.p_3))
        self.assertEqual(self.pp_32 + self.pp_23, PolyPair(2*self.p_3, 
                self.p_2))
        # PolyPair + Polynomial
        self.assertEqual(self.pp_25 + Polynomial([-3, -3]), PolyPair(
                self.p_3, 3 + -self.p_5))
        self.assertEqual(Polynomial([-3, -3]) + self.pp_25, PolyPair(
                self.p_2, self.p_5 - 3))
        # And PolyPair + int
        self.assertEqual(self.pp_15 + 2, PolyPair(1, self.p_5 + 2))
        self.assertEqual(2 + self.pp_15, PolyPair(1, self.p_5 + 2))
        # F: Has zero terms and one is proportional
        self.assertEqual(self.pp_54 + PolyPair(-3*self.p_5, self.p_3), 
                PolyPair(self.p_5, Polynomial([4, 1, 3, -4, 5, -6])))
        self.assertEqual(self.pp_45 + PolyPair(self.p_3, -3*self.p_5), 
                PolyPair(Polynomial([4, 1, 3, -4, 5, -6]), self.p_5))
        # G: Has a zero factor and other is proportional
        self.assertEqual(PolyPair(self.p_0, 2*self.p_3) + PolyPair(self.p_0, 
                -5*self.p_3), self.pp_00)
        self.assertEqual(PolyPair(self.p_3, self.p_0) + PolyPair(self.p_3, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(2*self.p_3, self.p_0) + PolyPair(-5*self.p_3, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(self.p_0, self.p_3) + PolyPair(self.p_0, 
                self.p_3), self.pp_00)
        # H: Has a zero factor and not proportional
        self.assertEqual(PolyPair(self.p_0, self.p_4) + PolyPair(self.p_0, 
                self.p_5), self.pp_00)
        self.assertEqual(PolyPair(self.p_4, self.p_0) + PolyPair(self.p_0, 
                self.p_5), self.pp_00)
        self.assertEqual(PolyPair(self.p_0, self.p_4) + PolyPair(self.p_5, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(self.p_4, self.p_0) + PolyPair(self.p_5, 
                self.p_0), self.pp_00)
        # I: Check data type errors
        with self.assertRaises(TypeError): self.pp_25 + 1.0
        with self.assertRaises(TypeError): 1.0 + self.pp_25
        with self.assertRaises(TypeError): self.pp_25 + "x"
        with self.assertRaises(TypeError): "x" + self.pp_25
        # Check to make sure none of the examples were modified
        self.assertEqual(self.p_0, Polynomial())
        self.assertEqual(self.p_1, Polynomial(1))
        self.assertEqual(self.p_2, Polynomial([1, 1]))
        self.assertEqual(self.p_3, Polynomial([-1, -1]))
        self.assertEqual(self.p_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.p_exs) - 1):
            for j in range(len(self.p_exs) - 1):
                self.assertEqual(PolyPair(self.p_exs[i+1], self.p_exs[j+1]), 
                        self.pp_exs[2 + i*(len(self.p_exs)-1) + j])

    def test_dunder_sub(self):
        """Includes __rsub__"""
        # A: Difference of two PolyPairs
        self.assertEqual(self.pp_23 - self.pp_13, PolyPair(
                Polynomial([0, 1]), self.p_3))
        self.assertEqual(self.pp_32 - self.pp_31, PolyPair(
                self.p_3, Polynomial([0, 1])))
        self.assertEqual(self.pp_13 - self.pp_00, self.pp_13)
        self.assertEqual(self.pp_00 - self.pp_13, -self.pp_13)
        # B: PolyPair - Polynomial and reverse
        self.assertEqual(self.pp_23 - self.p_2, PolyPair(
                self.p_2, self.p_3 - 1))
        self.assertEqual(self.p_4 - self.pp_43, PolyPair(
                self.p_4, 1 - self.p_3))
        # C: Check subtraction of pairs with no common factor.
        self.assertEqual(self.pp_23 - self.pp_45, [self.pp_23, -self.pp_45])
        self.assertEqual(self.pp_45 - self.pp_23, [self.pp_45, -self.pp_23])
        # D: PolyPair - int and reverse
        self.assertEqual(self.pp_15 - 1, PolyPair(1, self.p_5 - 1))
        self.assertEqual(2 - self.pp_31, PolyPair(2 - self.p_3, 1))
        # Don't have to worry about common factor if int is zero
        self.assertEqual(self.pp_43 - 0, self.pp_43)
        self.assertEqual(0 - self.pp_43, -self.pp_43)
        # E: self.a = n * other.a or n * self.a = other.a
        # PolyPair - PolyPair 
        self.assertEqual(self.pp_25 - PolyPair(5*self.p_2, self.p_4), 
                PolyPair(self.p_2, self.p_5 - 5*self.p_4))
        self.assertEqual(PolyPair(5*self.p_2, self.p_4) - self.pp_25, 
                PolyPair(self.p_2, -self.p_5 + 5*self.p_4))
        self.assertEqual(self.pp_34 - self.pp_25, PolyPair(self.p_3,
                self.p_4 + self.p_5))
        self.assertEqual(self.pp_34 - self.pp_25, PolyPair(self.p_2,
                -self.p_4 - self.p_5))
        self.assertEqual(self.pp_52 - PolyPair(self.p_4, 5*self.p_2), 
                PolyPair(self.p_5 - 5*self.p_4, self.p_2))
        self.assertEqual(PolyPair(self.p_4, 5*self.p_2) - self.pp_52, 
                PolyPair(-self.p_5 + 5*self.p_4, self.p_2))
        self.assertEqual(self.pp_43 - self.pp_53, PolyPair(self.p_4 - self.p_5,
                self.p_3))
        self.assertEqual(self.pp_43 - self.pp_52, PolyPair(-self.p_4 - self.p_5,
                self.p_2))
        self.assertEqual(self.pp_23 - self.pp_32, 0)
        # PolyPair - Polynomial
        self.assertEqual(self.pp_25 - Polynomial([-3, -3]), PolyPair(
                self.p_2, 3 + self.p_5))
        self.assertEqual(Polynomial([-3, -3]) - self.pp_25, PolyPair(
                self.p_2, -self.p_5 - 3))
        self.assertEqual(self.pp_25 - self.p_2, PolyPair(self.p_2, 
                -1 + self.p_5))
        self.assertEqual(self.p_2 - self.pp_25, PolyPair(self.p_2, 
                1 - self.p_5))
        # And PolyPair - int
        self.assertEqual(self.pp_15 - 2, PolyPair(1, self.p_5 - 2))
        self.assertEqual(2 - self.pp_15, PolyPair(1, -self.p_5 + 2))
        # F: Has zero terms and one is proportional
        self.assertEqual(self.pp_54 - PolyPair(-3*self.p_5, self.p_3), 
                PolyPair(self.p_5, Polynomial([-2, -5, 3, -4, 5, -6])))
        self.assertEqual(self.pp_45 - PolyPair(self.p_3, -3*self.p_5), 
                PolyPair(Polynomial([-2, -5, 3, -4, 5, -6]), self.p_5))
        # G: Has a zero factor and other is proportional
        self.assertEqual(PolyPair(self.p_0, 2*self.p_3) - PolyPair(self.p_0, 
                -5*self.p_3), self.pp_00)
        self.assertEqual(PolyPair(self.p_3, self.p_0) - PolyPair(self.p_3, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(2*self.p_3, self.p_0) - PolyPair(-5*self.p_3, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(self.p_0, self.p_3) - PolyPair(self.p_0, 
                self.p_3), self.pp_00)
        # H: Has a zero factor and not proportional
        self.assertEqual(PolyPair(self.p_0, self.p_4) - PolyPair(self.p_0, 
                self.p_5), self.pp_00)
        self.assertEqual(PolyPair(self.p_4, self.p_0) - PolyPair(self.p_0, 
                self.p_5), self.pp_00)
        self.assertEqual(PolyPair(self.p_0, self.p_4) - PolyPair(self.p_5, 
                self.p_0), self.pp_00)
        self.assertEqual(PolyPair(self.p_4, self.p_0) - PolyPair(self.p_5, 
                self.p_0), self.pp_00)
        # I: Check data type errors
        with self.assertRaises(TypeError): self.pp_25 - 1.0
        with self.assertRaises(TypeError): 1.0 - self.pp_25
        with self.assertRaises(TypeError): self.pp_25 - "x"
        with self.assertRaises(TypeError): "x" - self.pp_25
        # Check to make sure none of the examples were modified
        self.assertEqual(self.p_0, Polynomial())
        self.assertEqual(self.p_1, Polynomial(1))
        self.assertEqual(self.p_2, Polynomial([1, 1]))
        self.assertEqual(self.p_3, Polynomial([-1, -1]))
        self.assertEqual(self.p_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.p_exs) - 1):
            for j in range(len(self.p_exs) - 1):
                self.assertEqual(PolyPair(self.p_exs[i+1], self.p_exs[j+1]), 
                        self.pp_exs[2 + i*(len(self.p_exs)-1) + j])

    def test_dunder_mul(self):
        """Includes rmul"""
        # Product of two PolyPairs
        self.assertEqual(self.pp_23 * self.pp_13, PolyPair(
                Polynomial([1, 1]), self.p_3 * self.p_3))
        self.assertEqual(self.pp_32 * self.pp_31, PolyPair(
                self.p_3 * self.p_3, Polynomial([1, 1])))
        self.assertEqual(self.pp_13 * self.pp_00, self.pp_00)
        # PolyPair * Polynomial and reverse
        self.assertEqual(self.pp_23 * self.p_2, PolyPair(
                self.p_2 * self.p_2, self.p_3))
        self.assertEqual(self.p_4 * self.pp_43, PolyPair(
                self.p_4 * self.p_4, self.p_3))
        self.assertEqual(self.pp_23 * self.pp_45, PolyPair(self.p_2 * self.p_4, 
                self.p_3 * self.p_5))
        self.assertEqual(self.pp_23 * self.pp_32, PolyPair(self.p_2 * self.p_3,
                self.p_2 * self.p_3))
        # PolyPair * int and reverse
        self.assertEqual(self.pp_15 * 5, PolyPair(5, self.p_5))
        self.assertEqual(2 * self.pp_31, PolyPair(2 * self.p_3, 1))
        # PolyPair * Fraction and reverse
        # Fraction goes evenly into a
        self.assertEqual(PolyPair(Polynomial([9, -24, 15]), self.p_5) * 
                Fraction(-2, 3), PolyPair(Polynomial([-6, 16, -10]), self.p_5))
        self.assertEqual(Fraction(-2, 3) * PolyPair(Polynomial([9, -24, 15]), 
                self.p_5), PolyPair(Polynomial([-6, 16, -10]), self.p_5))
        # Fraction goes evenly into b
        self.assertEqual(PolyPair(self.p_5, Polynomial([9, -24, 15])) * 
                Fraction(-2, 3), PolyPair(self.p_5, Polynomial([-6, 16, -10])))
        # Part of Fraction goes evenly into a, part into b
        self.assertEqual(PolyPair(Polynomial([4, 6]), Polynomial([15, 10])) * 
                Fraction(-7, 10), PolyPair(Polynomial([-14, -21]), 
                Polynomial([3, 2])))
        self.assertEqual(Fraction(-7, 10) * PolyPair(Polynomial([4, 6]), 
                Polynomial([15, 10])), PolyPair(Polynomial([-14, -21]), 
                Polynomial([3, 2])))
        # PolyPair * Polynomial and reverse
        self.assertEqual(self.pp_35 * self.p_3, 
                PolyPair(self.p_3 * self.p_3, self.p_5))
        self.assertEqual(self.p_3 * self.pp_35, 
                PolyPair(self.p_3 * self.p_3, self.p_5))
        # Multiplying by a Fraction that doesn't result in integers
        with self.assertRaises(ValueError): Fraction(1, 2) * self.pp_24
        # Data type error checking
        with self.assertRaises(TypeError): self.pp_25 * 1.0
        with self.assertRaises(TypeError): 1.0 * self.pp_25
        with self.assertRaises(TypeError): self.pp_25 * "x"
        with self.assertRaises(TypeError): "x" * self.pp_25
        with self.assertRaises(TypeError): Fraction(2, 3) * PolyPair(
                Polynomial([8, 6, 4, 2]), PolyPair([2, 4, 6, 8]))
        # Check to make sure none of the examples were modified
        self.assertEqual(self.p_0, Polynomial())
        self.assertEqual(self.p_1, Polynomial(1))
        self.assertEqual(self.p_2, Polynomial([1, 1]))
        self.assertEqual(self.p_3, Polynomial([-1, -1]))
        self.assertEqual(self.p_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.p_exs) - 1):
            for j in range(len(self.p_exs) - 1):
                self.assertEqual(PolyPair(self.p_exs[i+1], self.p_exs[j+1]), 
                        self.pp_exs[2 + i*(len(self.p_exs)-1) + j])

        def test_dunder_call(self):
            self.assertEqual(self.pp_00(0), 0)
            self.assertEqual(self.pp_11(0), 1)
            self.assertEqual(self.pp_22(0), 1)
            self.assertEqual(self.pp_23(0), -1)
            self.assertEqual(self.pp_24(0), 1)
            self.assertEqual(self.pp_25(0), 0)
            self.assertIsNone(self.pp_22(1))