import pickle
import re
import pathlib
import time
import argparse

import sympy
import polynomial
import polypair
import rationalfn

def load_one_function(N: int, n: int) -> list[rationalfn.RationalFn]:
    """Read a save file.  N=exponent, n=term number."""
    with open(f"saves/gregory_function_{N:02d}_{n:04d}.dat", "rb") as f:
        g = pickle.load(f)
    return g

def myobjs_to_sympy(g: list[rationalfn.RationalFn]) -> sympy.core.add.Add:
    x = sympy.symbols("x")
    f = sympy.parse_expr("0")
    for r in g:
        s = str(r)[12:]
        s = s.replace("L", "ln(x+1)")
        s = s.replace("^", "**")
        s = s.replace(")(", ")*(")
        s = s.replace("/ (", "/ ((")
        s += ")"
        # sympy crashes on e.g. 1x, 1ln(x+1), need 1*ln(x+1) so pattern match it
        regexp = "[-0-9]+ln"
        i = 0
        new_s = ""
        while (found := re.search(regexp, s[i:])) is not None:
            new_s += s[i:i + found.end() - 2]
            new_s += "*ln"
            i += found.end()
        new_s += s[i:]
        s = new_s
        regexp = "[-0-9]+x"
        i = 0
        new_s = ""
        while (found := re.search(regexp, s[i:])) is not None:
            new_s += s[i:i + found.end() - 1]
            new_s += "*x"
            i += found.end()
        new_s += s[i:]
        s = new_s
        f += sympy.parse_expr(s)
    return f

def find_one_limit(N: int, n: int) -> None:
    """Calculates the lim x->0 g_{N, n}(x) and writes to file."""
    print(f"Calculating limit for case {N=}, {n=}")
    start = time.perf_counter()
    x = sympy.symbols("x")
    g = load_one_function(N, n)
    f = myobjs_to_sympy(g)
    l = sympy.limit(f, x, 0)
    print(f"{N=} {n=} {l=}, time:{time.perf_counter() - start:.4f}")
    with open(f"saves/gregory_numbers_{N:02d}.txt", "a") as f:
        print(f"{n} {l}", file=f)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-N", default=0, type=int, help="Exponent")
    user_N = ap.parse_args().N
    base_path = pathlib.Path("saves")
    polynomial_cases = []
    for path in base_path.iterdir():
        if path.suffix == ".dat":
            spl = path.stem.split("_")
            N = int(spl[-2])
            n = int(spl[-1])
            polynomial_cases.append((N, n))
    polynomial_cases = sorted(polynomial_cases, key=lambda x: x[1])
    polynomial_cases = sorted(polynomial_cases, key=lambda x: x[0])
    # The fractions in the files all have matching polyomial cases, remove those
    for path in base_path.iterdir():
        if path.suffix == ".txt":
            spl = path.stem.split("_")
            if spl[1] == "numbers":
                N = int(spl[2])
                with open(path, "r") as f:
                    for line in f.readlines():
                        n = int(line.split()[0])
                        polynomial_cases.remove((N, n))
    for N, n in polynomial_cases:
        # If given an N from the user, only do those.
        if user_N > 0: 
            if N == user_N:
                find_one_limit(N, n)
        # otherwise, do any that are found.
        else:
            find_one_limit(N, n)    

if __name__ == "__main__":
    main()