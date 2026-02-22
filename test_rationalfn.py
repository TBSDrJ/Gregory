import unittest

from polynomial import Polynomial
from polypair import PolyPair
from rationalfn import RationalFn

class TestRationalFn(unittest.TestCase):
    def setUp(self):
        self.num_polys = 7
        self.p_0 = Polynomial()
        self.p_1 = Polynomial(1)
        self.p_2 = Polynomial([1, 1])
        self.p_3 = Polynomial([-1, -1])
        self.p_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.p_5 = Polynomial([0, 0, 0, 0, 0, 1])
        self.p_6 = Polynomial([2, 1])
        self.p_exs = []
        for i in range(self.num_polys):
            self.p_exs.append(eval(f"self.p_{i}"))
        self.pp_exs = []
        for i in range(0, self.num_polys):
            for j in range(0, self.num_polys):
                setattr(self, f"pp_{i}{j}", PolyPair(
                    getattr(self, f"p_{i}"), getattr(self, f"p_{j}")))
                self.pp_exs.append(getattr(self, f"pp_{i}{j}"))
        self.rf_exs = []
        for i in range(0, self.num_polys):
            for j in range(0, self.num_polys):
                # Switch to 1 because moving into the denominator
                for k in range(1, self.num_polys):
                    for m in range(1, self.num_polys):
                        setattr(self, f"rf_{i}{j}{k}{m}", RationalFn(
                            getattr(self, f"pp_{i}{j}"), 
                            getattr(self, f"pp_{k}{m}")))
                        self.rf_exs.append(getattr(self, f"rf_{i}{j}{k}{m}"))

    def test_init(self):
        # constructor should be flexible enough to take PolyPairs, Polynomials
        #   and integers in any combination.
        self.assertIsInstance(RationalFn(), RationalFn)
        # If one int/Polynomial is provided, args 2-4 are assumed to be 1
        self.assertIsInstance(RationalFn(1), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2), RationalFn)
        # If two ints/Polynomials are provided, args 3,4 are assumed to be 1.
        self.assertIsInstance(RationalFn(0, 1), RationalFn)
        self.assertIsInstance(RationalFn(1, 1), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 1), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_2), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_22), RationalFn)
        # Mostly if three ints/Polynomials are provided, arg 4 is assumed 1.
        self.assertIsInstance(RationalFn(0, 1, 1), RationalFn)
        self.assertIsInstance(RationalFn(1, 0, 1), RationalFn)
        self.assertIsInstance(RationalFn(1, 2, 3), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, 4), RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, 4), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, self.p_4), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, 4), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, self.p_4), RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, self.p_4), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, self.p_4), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, 4), RationalFn)
        # In this ex, the 2nd arg should be assumed to be 1.
        self.assertIsInstance(RationalFn(2, self.pp_34), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, self.p_4), RationalFn)
        # In this ex, the 2nd arg should be assumed to be 1.
        self.assertIsInstance(RationalFn(self.p_2, self.pp_34), RationalFn)
        self.assertIsInstance(RationalFn(0, 1, 1, 1), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(0, self.p_2, self.p_3, self.p_4), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, self.p_4, 5), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, 4, self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, self.p_4, 5), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, 4, self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, self.p_4, 5), RationalFn)
        self.assertIsInstance(RationalFn(3, self.p_3, 4, self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, self.p_4, self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, self.p_4, 5), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, 4, self.p_5), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, self.p_4, self.p_5), 
                RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, self.p_4, self.p_5), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, self.p_4, 
                self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(2, 3, self.pp_45), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(2, self.p_3, self.pp_45), RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, 3, self.pp_45), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, 4, 5), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, self.p_4, 5), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, 4, self.p_5), RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, self.p_4, self.p_5), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.p_2, self.p_3, self.pp_45), 
                RationalFn)
        self.assertIsInstance(RationalFn(self.pp_23, self.pp_45), RationalFn)
        # One argument of the wrong data type
        self.assertRaises(TypeError, RationalFn, "x")
        self.assertRaises(TypeError, RationalFn, 1, "x")
        self.assertRaises(TypeError, RationalFn, 1, 1, "x")
        self.assertRaises(TypeError, RationalFn, 1, 1, 1, "x")
        self.assertRaises(TypeError, RationalFn, 1.0)
        self.assertRaises(TypeError, RationalFn, 1, 1.0)
        self.assertRaises(TypeError, RationalFn, 1, 1, 1.0)
        self.assertRaises(TypeError, RationalFn, 1, 1, 1, 1.0)
        # Too many args
        self.assertRaises(TypeError, RationalFn, 1, 1, 1, 1, 1)
        self.assertRaises(TypeError, RationalFn, 1, self.pp_23, self.p_4)
        self.assertRaises(TypeError, RationalFn, self.p_1, self.p_2, self.p_3, 
                self.p_4, self.p_5)
        # 0 in the denominator
        self.assertRaises(ZeroDivisionError, RationalFn, 1, 1, 0)
        self.assertRaises(ZeroDivisionError, RationalFn, 1, 1, 1, 0)
        for i in range(0, self.num_polys):
            for j in range(0, self.num_polys):
                for k in range(1, self.num_polys):
                    for m in range(1, self.num_polys):
                        self.assertIsInstance(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), RationalFn)
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "ab"), PolyPair)
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "cd"), PolyPair)
                        # We test that ab.a etc are Polynomials in PolyPair test
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "a"), Polynomial)
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "b"), Polynomial)
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "c"), Polynomial)
                        self.assertIsInstance(getattr(getattr(self, 
                            f"rf_{i}{j}{k}{m}"), "d"), Polynomial)

    def test_dunder_str(self):
        # The second Polynomial in the numerator gets converted to 0 because
        #   anything*0 = 0, in PolyPair.__init__().
        self.assertEqual(str(self.rf_0111), "RationalFn: (0)(0) / (1)(1)")
        self.assertEqual(str(self.rf_2233), 
                "RationalFn: (1x + 1)(1L + 1) / (-1x + -1)(-1L + -1)")
        
    def test_dunder_repr(self):
        for i in range(0, self.num_polys):
            for j in range(0, self.num_polys):
                for k in range(1, self.num_polys):
                    for m in range(1, self.num_polys):
                        self.assertIsInstance(eval(repr(eval(
                                f"self.rf_{i}{j}{k}{m}"))), RationalFn)
                        # Depends on __eq__
                        self.assertEqual(eval(f"self.rf_{i}{j}{k}{m}"), 
                                eval(repr(eval(f"self.rf_{i}{j}{k}{m}"))))

    def test_dunder_eq(self):
        # Test that things that should be equal to zero actually are.
        self.assertEqual(0, RationalFn())
        self.assertEqual(0, RationalFn(0, 0, 1, 1))
        self.assertEqual(0, RationalFn(0, self.p_5, self.p_1, self.p_1))
        self.assertEqual(0, RationalFn(self.p_3, 0, self.p_4, self.p_5))
        self.assertEqual(0, RationalFn(self.p_0, self.p_5, self.p_1, self.p_1))
        self.assertEqual(0, RationalFn(self.p_3, self.p_0, self.p_4, self.p_5))
        self.assertEqual(0, RationalFn(self.pp_40, self.pp_23))
        # If one int/Polynomial is provided, args 2-4 are assumed to be 1
        self.assertEqual(RationalFn(1), RationalFn(1, 1, 1, 1))
        self.assertEqual(RationalFn(self.p_2), RationalFn(Polynomial([1, 1]), 
                1, 1, 1))
        # If two ints/Polynomials are provided, args 3,4 are assumed to be 1.
        self.assertEqual(RationalFn(0, 1), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(1, 1), RationalFn(1, 1, 1, 1))
        self.assertEqual(RationalFn(self.p_2, 1), 
                RationalFn(Polynomial([1, 1]), 1, 1, 1))
        self.assertEqual(RationalFn(self.p_2, self.p_2), 
                RationalFn(Polynomial([1, 1]), Polynomial([1, 1]), 1, 1))
        self.assertEqual(RationalFn(self.pp_22), 
                RationalFn(Polynomial([1, 1]), Polynomial([1, 1]), 1, 1))
        # Mostly if three ints/Polynomials are provided, arg 4 is assumed 1.
        self.assertEqual(RationalFn(0, 1, 1), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(1, 0, 1), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(1, 2, 3), RationalFn(1, 2, 3, 1))
        self.assertEqual(RationalFn(self.p_2, 3, 4), 
                RationalFn(Polynomial([1, 1]), 3, 4, 1))
        self.assertEqual(RationalFn(2, self.p_3, 4), 
                RationalFn(2, Polynomial([-1, -1]), 4, 1))
        self.assertEqual(RationalFn(2, 3, self.p_4), 
                RationalFn(2, 3, Polynomial([1, -2, 3, -4, 5, -6]), 1))
        self.assertEqual(RationalFn(self.p_2, self.p_3, 4), 
                RationalFn(Polynomial([1, 1]), Polynomial([-1, -1]), 4, 1))
        self.assertEqual(RationalFn(self.p_2, 3, self.p_4), RationalFn(
                Polynomial([1, 1]), 3, Polynomial([1, -2, 3, -4, 5, -6]), 1))
        self.assertEqual(RationalFn(2, self.p_3, self.p_4), RationalFn(
                2, Polynomial([-1, -1]), Polynomial([1, -2, 3, -4, 5, -6]), 1))
        self.assertEqual(RationalFn(self.p_2, self.p_3, self.p_4), RationalFn(
                Polynomial([1, 1]), Polynomial([-1, -1]), 
                Polynomial([1, -2, 3, -4, 5, -6]), 1))
        self.assertEqual(RationalFn(self.pp_23, 4), 
                RationalFn(Polynomial([1, 1]), Polynomial([-1, -1]), 4, 1))
        # In this ex, the 2nd arg should be assumed to be 1.
        self.assertEqual(RationalFn(2, self.pp_34), RationalFn(2, 1, 
                self.p_3, self.p_4))
        # Here, the 4th arg is assumed to be 1.
        self.assertEqual(RationalFn(self.pp_23, self.p_4), RationalFn(self.p_2,
                self.p_3, self.p_4, 1))
        # In this ex, the 2nd arg should be assumed to be 1.
        self.assertEqual(RationalFn(self.p_2, self.pp_34), RationalFn(self.p_2, 
                1, self.p_3, self.p_4))
        self.assertEqual(RationalFn(0, 1, 1, 1), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(1, 0, 1, 1), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(0, 1, self.pp_45), RationalFn(0, 0, 1, 1))
        self.assertEqual(RationalFn(0, self.p_2, self.p_3, self.p_4), 
                RationalFn(0, 0, self.pp_34))
        self.assertEqual(RationalFn(self.p_2, self.p_3, 4, 5), 
                RationalFn(self.pp_23, 4, 5))
        self.assertEqual(RationalFn(2, 3, self.pp_45), RationalFn(2, 3,
                self.p_4, self.p_5))
        self.assertEqual(1, RationalFn(1, 1, 1, 1))
        self.assertEqual(Polynomial(2), RationalFn(6, 4, 4, 3))
        self.assertEqual(self.p_2, RationalFn(self.p_2, 1, 1, 1))
        self.assertEqual(3*self.p_5, RationalFn(self.p_5, 12, 2, 2))

    def test_dunder_add(self):
        # Start with cases with RationalFn + RationalFn.
        # A: a, c, d match
        self.assertEqual(self.rf_2445 + self.rf_2245, RationalFn(
                self.p_2, self.p_2 + self.p_4, self.pp_45))
        # B: b, c, d match
        self.assertEqual(self.rf_2245 + self.rf_4245, RationalFn(
                self.p_2 + self.p_4, self.p_2, self.pp_45))
        # C: All 4 match
        self.assertEqual(self.rf_2345 + self.rf_2345, RationalFn(
                self.p_2, 2*self.p_3, self.pp_45))
        self.assertEqual(self.rf_2345 + self.rf_2345, RationalFn(
                2*self.p_2, self.p_3, self.pp_45))
        self.assertEqual(self.rf_2345 + self.rf_2345, RationalFn(
                2*self.pp_23, self.pp_45))
        # D: One summand is zero
        self.assertEqual(self.rf_3345 + self.rf_0011, self.rf_3345)
        self.assertEqual(self.rf_4523 + self.rf_1011, self.rf_4532)
        # E: Denominators the same, Numerators proportional
        #   Recall that self.p_2 = -self.p_3, the next batch uses that.
        self.assertEqual(self.rf_2345 + self.rf_3345, 0)
        self.assertEqual(self.rf_3345 + self.rf_2345, 0)
        self.assertEqual(self.rf_3245 + self.rf_3345, 0)
        self.assertEqual(self.rf_3345 + self.rf_3245, 0)
        #   Numerators with different proportions
        self.assertEqual(RationalFn(self.p_6, 3*self.p_2, self.pp_45) + 
                RationalFn(5*self.p_6, self.p_2, self.pp_45), RationalFn(
                self.p_6, 8*self.p_2, self.pp_45))
        self.assertEqual(RationalFn(7*self.p_6, 3*self.p_2, self.pp_45) + 
                RationalFn(5*self.p_6, 2*self.p_2, self.pp_45), RationalFn(
                self.p_6, 31*self.p_2, self.pp_45))
        # F: Denominators the same, only one numerator pair proportional
        self.assertEqual(self.rf_6245 + RationalFn(self.p_5, 5*self.p_2, 
                self.pp_45), RationalFn(Polynomial([2, 1, 0, 0, 0, 5]), 
                self.p_2, self.pp_45))
        self.assertEqual(self.rf_2645 + RationalFn(5*self.p_2, self.p_5, 
                self.pp_45), RationalFn(self.p_2, Polynomial([2, 1, 0, 0, 0, 
                5]), self.pp_45))
        # G: Denominators are proportional not equal
        #   Proportional in c or d, Numerators equal
        self.assertEqual(self.rf_6256 + RationalFn(self.pp_62, 3*self.p_5, 
                self.p_6), RationalFn(4*self.pp_62, 3*self.pp_56))
        self.assertEqual(self.rf_2665 + RationalFn(self.pp_26, self.p_6, 
                3*self.p_5), RationalFn(4*self.pp_26, 3*self.pp_65))
        #   Proportional in c and d, Numerators equal
        self.assertEqual(self.rf_6256 + RationalFn(self.pp_62, 3*self.p_5,
                5*self.p_6), RationalFn(16*self.pp_62, 15*self.pp_56))
        self.assertEqual(RationalFn(self.pp_62, 3*self.p_5, 5*self.p_6) + 
                self.rf_6256, RationalFn(16*self.pp_62, 15*self.pp_56))
        self.assertEqual(RationalFn(self.pp_62, 3*self.p_5, self.p_6) + 
                RationalFn(self.pp_62, self.p_5, 5*self.p_6), RationalFn(
                self.p_6, 8*self.p_2, 15*self.pp_56))
        # H: Denominators need common denominator, result can be added
        self.assertEqual(self.rf_6245 + self.rf_1315, RationalFn(Polynomial(
                [1, 3, -3, 4, -5, 6]), self.p_2, self.pp_45))
        self.assertEqual(self.rf_2654 + self.rf_3151, RationalFn(self.p_2, 
                Polynomial([1, 3, -3, 4, -5, 6]), self.pp_54))
        # I: Result cannot be added
        #   Denominators ok, Numerator is not
        self.assertIsNone(self.rf_3545 + self.rf_6445)
        #   Need common denominator, but then Numerators can't be added
        self.assertIsNone(self.rf_6245 + self.rf_5641)
        #   No common denominator
        self.assertIsNone(self.rf_1145 + self.rf_1154)
        # J: Check with other data types
        self.assertEqual(RationalFn(Polynomial([2, 3, 1]), self.p_1, 
                self.pp_26) + self.pp_62, RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([3, 3, 1]), self.pp_26))
        self.assertEqual(self.pp_62 + RationalFn(Polynomial([2, 3, 1]), 
                self.p_1, self.pp_26), RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([3, 3, 1]), self.pp_26))
        self.assertEqual(RationalFn(Polynomial([2, 3, 1]), self.p_1,
                self.pp_26) + self.p_6, RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([3, 1]), self.pp_26))
        self.assertEqual(self.p_5 + self.rf_1121, RationalFn(Polynomial(
                [1, 0, 0, 0, 0, 1, 1]), 1, self.p_2))
        self.assertEqual(self.rf_1116 + 3, RationalFn(1, Polynomial([7, 3]), 
                self.pp_16))
        self.assertEqual(3 + self.rf_1116, RationalFn(1, Polynomial([7, 3]), 
                self.pp_16))

    def test_dunder_sub(self):
        # Start with cases with RationalFn + RationalFn.
        # A: a, c, d match
        self.assertEqual(self.rf_2445 - self.rf_2245, RationalFn(
                self.p_2, self.p_4 - self.p_2, self.pp_45))
        # B: b, c, d match
        self.assertEqual(self.rf_2245 - self.rf_4245, RationalFn(
                self.p_2 - self.p_4, self.p_2, self.pp_45))
        # C: All 4 match
        self.assertEqual(self.rf_2345 - self.rf_2345, RationalFn(0))
        # D: One summand is zero
        self.assertEqual(self.rf_3345 - self.rf_0011, self.rf_3345)
        self.assertEqual(self.rf_4523 - self.rf_1011, self.rf_4532)
        self.assertEqual(self.rf_0011 - self.rf_3345, RationalFn(-1*self.pp_33,
                self.pp_45))
        self.assertEqual(self.rf_1011 - self.rf_4523, RationalFn(-1*self.pp_45,
                self.pp_23))
        # E: Denominators the same, Numerators proportional
        #   Recall that self.p_2 = -self.p_3, the next batch uses that.
        self.assertEqual(self.rf_2345 - self.rf_3345, RationalFn(2*self.pp_23, 
                self.pp_45))
        self.assertEqual(self.rf_3345 - self.rf_2345, RationalFn(2*self.pp_33,
                self.pp_45))
        self.assertEqual(self.rf_3245 - self.rf_3345, RationalFn(2*self.pp_32,
                self.pp_45))
        self.assertEqual(self.rf_3345 - self.rf_3245, RationalFn(2*self.pp_33,
                self.pp_45))
        #   Numerators with different proportions
        self.assertEqual(RationalFn(self.p_6, 3*self.p_2, self.pp_45) - 
                RationalFn(5*self.p_6, self.p_2, self.pp_45), RationalFn(
                self.p_6, -2*self.p_2, self.pp_45))
        self.assertEqual(RationalFn(7*self.p_6, 3*self.p_2, self.pp_45) - 
                RationalFn(5*self.p_6, 2*self.p_2, self.pp_45), RationalFn(
                self.p_6, 11*self.p_2, self.pp_45))
        # F: Denominators the same, only one numerator pair proportional
        self.assertEqual(self.rf_6245 - RationalFn(self.p_5, 5*self.p_2, 
                self.pp_45), RationalFn(Polynomial([2, 1, 0, 0, 0, -5]), 
                self.p_2, self.pp_45))
        self.assertEqual(self.rf_2645 - RationalFn(5*self.p_2, self.p_5, 
                self.pp_45), RationalFn(self.p_2, Polynomial([2, 1, 0, 0, 0, 
                -5]), self.pp_45))
        # G: Denominators are proportional not equal
        #   Proportional in c or d, Numerators equal
        self.assertEqual(self.rf_6256 - RationalFn(self.pp_62, 3*self.p_5, 
                self.p_6), RationalFn(2*self.pp_62, 3*self.pp_56))
        self.assertEqual(self.rf_2665 - RationalFn(self.pp_26, self.p_6, 
                3*self.p_5), RationalFn(2*self.pp_26, 3*self.pp_65))
        #   Proportional in c and d, Numerators equal
        self.assertEqual(self.rf_6256 - RationalFn(self.pp_62, 3*self.p_5,
                5*self.p_6), RationalFn(14*self.pp_62, 15*self.pp_56))
        self.assertEqual(RationalFn(self.pp_62, 3*self.p_5, 5*self.p_6) - 
                self.rf_6256, RationalFn(-14*self.pp_62, 15*self.pp_56))
        self.assertEqual(RationalFn(self.pp_62, 3*self.p_5, self.p_6) - 
                RationalFn(self.pp_62, self.p_5, 5*self.p_6), RationalFn(
                self.p_6, 2*self.p_2, 15*self.pp_56))
        # H: Denominators need common denominator, result can be added
        self.assertEqual(self.rf_6245 - self.rf_1315, RationalFn(Polynomial(
                [3, -1, 3, -4, 5, -6]), self.p_2, self.pp_45))
        self.assertEqual(self.rf_2654 - self.rf_3151, RationalFn(self.p_2, 
                Polynomial([3, -1, 3, -4, 5, -6]), self.pp_54))
        # I: Result cannot be added
        #   Denominators ok, Numerator is not
        self.assertIsNone(self.rf_3545 - self.rf_6445)
        #   Need common denominator, but then Numerators can't be added
        self.assertIsNone(self.rf_6245 - self.rf_5641)
        #   No common denominator
        self.assertIsNone(self.rf_1145 - self.rf_1154)
        # J: Other data types
        self.assertEqual(RationalFn(Polynomial([2, 3, 1]), self.p_1, 
                self.pp_26) - self.pp_62, RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([-1, -3, -1]), self.pp_26))
        self.assertEqual(self.pp_62 - RationalFn(Polynomial([2, 3, 1]), 
                self.p_1, self.pp_26), RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([1, 3, 1]), self.pp_26))
        self.assertEqual(RationalFn(Polynomial([2, 3, 1]), self.p_1,
                self.pp_26) - self.p_6, RationalFn(Polynomial(
                [2, 3, 1]), Polynomial([-1, -1]), self.pp_26))
        self.assertEqual(self.p_5 - self.rf_1121, RationalFn(Polynomial(
                [-1, 0, 0, 0, 0, 1, 1]), 1, self.p_2))
        self.assertEqual(self.rf_1116 - 3, RationalFn(1, Polynomial(
                [-5, -3]), self.pp_16))
        self.assertEqual(3 - self.rf_1116, RationalFn(1, Polynomial([5, 3]), 
                self.pp_16))
    
    def test_common_denominator(self):
        p_5_by_neg6 = Polynomial([0, 0, 0, 0, 0, -6])
        # A: Can't be found within constraints of function
        self.assertEqual(self.rf_1145.common_denominator(self.rf_1154), 
                (None, None))
        self.assertEqual(self.rf_1124.common_denominator(self.rf_1165),
                (None, None))
        # B: Exact same denominator.
        self.assertEqual(self.rf_1125.common_denominator(self.rf_1125), 
                (self.rf_1125, self.rf_1125))
        # C: One factor in one denominator is 1.
        self.assertEqual(self.rf_1145.common_denominator(self.rf_1115), 
                (self.rf_1145, self.rf_4145))
        self.assertEqual(self.rf_1145.common_denominator(self.rf_1141), 
                (self.rf_1145, self.rf_1545))
        self.assertEqual(self.rf_2245.common_denominator(self.rf_2215), 
                (self.rf_2245, RationalFn(self.p_2*self.p_4, self.p_2,
                self.pp_45)))
        self.assertEqual(self.rf_2245.common_denominator(self.rf_2241), 
                (self.rf_2245, RationalFn(self.p_2, self.p_2*self.p_5,
                self.pp_45)))
        # D: One factor in one denominator has degree 0, but not =1.
        self.assertEqual(self.rf_1145.common_denominator(RationalFn(1, 1,
                3, self.p_5)), (RationalFn(3, 1, 3*self.p_4, self.p_5),
                RationalFn(self.p_4, 1, 3*self.p_4, self.p_5)))
        self.assertEqual(self.rf_1145.common_denominator(RationalFn(1, 1,
                self.p_4, -7)), (RationalFn(-7, 1, self.p_4, -7*self.p_5),
                RationalFn(1, self.p_5, self.p_4, -7*self.p_5)))
        self.assertEqual(self.rf_2245.common_denominator(RationalFn(self.pp_22,
                3, self.p_5)), (RationalFn(3*self.p_2, self.p_2, 3*self.p_4, 
                self.p_5), RationalFn(self.p_4*self.p_2, self.p_2, 3*self.p_4, 
                self.p_5)))
        self.assertEqual(self.rf_2245.common_denominator(RationalFn(self.pp_22,
                self.p_4, -7)), (RationalFn(-7*self.p_2, self.p_2, self.p_4, 
                -7*self.p_5), RationalFn(self.p_2, self.p_2*self.p_5, self.p_4, 
                -7*self.p_5)))
        # E: One factor is same, other is proportional.
        self.assertEqual(self.rf_1125.common_denominator(self.rf_1135), 
                (self.rf_1125, RationalFn(-1, 1, self.pp_25)))
        self.assertEqual(self.rf_1152.common_denominator(self.rf_1153), 
                (self.rf_1152, RationalFn(-1, 1, self.pp_52)))
        self.assertEqual(self.rf_1125.common_denominator(RationalFn(1, 1,
                self.p_2, p_5_by_neg6)), (RationalFn(-6, 1, self.p_2, 
                p_5_by_neg6), RationalFn(1, 1, self.p_2, p_5_by_neg6)))
        self.assertEqual(self.rf_1152.common_denominator(RationalFn(1, 1,
                p_5_by_neg6, self.p_2)), (RationalFn(-6, 1, p_5_by_neg6, 
                self.p_2), RationalFn(1, 1, p_5_by_neg6, self.p_2)))
        self.assertEqual(self.rf_6625.common_denominator(self.rf_6635), 
                (self.rf_6625, RationalFn(-self.pp_66, self.pp_25)))
        self.assertEqual(self.rf_6652.common_denominator(self.rf_6653), 
                (self.rf_6652, RationalFn(-self.pp_66, self.pp_52)))
        self.assertEqual(self.rf_6625.common_denominator(RationalFn(self.pp_66,
                self.p_2, p_5_by_neg6)), (RationalFn(-6*self.pp_66, self.p_2, 
                p_5_by_neg6), RationalFn(self.pp_66, self.p_2, p_5_by_neg6)))
        self.assertEqual(self.rf_6652.common_denominator(RationalFn(self.pp_66,
                p_5_by_neg6, self.p_2)), (RationalFn(-6*self.pp_66, 
                p_5_by_neg6, self.p_2), RationalFn(self.pp_66, p_5_by_neg6, 
                self.p_2)))
        # F: Both factors are proportional.
        self.assertEqual(self.rf_1125.common_denominator(RationalFn(1, 1,
                self.p_3, p_5_by_neg6)), (RationalFn(-6, 1, self.p_2, 
                p_5_by_neg6), RationalFn(-1, 1, self.p_2, p_5_by_neg6)))
        self.assertEqual(self.rf_1152.common_denominator(RationalFn(1, 1,
                p_5_by_neg6, self.p_3)), (RationalFn(-6, 1, p_5_by_neg6, 
                self.p_2), RationalFn(-1, 1, p_5_by_neg6, self.p_2)))
        self.assertEqual(self.rf_6625.common_denominator(RationalFn(self.pp_66,
                self.p_3, p_5_by_neg6)), (RationalFn(-6*self.pp_66, self.p_2, 
                p_5_by_neg6), RationalFn(-self.pp_66, self.p_2, p_5_by_neg6)))
        self.assertEqual(self.rf_6652.common_denominator(RationalFn(self.pp_66,
                p_5_by_neg6, self.p_3)), (RationalFn(-6*self.pp_66, 
                p_5_by_neg6, self.p_2), RationalFn(-self.pp_66, p_5_by_neg6, 
                self.p_2)))
        # G: Both factors in one denominator are 1.
        self.assertEqual(self.rf_6115.common_denominator(self.rf_6111), 
                (self.rf_6115, self.rf_6515))
        self.assertEqual(self.rf_6111.common_denominator(self.rf_6115), 
                (self.rf_6515, self.rf_6115))
        
    def test_dunder_mul(self):
        self.assertEqual(self.rf_0011 * self.rf_3645, 0)
        self.assertEqual(self.rf_1111 * self.rf_3645, self.rf_3645)
        self.assertEqual(self.rf_3161 * self.rf_1514, self.rf_3564)
        self.assertEqual(self.rf_3652 * self.rf_6556, RationalFn(Polynomial(
                [-2, -3, -1]), Polynomial([0, 0, 0, 0, 0, 2, 1]), 
                Polynomial([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]), Polynomial(
                [2, 3, 1])))
        self.assertEqual(self.pp_62 * self.rf_3645, RationalFn(Polynomial(
                [-2, -3, -1]), Polynomial([2, 3, 1]), self.pp_45))
        self.assertEqual(self.rf_3645 * self.pp_62, RationalFn(Polynomial(
                [-2, -3, -1]), Polynomial([2, 3, 1]), self.pp_45))
        self.assertEqual(self.p_5 * self.rf_3645, RationalFn(Polynomial(
                [0, 0, 0, 0, 0, -1, -1]), self.p_6, self.pp_45))
        self.assertEqual(self.rf_3645 * self.p_5, RationalFn(Polynomial(
                [0, 0, 0, 0, 0, -1, -1]), self.p_6, self.pp_45))
        self.assertEqual(-3 * self.rf_3645, RationalFn(-3*self.pp_36, 
                self.pp_45))
        self.assertEqual(self.rf_3645 * (-3), RationalFn(-3*self.pp_36, 
                self.pp_45))
