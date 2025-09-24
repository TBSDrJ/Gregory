import math

def intlog2(n: int) -> int:
    """Determine how many bits in n."""
    bits = 0
    while n > 0:
        n >>= 1
        bits += 1
    return bits

def split_into_places(n: int) -> list[int]:
    digits = []
    digits.append(n % 2**64)
    while n > 0:
        n //= 2**64
        if n > 0:
            digits.append(n % 2**64)
    return digits

def enqueue(n: int, a: int) -> int:
    """Add a*2**(k*64) to n"""
    bits = intlog2(n)
    if bits == 0:
        return a
    places = 0
    while bits > 0:
        bits -= 64
        places += 1
    n += a * 2**(places * 64)
    return n

def main():
    a = 1 << 32
    a = enqueue(a, 1 << 31)
    a = enqueue(a, 1 << 30)
    a = enqueue(a, 1 << 29)
    print(f"a: {split_into_places(a)}")
    b = a
    print(f"b: {split_into_places(b)}")
    c = a + b
    print(f"c: {split_into_places(c)}")
    d = a * b
    print(f"d: {split_into_places(d)}")
    e = d - c
    print(f"e: {split_into_places(e)}")
    f = math.gcd(d, e)
    print(f"f: {split_into_places(f)}")
    g = d // e
    h = d % e
    print(f"g: {split_into_places(g)}")
    print(f"h: {split_into_places(h)}")
    i = 37*41*43*47*53*59
    j = math.gcd(d, i)
    print(f"i: {split_into_places(i)}")
    print(f"j: {split_into_places(j)}")
    k = d << 100
    l = d >> 100
    print(f"k: {split_into_places(k)}")
    print(f"l: {split_into_places(l)}")






if __name__ == "__main__":
    main()