import unittest

from polynomial import Polynomial
from polypair import PolyPair
from rationalfn import RationalFn

class TestRationalFn(unittest.TestCase):
    def setUp(self):
        self.num_polys = 6
        self.p_0 = Polynomial()
        self.p_1 = Polynomial(1)
        self.p_2 = Polynomial([1, 1])
        self.p_3 = Polynomial([-1, -1])
        self.p_4 = Polynomial([1, -2, 3, -4, 5, -6])
        self.p_5 = Polynomial([0, 0, 0, 0, 0, 1])
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
        self.assertRaises(ValueError, RationalFn, "x")
        self.assertRaises(ValueError, RationalFn, 1, "x")
        self.assertRaises(ValueError, RationalFn, 1, 1, "x")
        self.assertRaises(ValueError, RationalFn, 1, 1, 1, "x")
        self.assertRaises(ValueError, RationalFn, 1.0)
        self.assertRaises(ValueError, RationalFn, 1, 1.0)
        self.assertRaises(ValueError, RationalFn, 1, 1, 1.0)
        self.assertRaises(ValueError, RationalFn, 1, 1, 1, 1.0)
        # Too many args
        self.assertRaises(ValueError, RationalFn, 1, 1, 1, 1, 1)
        self.assertRaises(ValueError, RationalFn, 1, self.pp_23, self.p_4)
        self.assertRaises(ValueError, RationalFn, self.p_1, self.p_2, self.p_3, 
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
