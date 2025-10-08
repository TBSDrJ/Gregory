import unittest
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
        self.assertIsInstance(PolyPair(6, 7), PolyPair)
        self.assertIsInstance(PolyPair(37, self.p_5), PolyPair)
        self.assertIsInstance(PolyPair(self.p_5, 37), PolyPair)
        self.assertIsInstance(PolyPair(1), PolyPair)
        self.assertIsInstance(PolyPair(self.p_2), PolyPair)
        self.assertRaises(ValueError, PolyPair, 1.1, 1)
        self.assertRaises(ValueError, PolyPair, 1, 1.1)
        self.assertRaises(ValueError, PolyPair, "x", Polynomial())
        self.assertRaises(ValueError, PolyPair, Polynomial(), "x")

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
        for i in range(len(self.pp_exs)):
            for j in range(len(self.pp_exs)):
                if i == j:
                    self.assertEqual(self.pp_exs[i], self.pp_exs[j])
                # The converse is not true, for ex: 
                #   (-x-1)(-L-1) = (x+1)(L+1)
        self.assertEqual(0, self.pp_00)
        self.assertEqual(0, self.pp_04)
        self.assertEqual(1, self.pp_11)
        self.assertNotEqual(37, self.pp_11)
        self.assertNotEqual(1.0, self.pp_11)
        self.assertNotEqual("x + 1", self.pp_21)
        # If different only by a negative in both, then also equal
        self.assertEqual(self.pp_22, self.pp_33)
        self.assertEqual(self.pp_23, self.pp_32)
        self.assertEqual(PolyPair(1), PolyPair(1, 1))
        self.assertEqual(PolyPair(self.p_2), PolyPair(self.p_2, 1))

    def test_dunder_bool(self):
        self.assertFalse(PolyPair())
        self.assertTrue(PolyPair(1))

    def test_dunder_add(self):
        """Includes __radd__"""
        # Sum of two PolyPairs
        self.assertEqual(self.pp_23 + self.pp_13, PolyPair(
                Polynomial([2, 1]), self.p_3))
        self.assertEqual(self.pp_32 + self.pp_31, PolyPair(
                self.p_3, Polynomial([2, 1])))
        self.assertEqual(self.pp_00 + self.pp_13, self.pp_13)
        # PolyPair + Polynomial and reverse
        self.assertEqual(self.pp_23 + self.p_2, PolyPair(
                self.p_2, self.p_3 + 1))
        self.assertEqual(self.p_4 + self.pp_43, PolyPair(
                self.p_4, self.p_3 + 1))
        self.assertEqual(self.pp_23 + self.pp_45, [self.pp_23, self.pp_45])
        self.assertEqual(self.pp_23 + self.pp_32, [self.pp_23, self.pp_32])
        # PolyPair + int and reverse
        # Notice that PolyPair(2) = (2)(1), so self.pp_15 + 2 does not share
        #       a common factor and so won't be added.
        self.assertEqual(self.pp_15 + 1, PolyPair(1, self.p_5 + 1))
        self.assertEqual(2 + self.pp_31, PolyPair(self.p_3 + 2, 1))
        # PolyPair + Polynomial and reverse
        self.assertEqual(self.pp_35 + self.p_3, 
                PolyPair(self.p_3, self.p_5 + 1))
        self.assertEqual(self.p_3 + self.pp_35, 
                PolyPair(self.p_3, self.p_5 + 1))
        # PolyPair + PolyPair where self.a = n * other.a
        # self.assertEqual(self.pp_25 + 5*self.pp_24, PolyPair(self.p_2,
        #         self.p_5 + 5*self.p_4))
        # self.assertEqual(self.pp_14 + self.pp_24, self.pp_00)
        with self.assertRaises(ValueError): self.pp_25 + 1.0
        with self.assertRaises(ValueError): 1.0 + self.pp_25
        with self.assertRaises(ValueError): self.pp_25 + "x"
        with self.assertRaises(ValueError): "x" + self.pp_25

    def test_dunder_sub(self):
        """Includes __rsub__"""
        # Difference of two PolyPairs
        self.assertEqual(self.pp_23 - self.pp_13, PolyPair(
                Polynomial([0, 1]), self.p_3))
        self.assertEqual(self.pp_32 - self.pp_31, PolyPair(
                self.p_3, Polynomial([0, 1])))
        self.assertEqual(self.pp_13 - self.pp_00, self.pp_13)
        # PolyPair + Polynomial and reverse
        self.assertEqual(self.pp_23 - self.p_2, PolyPair(
                self.p_2, self.p_3 - 1))
        self.assertEqual(self.p_4 - self.pp_43, PolyPair(
                self.p_4, 1 - self.p_3))
        self.assertEqual(self.pp_23 - self.pp_45, [self.pp_23, (-1)*self.pp_45])
        self.assertEqual(self.pp_23 - self.pp_32, [self.pp_23, (-1)*self.pp_32])
        # PolyPair + int and reverse
        # Notice that PolyPair(2) = (2)(1), so self.pp_15 + 2 does not share
        #       a common factor and so won't be added.
        self.assertEqual(self.pp_15 - 1, PolyPair(1, self.p_5 - 1))
        self.assertEqual(2 - self.pp_31, PolyPair(2 - self.p_3, 1))
        # PolyPair - Polynomial and reverse
        self.assertEqual(self.pp_35 - self.p_3, 
                PolyPair(self.p_3, self.p_5 - 1))
        self.assertEqual(self.p_3 - self.pp_35, 
                PolyPair(self.p_3, 1 - self.p_5))
        with self.assertRaises(ValueError): self.pp_25 - 1.0
        with self.assertRaises(ValueError): 1.0 - self.pp_25
        with self.assertRaises(ValueError): self.pp_25 - "x"
        with self.assertRaises(ValueError): "x" - self.pp_25

