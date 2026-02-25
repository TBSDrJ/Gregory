import pickle
import pathlib
import argparse
import time

from polynomial import Polynomial
from polypair import PolyPair
from rationalfn import RationalFn

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-N", required=True, help="Exponent on x, L", type=int)
    N = ap.parse_args().N
    # Initial condition if never run before
    coeffs = [0]*(N + 1)
    coeffs[-1] = 1
    p = Polynomial(coeffs)
    # Make a copy of coeffs so they don't both point to the same list
    q = Polynomial(coeffs[:])
    g = [RationalFn(p, 1, 1, q)]
    # Figure out where we left off and get set up to start there
    base_path = pathlib.Path("saves")
    n = 0
    for path in base_path.iterdir():
        s = path.stem.split("_")
        if path.suffix == ".dat" and int(s[-2]) == N:
            tmp = int(s[-1])
            if tmp > n:
                n = tmp
    if n > 0:
        print(f"Restarting at {N=}, {n=}")
        with open(f"saves/gregory_function_{N:02d}_{n:04d}.dat", "rb") as f:
            g = pickle.load(f)
    while True:
        with open(f"saves/gregory_function_{N:02d}_{n:04d}.dat", "wb") as f:
            pickle.dump(g, f)
        start = time.perf_counter()
        n += 1
        # Calculate the derivative of each term in the sequence/sum
        d = []
        for r in g:
            d += list(r.der())
        # Try to combine any terms that can be combined, append those as we
        #   find them, skip them after
        g = []
        got = []
        for i in range(len(d)):
            for j in range(i):
                if i not in got and j not in got:
                    if (sum := d[i] + d[j]) is not None:
                        g.append(sum)
                        got += [i, j]
        # Go back and get the ones that didn't combine with any others.
        for i in range(len(d)):
            if i not in got:
                g.append(d[i])
        # Now, g has all of the results, so loop again.
        print(f"Completed case {N=}, {n=}, {len(g)=} "
            f"time={time.perf_counter() - start:.4f}")

if __name__ == "__main__":
    main()