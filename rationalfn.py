from polynomial import Polynomial
from polypair import PolyPair

class RationalFn:
    def __init__(self, *args):
        polynomials = []
        polypairs = []
        msg_0 = "RationalFn() takes up to 4 inputs.  These can be "
        msg_0 += "int, Polynomial or PolyPair (counts as 2).\n"
        for arg in args:
            if len(polynomials) < 4:
                if isinstance(arg, int):
                    polynomials.append(Polynomial(arg))
                elif isinstance(arg, Polynomial):
                    polynomials.append(arg)
                elif isinstance(arg, PolyPair):
                    if len(polynomials) < 3:
                        if len(polynomials) == 1:
                            polynomials.append(Polynomial(1))
                            polypairs.append(PolyPair(polynomials[0], 
                                    polynomials[1]))
                        polynomials.append(arg.a)
                        polynomials.append(arg.b)
                        polypairs.append(arg)
                else:
                    msg_1 = f"Received argument {arg} of type {type(arg)}.\n"
                    raise ValueError(msg_0 + msg_1)
                if len(polynomials) == 2 and len(polypairs) == 0: 
                    polypairs.append(PolyPair(polynomials[0], 
                            polynomials[1]))
                elif len(polynomials) == 4 and len(polypairs) == 1:
                    polypairs.append(PolyPair(polynomials[2], 
                            polynomials[3]))
            else:
                msg_2 = f"Received too many arguments: {args}\n"
                raise ValueError(msg_0 + msg_2)
        while len(polynomials) < 4:
            if len(polynomials) == 0:
                polynomials.append(Polynomial(0))
            else:
                polynomials.append(Polynomial(1))
            if len(polynomials) == 2 and len(polypairs) == 0: 
                polypairs.append(PolyPair(polynomials[0], 
                        polynomials[1]))
            elif len(polynomials) == 4 and len(polypairs) == 1:
                polypairs.append(PolyPair(polynomials[2], 
                        polynomials[3]))
        self.ab = polypairs[0]
        self.cd = polypairs[1]
        self.a = polynomials[0]
        self.b = polynomials[1]
        self.c = polynomials[2]
        self.d = polynomials[3]
        if self.cd == PolyPair():
            msg = "PolyPair in the denominator == 0."
            raise ZeroDivisionError(msg)
