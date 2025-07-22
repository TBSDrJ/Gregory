import random
import math
import subprocess

all_signs = [[1,1], [-1,1], [1,-1], [-1, -1]]
errors = 0
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
        67, 71, 73, 79, 83, 89, 97]

def expand(n):
    ex = []
    s = math.copysign(1, n)
    n = abs(n)
    while n > 0:
        ex.append(n % 2**64)
        n -= n % 2**64
        n //= 2**64    
    ex[0] *= int(s)
    return ex

for d in range(1000):
    for signs in all_signs:
        exs = []

        for i in range(8):
            n = 0
            for j in range(i//2 + 1):
                for k in range(16):
                    n += random.randrange(10) * 10**(k+j*16)
            exs.append(n)

        # Add in a primorial, mostly for more larger gcd results
        p = random.randrange(5, len(primes))
        primorial = 1
        for i in range(p):
            primorial *= primes[i]
        exs.append(primorial)

        separated_exs = []

        for i in range(len(exs)):
            n = exs[i]
            ex = expand(n)
            separated_exs.append(ex)

        assert(len(exs) == len(separated_exs))

        for i in range(len(exs)):
            n = 0
            for j in range(len(separated_exs[i])):
                n += separated_exs[i][j] * 2**(64*j)
            try:
                assert(n == exs[i])
            except AssertionError:
                print("Assertion Failed: These should be equal")
                print(f"{n=}")
                print(f"{exs[i]=}")
                errors += 1

        code_str = ""
        for i in range(len(separated_exs)):
            letter = chr(97+i)
            code_str += f"struct Bigint* {letter}_0 = construct_Bigint();\n"
            for n in separated_exs[i]:
                code_str += f"enqueue_to_Bigint({letter}_0, (unsigned long) {n});\n"

        m = 0
        ans = []
        pairs = []
        # Add
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint* {chr(97+i)}_{m} = "
                code_str += f"add_Bigints({chr(97+j)}_0, {chr(97+k)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                ans.append((signs[0] * exs[j]) + (signs[1] * exs[k]))
                pairs.append([j, k, 'add'])

        # Subtract one way
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint* {chr(97+i)}_{m} = "
                code_str += f"subtract_Bigints({chr(97+j)}_0, {chr(97+k)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                ans.append((signs[0] * exs[j]) - (signs[1] * exs[k]))
                pairs.append([j, k, 'subtract'])

        # Subtract reverse direction
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint* {chr(97+i)}_{m} = "
                code_str += f"subtract_Bigints({chr(97+k)}_0, {chr(97+j)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                ans.append((signs[1] * exs[k]) - (signs[0] * exs[j]))
                pairs.append([k, j, 'rev subtract'])

        # Multiply
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint* {chr(97+i)}_{m} = "
                code_str += f"multiply_Bigints({chr(97+j)}_0, {chr(97+k)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                ans.append((signs[0] * exs[j]) * (signs[1] * exs[k]))
                pairs.append([j, k, 'multiply'])

        # Divmod
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint** {chr(97+i)}_{m} = "
                code_str += f"divmod_Bigints({chr(97+j)}_0, {chr(97+k)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}[0]); printf("\\n");\n'
                code_str += f'print_Bigint({chr(97+i)}_{m}[1]); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                a = (signs[0] * exs[j]) // (signs[1] * exs[k])
                if signs[0] * signs[1] < 0:
                    a += 1 
                ans.append(a)
                pairs.append([j, k, 'div'])
                a = (signs[0] * exs[j]) % (signs[1] * exs[k])
                if signs[0] < 0 and signs[1] > 0:
                    a -= exs[k]
                elif signs[0] > 0 and signs[1] < 0:
                    a += exs[k]
                ans.append(a)
                pairs.append([j, k, 'mod'])

        # Divmod, reverse direction
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint** {chr(97+i)}_{m} = "
                code_str += f"divmod_Bigints({chr(97+k)}_0, {chr(97+j)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}[0]); printf("\\n");\n'
                code_str += f'print_Bigint({chr(97+i)}_{m}[1]); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                a = (signs[1] * exs[k]) // (signs[0] * exs[j])
                if signs[0] * signs[1] < 0:
                    a += 1
                ans.append(a)
                pairs.append([k, j, 'rev div'])
                a = (signs[1] * exs[k]) % (signs[0] * exs[j])
                if signs[0] < 0 and signs[1] > 0:
                    a += exs[j]
                elif signs[0] > 0 and signs[1] < 0:
                    a -= exs[j]
                ans.append(a)
                pairs.append([k, j, 'rev mod'])

        # gcd
        for j in range(len(separated_exs)):
            for k in range(j + 1, len(separated_exs)):
                if (i < 25):
                    i += 1
                else:
                    i -= 25
                    m += 1
                code_str += f"{chr(97+j)}_0 -> sign = {signs[0]};\n"
                code_str += f"{chr(97+k)}_0 -> sign = {signs[1]};\n"
                code_str += f"struct Bigint* {chr(97+i)}_{m} = "
                code_str += f"gcd_Bigints({chr(97+j)}_0, {chr(97+k)}_0);\n"
                code_str += f'print_Bigint({chr(97+i)}_{m}); printf("\\n");\n'
                code_str += f"{chr(97+j)}_0 -> sign = 1;\n"
                code_str += f"{chr(97+k)}_0 -> sign = 1;\n"
                ans.append(math.gcd((signs[0] * exs[j]), (signs[1] * exs[k])))
                pairs.append([j, k, 'gcd'])

        with open("bigint.c", "r") as f:
            lines = f.readlines()
        with open("bigint_test.c", "w") as fout:
            for line in lines:
                print(line[:-1], file=fout)
                if "int main()" in line:
                    break
            print(code_str, file=fout)
            print("}", file=fout)

        proc = subprocess.run(["gcc", "bigint_test.c", "-o", "bigint.out"], 
                capture_output=True)
        for line in proc.stderr.decode():
            if "warning" in line:
                if "integer literal is too large to be represented in a signed integer type" not in line:
                    print(proc.stderr.decode())
                    break
        proc = subprocess.run(["./bigint.out"], capture_output=True)
        results = proc.stdout.decode()
        results = results.split("\n")[:-1]
        k = 0
        for i in range(len(results)):
            if "DEBUG" not in results[i]:
                result = results[i].split(", ")
                if ans[k]:
                    a = expand(ans[k])
                else:
                    a = [0]
                for j in range(len(result)):
                    try:
                        assert(int(result[j]) == a[j])
                    except AssertionError:
                        print("\n\nAssertion failed:")
                        print("C result: ", result)
                        print("Python result expanded: ", a) 
                        print("Python result not expanded: ", ans[k]) 
                        m, n, op = pairs[k]
                        print("Part I: ", signs[0], expand(exs[m]))
                        print("Part II: ", signs[1], expand(exs[n]))
                        print("Operation: ", op)
                        errors += 1
                    except IndexError:
                        print("IndexError: Why are these not the same length?")
                        print("C result: ", result)
                        print("Python result expanded: ", a)
                        print("Python result not expanded: ", ans[k]) 
                        print("Part I: ", signs[0], expand(exs[m]))
                        print("Part II: ", signs[1], expand(exs[n]))
                        errors += 1
                k += 1
            else:
                print(results[i])

        if errors:
            print("All inputs:")
            for ex in exs:
                print(expand(ex))

    print(f"  {d}", end="\r")

if not errors:
    print(f"{d} tests.\nSuccessfully completed all tests with no errors.")