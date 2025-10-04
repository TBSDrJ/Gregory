import unittest
from polynomial import Polynomial

class TestPolynomial(unittest.TestCase):

    def setUp(self):
        num_exs = 6
        self.ex_0 = Polynomial([0])
        self.ex_1 = Polynomial([1])
        self.ex_2 = Polynomial([1, 1])
        self.ex_3 = Polynomial([-1, -1])
        self.ex_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.ex_5 = Polynomial([0, 0, 0, 0, 0, 1])
        self.exs = []
        for i in range(num_exs):
            self.exs.append(eval(f"self.ex_{i}"))

    def test_dunder_str(self):
        """Expecting human-readable string."""
        self.assertEqual(str(self.ex_0), "0")
        self.assertEqual(str(self.ex_1), "1")
        self.assertEqual(str(self.ex_2), "1x + 1")
        self.assertEqual(str(self.ex_3), "-1x + -1")
        self.assertEqual(str(self.ex_4), 
                "-6x^5 + 5x^4 + -4x^3 + 3x^2 + -2x + 1")
        self.assertEqual(str(self.ex_5), "1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 0")

    def test_dunder_repr(self):
        """Expecting Python-readable string.
        
        Will test eval(repr(p)) has correct properties in __eq__ test"""
        for i in range(len(self.exs)):
            self.assertIsInstance(eval(repr(self.exs[i])), Polynomial)

    def test_dunder_eq(self):
        self.assertEqual(self.ex_0, Polynomial([0]))
        self.assertEqual(self.ex_1, Polynomial([1]))
        self.assertEqual(self.ex_2, Polynomial([1, 1]))
        self.assertEqual(self.ex_3, Polynomial([-1, -1]))
        self.assertEqual(self.ex_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.ex_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.exs)):
            self.assertEqual(self.exs[i], eval(repr(self.exs[i])))

    def test_coeffs_validation(self):
        self.assertEqual(Polynomial(), Polynomial([0]))
        self.assertEqual(Polynomial(5), Polynomial([5]))
        self.assertRaises(ValueError, Polynomial, "x")
        self.assertRaises(ValueError, Polynomial, [1.1])

