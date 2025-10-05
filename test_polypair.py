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
        self.exs = []
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
