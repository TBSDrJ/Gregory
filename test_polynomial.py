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
        self.assertEqual(Polynomial([0, 0]), Polynomial([0]))
        self.assertEqual(Polynomial([0]), Polynomial([0, 0]))
        self.assertEqual(Polynomial([1, 1]), Polynomial([1, 1, 0, 0, 0, 0, 0]))
        self.assertEqual(Polynomial([1, 1, 0, 0, 0, 0, 0]), Polynomial([1, 1]))

    def test_coeffs_validation(self):
        self.assertEqual(Polynomial(), Polynomial([0]))
        self.assertEqual(Polynomial(5), Polynomial([5]))
        self.assertRaises(ValueError, Polynomial, "x")
        self.assertRaises(ValueError, Polynomial, [1.1])

    def test_dunder_add(self):
        # and dunder radd
        for i in range(len(self.exs)):
            self.assertEqual(self.exs[0] + self.exs[i], self.exs[i])
            self.assertEqual(self.exs[i] + 5, self.exs[i] + Polynomial(5))
            self.assertEqual(5 + self.exs[i], self.exs[i] + Polynomial(5))
        self.assertEqual(self.exs[1] + self.exs[1], Polynomial([2]))
        self.assertEqual(self.exs[1] + self.exs[2], Polynomial([2, 1]))
        self.assertEqual(self.exs[1] + self.exs[3], Polynomial([0, -1]))
        self.assertEqual(self.exs[1] + self.exs[4], 
                Polynomial([2, -2, 3, -4, 5, -6]))
        self.assertEqual(self.exs[1] + self.exs[5], 
                Polynomial([1, 0, 0, 0, 0, 1]))
        self.assertEqual(self.exs[2] + self.exs[2], Polynomial([2, 2]))
        self.assertEqual(self.exs[2] + self.exs[3], Polynomial([0]))
        self.assertEqual(self.exs[2] + self.exs[4], 
                Polynomial([2, -1, 3, -4, 5, -6]))
        self.assertEqual(self.exs[2] + self.exs[5], 
                Polynomial([1, 1, 0, 0, 0, 1]))
        self.assertEqual(self.exs[3] + self.exs[3], Polynomial([-2, -2]))
        self.assertEqual(self.exs[3] + self.exs[4], 
                Polynomial([0, -3, 3, -4, 5, -6]))
        self.assertEqual(self.exs[3] + self.exs[5], 
                Polynomial([-1, -1, 0, 0, 0, 1]))
        self.assertEqual(self.exs[4] + self.exs[4], 
                Polynomial([2, -4, 6, -8, 10, -12]))
        self.assertEqual(self.exs[4] + self.exs[5], 
                Polynomial([1, -2, 3, -4, 5, -5]))
        self.assertEqual(self.exs[5] + self.exs[5], 
                Polynomial([0, 0, 0, 0, 0, 2]))
        self.assertRaises(ValueError, Polynomial.__add__, self.exs[0], 1.1)

    def test_dunder_subtract(self):
        # and dunder rsub
        for i in range(len(self.exs)):
            self.assertEqual(self.exs[i] - self.exs[0], self.exs[i])
            self.assertEqual(self.exs[i] - self.exs[i], Polynomial([0]))
            self.assertEqual(self.exs[i] - 5, self.exs[i] - Polynomial(5))
            self.assertEqual(5 - self.exs[i],  Polynomial(5) - self.exs[i])
        self.assertEqual(self.exs[1] - self.exs[2], Polynomial([0, -1]))
        self.assertEqual(self.exs[2] - self.exs[1], Polynomial([0, 1]))
        self.assertEqual(self.exs[1] - self.exs[3], Polynomial([2, 1]))
        self.assertEqual(self.exs[3] - self.exs[1], Polynomial([-2, -1]))
        self.assertEqual(self.exs[1] - self.exs[4], 
                Polynomial([0, 2, -3, 4, -5, 6]))
        self.assertEqual(self.exs[4] - self.exs[1], 
                Polynomial([0, -2, 3, -4, 5, -6]))
        self.assertEqual(self.exs[1] - self.exs[5], 
                Polynomial([1, 0, 0, 0, 0, -1]))
        self.assertEqual(self.exs[5] - self.exs[1], 
                Polynomial([-1, 0, 0, 0, 0, 1]))
        self.assertEqual(self.exs[2] - self.exs[3], Polynomial([2, 2]))
        self.assertEqual(self.exs[3] - self.exs[2], Polynomial([-2, -2]))
        self.assertEqual(self.exs[2] - self.exs[4], 
                Polynomial([0, 3, -3, 4, -5, 6]))
        self.assertEqual(self.exs[4] - self.exs[2], 
                Polynomial([0, -3, 3, -4, 5, -6]))
        self.assertEqual(self.exs[2] - self.exs[5], 
                Polynomial([1, 1, 0, 0, 0, -1]))
        self.assertEqual(self.exs[5] - self.exs[2], 
                Polynomial([-1, -1, 0, 0, 0, 1]))
        self.assertEqual(self.exs[3] - self.exs[4], 
                Polynomial([-2, 1, -3, 4, -5, 6]))
        self.assertEqual(self.exs[4] - self.exs[3], 
                Polynomial([2, -1, 3, -4, 5, -6]))
        self.assertEqual(self.exs[3] - self.exs[5], 
                Polynomial([-1, -1, 0, 0, 0, -1]))
        self.assertEqual(self.exs[5] - self.exs[3], 
                Polynomial([1, 1, 0, 0, 0, 1]))
        self.assertEqual(self.exs[4] - self.exs[5], 
                Polynomial([1, -2, 3, -4, 5, -7]))
        self.assertEqual(self.exs[5] - self.exs[4], 
                Polynomial([-1, 2, -3, 4, -5, 7]))
        self.assertRaises(ValueError, Polynomial.__sub__, self.exs[0], 1.1)

    def test_dunder_mul(self):
        for i in range(len(self.exs)):
            self.assertEqual(self.exs[0] * self.exs[i], Polynomial())
            self.assertEqual(self.exs[1] * self.exs[i], self.exs[i])
        self.assertEqual(self.exs[2] * self.exs[2], Polynomial([1, 2, 1]))
        self.assertEqual(self.exs[2] * self.exs[3], Polynomial([-1, -2, -1]))
        self.assertEqual(self.exs[2] * self.exs[4], 
                Polynomial([1, -1, 1, -1, 1, -1, -6]))
        self.assertEqual(self.exs[2] * self.exs[5], 
                Polynomial([0, 0, 0, 0, 0, 1, 1]))
        self.assertEqual(self.exs[3] * self.exs[3], 
                Polynomial([1, 2, 1]))
        self.assertEqual(self.exs[3] * self.exs[4], 
                Polynomial([-1, 1, -1, 1, -1, 1, 6]))
        self.assertEqual(self.exs[3] * self.exs[5], 
                Polynomial([0, 0, 0, 0, 0, -1, -1]))
        self.assertEqual(self.exs[4] * self.exs[4], 
                Polynomial([1, -4, 10, -20, 35, -56, 70, -76, 73, -60, 36]))
        self.assertEqual(self.exs[4] * self.exs[5], 
                Polynomial([0, 0, 0, 0, 0, 1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.exs[5] * self.exs[5], 
                Polynomial([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]))
