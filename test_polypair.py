import unittest
from polynomial import Polynomial
from polypair import PolyPair

class TestPolyPair(unittest.TestCase):
    def setUp(self):
        num_polys = 6
        num_exs = 1
        self.p_0 = Polynomial()
        self.p_1 = Polynomial(1)
        self.p_2 = Polynomial([1, 1])
        self.p_3 = Polynomial([-1, -1])
        self.p_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.p_5 = Polynomial([0, 0, 0, 0, 0, 1])
        self.p_exs = []
        for i in range(num_polys):
            self.p_exs.append(eval(f"self.p_{i}"))
        # p_0 paired with anything should just produce 0
        self.ex_00 = PolyPair()
        self.ex_04 = PolyPair(self.p_0, self.p_4)
        self.ex_11 = PolyPair(self.p_1, self.p_1)
        self.ex_12 = PolyPair(self.p_1, self.p_2)
        self.ex_13 = PolyPair(self.p_1, self.p_3)
        self.ex_14 = PolyPair(self.p_1, self.p_4)
        self.ex_15 = PolyPair(self.p_1, self.p_5)
        self.ex_21 = PolyPair(self.p_2, self.p_1)
        self.ex_22 = PolyPair(self.p_2, self.p_2)
        self.ex_23 = PolyPair(self.p_2, self.p_3)
        self.ex_24 = PolyPair(self.p_2, self.p_4)
        self.ex_25 = PolyPair(self.p_2, self.p_5)
        self.ex_31 = PolyPair(self.p_3, self.p_1)
        self.ex_32 = PolyPair(self.p_3, self.p_2)
        self.ex_33 = PolyPair(self.p_3, self.p_3)
        self.ex_34 = PolyPair(self.p_3, self.p_4)
        self.ex_35 = PolyPair(self.p_3, self.p_5)
        self.ex_41 = PolyPair(self.p_4, self.p_1)
        self.ex_42 = PolyPair(self.p_4, self.p_2)
        self.ex_43 = PolyPair(self.p_4, self.p_3)
        self.ex_44 = PolyPair(self.p_4, self.p_4)
        self.ex_45 = PolyPair(self.p_4, self.p_5)
        self.ex_51 = PolyPair(self.p_5, self.p_1)
        self.ex_52 = PolyPair(self.p_5, self.p_2)
        self.ex_53 = PolyPair(self.p_5, self.p_3)
        self.ex_54 = PolyPair(self.p_5, self.p_4)
        self.ex_55 = PolyPair(self.p_5, self.p_5)
        self.exs = [self.ex_00, self.ex_04]
        for i in range(1,6):
            for j in range(1,6):
                self.exs.append(eval(f"self.ex_{i}{j}"))

    def test_dunder_init(self):
        for ex in self.exs:
            self.assertIsInstance(ex, PolyPair)
        self.assertIsInstance(PolyPair(6, 7), PolyPair)
        self.assertIsInstance(PolyPair(37, self.p_5), PolyPair)
        self.assertIsInstance(PolyPair(self.p_5, 37), PolyPair)
        self.assertRaises(ValueError, PolyPair, 1.1, 1)
        self.assertRaises(ValueError, PolyPair, 1, 1.1)
        self.assertRaises(ValueError, PolyPair, "x", Polynomial())
        self.assertRaises(ValueError, PolyPair, Polynomial(), "x")

    def test_dunder_str(self):
        for ex in self.exs:
            self.assertIsInstance(str(ex), str)
        self.assertEqual(str(self.ex_15), 
                "PolyPair: (1)(1L^5 + 0L^4 + 0L^3 + 0L^2 + 0L + 0)")
        self.assertEqual(str(self.ex_51), 
                "PolyPair: (1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 0)(1)")
        self.assertEqual(str(self.ex_24), 
                "PolyPair: (1x + 1)(-6L^5 + 5L^4 + -4L^3 + 3L^2 + -2L + 1)")
        self.assertEqual(str(self.ex_42),
                "PolyPair: (-6x^5 + 5x^4 + -4x^3 + 3x^2 + -2x + 1)(1L + 1)")
        self.assertEqual(str(self.ex_33), "PolyPair: (-1x + -1)(-1L + -1)")

    def test_dunder_repr(self):
        for ex in self.exs:
            self.assertIsInstance(repr(ex), str)
            self.assertIsInstance(eval(repr(ex)), PolyPair)
            self.assertEqual(eval(repr(ex)), ex)
        self.assertEqual(repr(self.ex_15), 
                "PolyPair(Polynomial([1]), Polynomial([0, 0, 0, 0, 0, 1]))")
        repr_str = "PolyPair(Polynomial([1, 1]), "
        repr_str += "Polynomial([1, -2, 3, -4, 5, -6]))"
        self.assertEqual(repr(self.ex_24), repr_str)
        self.assertEqual(repr(self.ex_33), 
                "PolyPair(Polynomial([-1, -1]), Polynomial([-1, -1]))")
        repr_str = "PolyPair(Polynomial([1, -2, 3, -4, 5, -6]), "
        repr_str += "Polynomial([1, 1]))"
        self.assertEqual(repr(self.ex_42), repr_str)
        self.assertEqual(repr(self.ex_51), 
                "PolyPair(Polynomial([0, 0, 0, 0, 0, 1]), Polynomial([1]))")

    def test_dunder_eq(self):
        for i in range(len(self.exs)):
            for j in range(len(self.exs)):
                if i == j:
                    self.assertEqual(self.exs[i], self.exs[j])
                else:
                    self.assertNotEqual(self.exs[i], self.exs[j])
        self.assertEqual(0, self.ex_00)
        self.assertEqual(1, self.ex_11)
        self.assertNotEqual(37, self.ex_11)
        self.assertNotEqual(1.0, self.ex_11)
        self.assertNotEqual("x + 1", self.ex_21)
