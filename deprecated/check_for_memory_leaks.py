"""
To use this, insert a line similar to:
    printf("malloc Bigint %li\n", (long) a);
right after using a = malloc(...);

Then a line similar to:
    printf("free Bigint %li\n", (long) a);
right before using free(a);

Then compile and run using, e.g. ./bigint.out > tmp.txt
and then just run this program with no command-line arguments.
"""

with open('tmp.ptrs.txt', 'r') as f:
    lines = list(f.readlines())
probs = False
addr = {}
mallocs = 0
frees = 0
for line in lines:
    s = line.split()
    if s[0] == "malloc":
        addr[int(s[2])] = s[1]
        mallocs += 1
    if s[0] == "free":
        ptr = int(s[2])
        frees += 1
        if ptr in addr.keys():
            if addr[ptr] == s[1]:
                del addr[ptr]
            else:
                print(f"Type mismatch. malloc {addr[ptr]}, free {s[1]} {s[2]}")
        else:
            msg = f"freed ptr {ptr} to {s[1]} not in allocated addresses"
            print(msg)
            probs = True
if addr:
    print(f"\n{len(addr)} addresses not matched.")
    for address in addr:
        found = [line for line in lines if str(address) in line]
        print(found[-1], end="")
    probs = True
if not probs:
    print("\nAll memory handled properly.  Whoo-hoo!\n\n")
print(f"Found {mallocs} mallocs and {frees} frees")
