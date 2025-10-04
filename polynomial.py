class Polynomial:
    def __init__(self, coeffs: list[int] = [0]):
        # My intent is that coeffs[n] is the degree n term.  Notice that this
        #   means that a degree n polynomial has coeffs list of length n+1.
        self.coeffs = coeffs

    def __str__(self) -> str:
        # I'm actually undecided about printing terms whose coeff is 0.
        ret_str = ""
        for n in range(1, len(self.coeffs)-1):
            ret_str += f"{self.coeffs[-n]}x^{len(self.coeffs) - n} + "
        if len(self.coeffs) > 1:
            ret_str += f"{self.coeffs[1]}x + "
        if len(self.coeffs) > 0:
            ret_str += f"{self.coeffs[0]}"
        return ret_str
        
    def __repr__(self) -> str:
        return f"Polynomial({self.coeffs})"

    def __eq__(self, other: "Polynomial") -> bool:
        return self.coeffs == other.coeffs

    @property
    def coeffs(self):
        return self._coeffs
    
    @coeffs.setter
    def coeffs(self, new_coeffs: list[int]):
        """Validation: Input needs to be a list of ints or a single int."""
        if isinstance(new_coeffs, int):
            # If input is a single integer, assume it is degree 0 term.
            new_coeffs = [new_coeffs]
        elif not isinstance(new_coeffs, list):
            msg = "Polynomial() expects list of ints as its only argument.\n"
            msg += f"Received input: {new_coeffs} which is not a list."
            raise ValueError(msg)
        else:
            # If any entry is not an integer, throw ValueError.
            for i, entry in enumerate(new_coeffs):
                if not isinstance(entry, int):
                    msg = "All input list entries must be of type int.\n"
                    msg += f"Entry {i} of input list is {entry}, which "
                    msg += "is not an int."
                    raise ValueError(msg)
        self._coeffs = new_coeffs

    def __add__(self, other: "Polynomial") -> "Polynomial":
        pass