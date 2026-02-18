import unittest
from polynomial import Polynomial
from fractions import Fraction

class TestPolynomial(unittest.TestCase):
    """Test the Polynomial class."""
    def setUp(self):
        num_exs = 6
        self.p_0 = Polynomial([0])
        self.p_1 = Polynomial([1])
        self.p_2 = Polynomial([1, 1])
        self.p_3 = Polynomial([-1, -1])
        self.p_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.p_5 = Polynomial([0, 0, 0, 0, 0, 1])
        self.p_exs = []
        for i in range(num_exs):
            self.p_exs.append(eval(f"self.p_{i}"))

    def test_dunder_init(self):
        for ex in self.p_exs:
            self.assertIsInstance(ex, Polynomial)
        test_list = [1, 2, 3]
        poly_0 = Polynomial(test_list)
        test_list.append(4)
        self.assertEqual(poly_0.deg, 2)
        test_coeff = 3
        poly_1 = Polynomial(test_coeff)
        self.assertIsInstance(poly_1, Polynomial)
        self.assertIsInstance(poly_1.coeffs, list)
        self.assertEqual(poly_1.deg, 0)

    def test_dunder_str(self):
        """Expecting human-readable string."""
        self.assertEqual(str(self.p_0), "0")
        self.assertEqual(str(self.p_1), "1")
        self.assertEqual(str(self.p_2), "1x + 1")
        self.assertEqual(str(self.p_3), "-1x + -1")
        self.assertEqual(str(self.p_4), 
                "-6x^5 + 5x^4 + -4x^3 + 3x^2 + -2x + 1")
        self.assertEqual(str(self.p_5), "1x^5 + 0x^4 + 0x^3 + 0x^2 + 0x + 0")

    def test_dunder_repr(self):
        """Expecting Python-readable string.
        
        Will test eval(repr(p)) has correct properties in __eq__ test"""
        for i in range(len(self.p_exs)):
            self.assertIsInstance(eval(repr(self.p_exs[i])), Polynomial)

    def test_dunder_eq(self):
        self.assertEqual(self.p_0, Polynomial([0]))
        self.assertEqual(self.p_1, Polynomial([1]))
        self.assertEqual(self.p_2, Polynomial([1, 1]))
        self.assertEqual(self.p_3, Polynomial([-1, -1]))
        self.assertEqual(self.p_4, Polynomial([1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5, Polynomial([0, 0, 0, 0, 0, 1]))
        for i in range(len(self.p_exs)):
            self.assertEqual(self.p_exs[i], eval(repr(self.p_exs[i])))
        self.assertEqual(Polynomial([0, 0]), Polynomial([0]))
        self.assertEqual(Polynomial([0]), Polynomial([0, 0]))
        self.assertEqual(Polynomial([1, 1]), Polynomial([1, 1, 0, 0, 0, 0, 0]))
        self.assertEqual(Polynomial([1, 1, 0, 0, 0, 0, 0]), Polynomial([1, 1]))
        self.assertEqual(1, self.p_1)
        self.assertNotEqual(1, self.p_2)
        self.assertNotEqual("x + 1", self.p_2)
        self.assertNotEqual(1.1, self.p_0)

    def test_coeffs_validation(self):
        self.assertEqual(Polynomial(), Polynomial([0]))
        self.assertEqual(Polynomial(5), Polynomial([5]))
        self.assertRaises(TypeError, Polynomial, "x")
        self.assertRaises(TypeError, Polynomial, [1.1])

    def test_dunder_add(self):
        # and dunder radd
        for i in range(len(self.p_exs)):
            self.assertEqual(self.p_0 + self.p_exs[i], self.p_exs[i])
            self.assertEqual(self.p_exs[i] + 5, self.p_exs[i] + Polynomial(5))
            self.assertEqual(5 + self.p_exs[i], self.p_exs[i] + Polynomial(5))
        self.assertEqual(self.p_1 + self.p_1, Polynomial([2]))
        self.assertEqual(self.p_1 + self.p_2, Polynomial([2, 1]))
        self.assertEqual(self.p_1 + self.p_3, Polynomial([0, -1]))
        self.assertEqual(self.p_1 + self.p_4, 
                Polynomial([2, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_1 + self.p_5, 
                Polynomial([1, 0, 0, 0, 0, 1]))
        self.assertEqual(self.p_2 + self.p_2, Polynomial([2, 2]))
        self.assertEqual(self.p_2 + self.p_3, Polynomial([0]))
        self.assertEqual(self.p_2 + self.p_4, 
                Polynomial([2, -1, 3, -4, 5, -6]))
        self.assertEqual(self.p_2 + self.p_5, 
                Polynomial([1, 1, 0, 0, 0, 1]))
        self.assertEqual(self.p_3 + self.p_3, Polynomial([-2, -2]))
        self.assertEqual(self.p_3 + self.p_4, 
                Polynomial([0, -3, 3, -4, 5, -6]))
        self.assertEqual(self.p_3 + self.p_5, 
                Polynomial([-1, -1, 0, 0, 0, 1]))
        self.assertEqual(self.p_4 + self.p_4, 
                Polynomial([2, -4, 6, -8, 10, -12]))
        self.assertEqual(self.p_4 + self.p_5, 
                Polynomial([1, -2, 3, -4, 5, -5]))
        self.assertEqual(self.p_5 + self.p_5, 
                Polynomial([0, 0, 0, 0, 0, 2]))
        with self.assertRaises(TypeError): self.p_0 + 1.0
        with self.assertRaises(TypeError): 1.0 + self.p_0
        with self.assertRaises(TypeError): self.p_0 + "x"
        with self.assertRaises(TypeError): "x" + self.p_0

    def test_dunder_subtract(self):
        # and dunder rsub
        for i in range(len(self.p_exs)):
            self.assertEqual(self.p_exs[i] - self.p_0, self.p_exs[i])
            self.assertEqual(self.p_exs[i] - self.p_exs[i], Polynomial([0]))
            self.assertEqual(self.p_exs[i] - 5, self.p_exs[i] - Polynomial(5))
            self.assertEqual(5 - self.p_exs[i],  Polynomial(5) - self.p_exs[i])
        self.assertEqual(self.p_1 - self.p_2, Polynomial([0, -1]))
        self.assertEqual(self.p_2 - self.p_1, Polynomial([0, 1]))
        self.assertEqual(self.p_1 - self.p_3, Polynomial([2, 1]))
        self.assertEqual(self.p_3 - self.p_1, Polynomial([-2, -1]))
        self.assertEqual(self.p_1 - self.p_4, 
                Polynomial([0, 2, -3, 4, -5, 6]))
        self.assertEqual(self.p_4 - self.p_1, 
                Polynomial([0, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_1 - self.p_5, 
                Polynomial([1, 0, 0, 0, 0, -1]))
        self.assertEqual(self.p_5 - self.p_1, 
                Polynomial([-1, 0, 0, 0, 0, 1]))
        self.assertEqual(self.p_2 - self.p_3, Polynomial([2, 2]))
        self.assertEqual(self.p_3 - self.p_2, Polynomial([-2, -2]))
        self.assertEqual(self.p_2 - self.p_4, 
                Polynomial([0, 3, -3, 4, -5, 6]))
        self.assertEqual(self.p_4 - self.p_2, 
                Polynomial([0, -3, 3, -4, 5, -6]))
        self.assertEqual(self.p_2 - self.p_5, 
                Polynomial([1, 1, 0, 0, 0, -1]))
        self.assertEqual(self.p_5 - self.p_2, 
                Polynomial([-1, -1, 0, 0, 0, 1]))
        self.assertEqual(self.p_3 - self.p_4, 
                Polynomial([-2, 1, -3, 4, -5, 6]))
        self.assertEqual(self.p_4 - self.p_3, 
                Polynomial([2, -1, 3, -4, 5, -6]))
        self.assertEqual(self.p_3 - self.p_5, 
                Polynomial([-1, -1, 0, 0, 0, -1]))
        self.assertEqual(self.p_5 - self.p_3, 
                Polynomial([1, 1, 0, 0, 0, 1]))
        self.assertEqual(self.p_4 - self.p_5, 
                Polynomial([1, -2, 3, -4, 5, -7]))
        self.assertEqual(self.p_5 - self.p_4, 
                Polynomial([-1, 2, -3, 4, -5, 7]))
        with self.assertRaises(TypeError): self.p_0 - 1.0
        with self.assertRaises(TypeError): 1.0 - self.p_0
        with self.assertRaises(TypeError): self.p_0 - "x"
        with self.assertRaises(TypeError): "x" - self.p_0

    def test_dunder_mul(self):
        for i in range(len(self.p_exs)):
            self.assertEqual(self.p_0 * self.p_exs[i], Polynomial())
            self.assertEqual(self.p_1 * self.p_exs[i], self.p_exs[i])
        self.assertEqual(self.p_2 * self.p_2, Polynomial([1, 2, 1]))
        self.assertEqual(self.p_2 * self.p_3, Polynomial([-1, -2, -1]))
        self.assertEqual(self.p_2 * self.p_4, 
                Polynomial([1, -1, 1, -1, 1, -1, -6]))
        self.assertEqual(self.p_2 * self.p_5, 
                Polynomial([0, 0, 0, 0, 0, 1, 1]))
        self.assertEqual(self.p_3 * self.p_3, 
                Polynomial([1, 2, 1]))
        self.assertEqual(self.p_3 * self.p_4, 
                Polynomial([-1, 1, -1, 1, -1, 1, 6]))
        self.assertEqual(self.p_3 * self.p_5, 
                Polynomial([0, 0, 0, 0, 0, -1, -1]))
        self.assertEqual(self.p_4 * self.p_4, 
                Polynomial([1, -4, 10, -20, 35, -56, 70, -76, 73, -60, 36]))
        self.assertEqual(self.p_4 * self.p_5, 
                Polynomial([0, 0, 0, 0, 0, 1, -2, 3, -4, 5, -6]))
        self.assertEqual(self.p_5 * self.p_5, 
                Polynomial([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]))
        self.assertEqual(Fraction(1, 2) * Polynomial([6, 4, 14, 22]), 
                Polynomial([3, 2, 7, 11]))
        with self.assertRaises(TypeError): self.p_0 * 1.0
        with self.assertRaises(TypeError): 1.0 * self.p_0
        with self.assertRaises(TypeError): self.p_0 * "x"
        with self.assertRaises(TypeError): "x" * self.p_0
        self.assertEqual(Fraction(1, 2) * Polynomial([6, 4, 0, -12, 82]),
                Polynomial([3, 2, 0, -6, 41]))
        with self.assertRaises(ValueError): 
            Fraction(1, 2) * Polynomial([5, 4, 0, -12, 82])

    def test_dunder_call(self):
        for i in range(len(self.p_exs)):
            self.assertEqual(self.p_exs[i](0), self.p_exs[i].coeffs[0])
        self.assertEqual(self.p_1(5), 1)
        self.assertEqual(self.p_1(-2), 1)
        self.assertEqual(self.p_2(5), 6)
        self.assertEqual(self.p_2(-2), -1)
        self.assertEqual(self.p_3(5), -6)
        self.assertEqual(self.p_3(-2), 1)
        self.assertEqual(self.p_4(5), -16059)
        self.assertEqual(self.p_4(-2), 321) 
        self.assertEqual(self.p_5(5), 3125)
        self.assertEqual(self.p_5(-2), -32)
        self.assertRaises(TypeError, Polynomial.__call__, self.p_0, 1.1)
        self.assertRaises(TypeError, Polynomial.__call__, self.p_0, "x")

    def test_der(self):
        self.assertEqual(self.p_0.der(), Polynomial())
        self.assertEqual(self.p_1.der(), Polynomial())
        self.assertEqual(self.p_2.der(), Polynomial([1]))
        self.assertEqual(self.p_3.der(), Polynomial([-1]))
        self.assertEqual(self.p_4.der(), Polynomial([-2, 6, -12, 20, -30]))
        self.assertEqual(self.p_5.der(), Polynomial([0, 0, 0, 0, 5]))

    def test_bool(self):
        self.assertFalse(Polynomial())
        self.assertTrue(Polynomial(1))

    def test_proportional(self):
        self.assertEqual(self.p_2.proportional(self.p_2), Fraction(1))
        self.assertEqual(self.p_2.proportional(self.p_3), Fraction(-1))
        self.assertEqual(self.p_2.proportional(self.p_4), None)
        # Leading zeros
        self.assertEqual((-3*self.p_5).proportional(2*self.p_5), Fraction(-2, 3))
        # Proportional by variable should return 0 (here using x^4, x^5)
        self.assertEqual(Polynomial([0, 0, 0, 0, 1]).proportional(
                self.p_5), None)
        # Extra trivial zeros
        self.assertEqual(Polynomial([0, 0, 2, 0, 0]).proportional(
                Polynomial([0, 0, -7])), Fraction(-7, 2))