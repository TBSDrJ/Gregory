import math

def intlog2(n: int) -> int:
    """Determine how many bits in n."""
    if n == 0:
        return -1
    bits = 0
    while n > 0:
        n >>= 1
        bits += 1
    return bits - 1

def split_into_places(n: int) -> str:
    """Change from base 10 to base 2**64 notation, as used by the C program."""
    neg = False
    if n < 0:
        neg = True
        n = abs(n)
    digits = []
    digits.append(n % 2**64)
    while n > 0:
        n //= 2**64
        if n > 0:
            digits.append(n % 2**64)
    if neg:
        return "-" + str(digits)
    else:
        return str(digits)

def enqueue(n: int, a: int) -> int:
    """Add a*2**(k*64) to n, matches Bigint_enqueue()
    
    WARNING: This does NOT work to enqueue zeros the way the C version does.
    I'm not going to try to solve that problem."""
    bits = intlog2(n)
    if bits == 0:
        return a
    places = 0
    while bits > 0:
        bits -= 64
        places += 1
    n += a * 2**(places * 64)
    return n

def memory_leak_checks() -> None:
    """Should give the same outputs as memory_leak_checks() in bigint.c
    
    Notice that this only checks the arithmetic, use check_for_memory_leaks.py 
    to check for memory leaks."""
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

def b_0() -> int:
    return 0

def b_1() -> int:
    return 1

def b_2() -> int:
    return 37*41*43*47*53*59

def b_3() -> int:
    a = 0
    a = enqueue(a, 1 << 32)
    a = enqueue(a, 1 << 31)
    a = enqueue(a, 1 << 30)
    a = enqueue(a, 1 << 29)
    return a

def b_4() -> int:
    a = (1 << 32) + (1 << 30) * 2**(256)
    return a

def b_n() -> list[int]:
    bn = []
    bn.append(b_0())
    bn.append(b_1())
    bn.append(b_2())
    bn.append(b_3())
    bn.append(b_4())
    return bn

def arithmetic_checks() -> None:
    signs = [1, 1, 1, -1, -1, 1, -1, -1]
    exs = 5
    bn = b_n()
    bitshifts = [0, 1, 30, 100, 300]
    print("\n\nTest Addition:")
    for i in range(exs):
        a = bn[i]
        for j in range(i, exs):
            b = bn[j]
            for k in range(4):
                a = abs(a) * signs[2*k]
                b = abs(b) * signs[2*k+1]
                print(f"{split_into_places(a)} + ", end="")
                print(f"{split_into_places(b)} = ", end="")
                print(f"{split_into_places(a+b)}")
    print("\n\nTest Subtraction:")
    for i in range(exs):
        a = bn[i]
        for j in range(i, exs):
            b = bn[j]
            for k in range(4):
                a = abs(a) * signs[2*k]
                b = abs(b) * signs[2*k+1]
                print(f"{split_into_places(a)} - ", end="")
                print(f"{split_into_places(b)} = ", end="")
                print(f"{split_into_places(a-b)}")
    print("\n\nTest Multiplication:")
    for i in range(exs):
        a = bn[i]
        for j in range(i, exs):
            b = bn[j]
            for k in range(4):
                a = abs(a) * signs[2*k]
                b = abs(b) * signs[2*k+1]
                print(f"{split_into_places(a)} * ", end="")
                print(f"{split_into_places(b)} = ", end="")
                print(f"{split_into_places(a*b)}")
    print("\n\nTest Divmod:")
    for i in range(1, exs):
        a = bn[i]
        for j in range(i, exs):
            b = bn[j]
            for k in range(4):
                a = abs(a) * signs[2*k]
                b = abs(b) * signs[2*k+1]
                print(f"{split_into_places(a)} // ", end="")
                print(f"{split_into_places(b)} = ", end="")
                print(f"{split_into_places(a//b)}")
                print(f"{split_into_places(a)} % ", end="")
                print(f"{split_into_places(b)} = ", end="")
                print(f"{split_into_places(a%b)}")
                print(f"{split_into_places(b)} // ", end="")
                print(f"{split_into_places(a)} = ", end="")
                print(f"{split_into_places(b//a)}")
                print(f"{split_into_places(b)} % ", end="")
                print(f"{split_into_places(a)} = ", end="")
                print(f"{split_into_places(b%a)}")
    print("\n\nTest gcd:")
    for i in range(exs):
        a = bn[i]
        for j in range(i, exs):
            b = bn[j]
            for k in range(4):
                a = abs(a) * signs[2*k]
                b = abs(b) * signs[2*k+1]
                print(f"gcd({split_into_places(a)}, ", end="")
                print(f"{split_into_places(b)}) = ", end="")
                print(f"{split_into_places(math.gcd(a,b))}")
    print("\n\nTest intlog2:")
    for i in range(exs):
        a = bn[i]
        print(f"intlog2({a}) = {intlog2(a)}")
    print("\n\nTest bitshift_left:")
    for i in range(exs):
        a = bn[i]
        for n in bitshifts:
            print(f"{split_into_places(a)} << {n} = ", end="")
            print(f"{split_into_places(a << n)}")
    print("\n\nTest bitshift_right:")
    for i in range(exs):
        a = bn[i]
        for n in bitshifts:
            print(f"{split_into_places(a)} >> {n} = ", end="")
            print(f"{split_into_places(a >> n)}")


def main():
    # memory_leak_checks()
    arithmetic_checks()

if __name__ == "__main__":
    main()