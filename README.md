# Gregory Numbers

This problem was brought to me by Jay Zhao, I was trying to help him run it in Maple on one of my computers, his computer couldn't handle the level of computation.

The Conjecture is this: 

For $n > 1$, the Generalized $n$<sup>th</sup> Gregory Numbers are eventually alternating in sign.

For $n = 1$, they are known to be alternating in sign for the entire sequence.

The Generalized $n$<sup>th</sup> Gregory Numbers can be computed as the coefficients of the Maclaurin Series of:

```math
\frac{x^n}{(\ln(x+1))^n}
```

This library is an attempt to provide test data for that Conjecture.  I decided at the outset to attempt to do this in possibly the most difficult and complicated way imaginable, but for reasons that are not totally crazy (see below, The Computer Science of this Library).

We ran this in Maple, but it eventually choked on it, using over 70GB of RAM.  So, the goal is to write a version that can go much farther because we can control exactly how much we are keeping in memory vs. storing to disk.

## The Mathematics of this library

In order to calculate the coefficients that we need, we will need to calculate many derivatives of the generating function.

I spent a little time playing around with the format of this function and its derivatives, and I noted that the generating function, and all terms of its derivatives, can be written in the form:

```math
f(x) = \frac{a(x)b(L)}{c(x)d(L)}
```
where $a, b, c, d \in \mathbb{Z}[x]$, and $L = \ln(x+1)$.

Each term gives rise to a pair of terms of that same form in the derivative:

```math
f'(x) = \frac{c(x)d(L)(a(x)b'(L)(\frac{1}{x+1}) + a'(x)b(L)) - a(x)b(L)(c(x)d'(L)(\frac{1}{x+1} )+ c'(x)d(L))}{c^2(x)d^2(L)}
```

```math
= \frac{a(x)(b'(L)d(L) - b(L)d'(L))}{(x+1)c(x)d^2(L)} + \frac{(a'(x)c(x) - a(x)c'(x))b(L)}{c^2(x)d(L)}
```

Evaluating these terms at $x=0$ involves multiple applications of L'Hôpital's Rule, first the $\frac{0}{0}$ form, and then the $\infty - \infty$ form.

So, we start with $a(x) = x^n, b(L) = 1, c(x) = 1, d(L) = L^n$, and then compute and evaluate derivatives at $x=0$ as far as possible.  

Of course, the number of terms grows exponentially, so we will try to combine terms where possible to knock down the growth rate a bit, but we can't sacrifice the form $f(x)$, or else the derivatives won't be able to computed using the above process, so either $a, c, d$ all need to be the same or $b, c, d$ all need to be the same in order to combine terms.  Once the program starts to generate results, we will have to evaluate this choice, the exponential growth may become a limiting factor and we may need to expand the kinds of derivatives we can take.

## The Computer Science of this Library

It was clear to me at the outset that evaluating these derivatives was going to involve very large integer calculations in order to find common denominators and carry out exact calculations.  It is clearly necessary that we not convert to float due to the possibility of round-off error causing an incorrect inversion of sign in some term.

I am at a place in my development as a programmer where I have some knowledge about C and C++, but have never developed a C or C++ library of this complexity.  I have also been thinking a lot about how C/C++ and other languages handle the binary storage and manipulation of various data types, so I decided (in a fit of madness) that the thing to do was to write my own arbitrary-precision integer arithmetic library, from scratch, out of my head, without looking up optimized algorithms or using any significant dependencies.

### Dependencies

Specifically, I am developing an arbitrary-precision library for integers, integer fractions, and integer polynomials, satisfying the following requirements:
1. The only dependency is to have an installation of `gcc`.  So far, I have tested all the code on a Mac; some of my tests have failed recently in Ubuntu linux, but I expect to make sure it works in linux and Windows in the longer run.  If you are using a different C compiler, your mileage may vary.
1. I am *not* using `math.h` at all.
1. At this point, I believe am using only these functions/constants from the C Standard Library: 
   1. `stdio.h` for `printf`, `fprintf`, `stderr` 
   1. `stdbool.h` for the `bool` data type.
   1. `stdlib.h` for `malloc` and `free`
   1. `string.h` for `memcpy` 
   I could re-factor to remove `string.h`, but I've decided to keep it, it is a very small dependency that is very reliably available.
1. I believe that I have everything set up in the libraries to be independent of the byte size of the data types.  Some of the tests require long to be 64 bits/8 bytes.  So far, I have only tested the library on 64-bit systems where `long` is 64 bits, but I think everything in the libraries themselves is flexible about the size of `long`.  I definitely assume that there are 8 bits in a byte in several places.  A change in that would involve some work, but that seems to be a very, very safe assumption these days.  

   Right now, any user of the library needs to know how many bits/bytes are in `long` and use that information when creating integer objects; I would like to remove that requirement, but I would have more thinking to do about how I am writing the constructors to accomplish that.

### The structure of the library

1. It starts with `bigint.c` and `bigint.h`, these develop a library for arithmetic of integers of arbitrary size, implemented essentially as a dynamically allocated object somewhere between a queue and a doubly-linked list in its functionality, where each entry in the queue is a 'digit' base $2^n$ where $n=$the number of bits in a `long`, so base $2^{64}$ on all the systems I've tested on.  These are printed in what might feel like reverse order, the least significant 'digit' is printed first.  So, for ex: $(1, 3) = 3*2^{64}+1$.
1. Then `myint.c` and `myint.h` develop a data structure that could be `long` or my `Bigint` -- if the input `Myint`(s) and the result each fit in size `long`, then the calculations are carried out using standard `long` arithmetic.  If any input or expected output `Myints` are larger than size `long`, then the arithmetic is switched over to `Bigint` operations.  I've read some more about this, and the way I'm doing it isn't necessarily the best, but it's done and working, so I'm not going to mess with it now.  

   I may work on this later, switching to either function pointers or the `_Generic` data type from the `C11` standard.  There's an added complication for me to make this switch, that I am tracking the sign of the `Myint` as a separate attribute, so my code assumes an `unsigned long`, and so some of the thresholds that I use to switch from `long` to `Bigint` might need to be adjusted by one or two bits. 
1. Then `frac.c` and `frac.h` handle `Fraction`s with numerator and denominator of type `Myint`.
1. Then `polynomial.c` and `polynomial.h` handle `Polynomial`s with coefficients that are of type `Myint`.
1. Right now (and expected to change at least in naming), there is `queue_2.c` and `queue_2.h` that handle polynomial products of the form $a(x)b(L)$, where $a(x)$ and $b(L)$ are of type `Polynomial`.
1. Right now (with bugs and memory leaks) `rationalfn.c` and `rationalfn.h` handle functions with a `Queue_2` product of `Polynomial`s in the numerator and denominator, and carry out cancellation of zeros and the first case of L'Hôpital's Rule.  Obviously, the second case of L'Hôpital's Rule is on the TODO list.

## Where I'm at in the development process

I've obviously been testing some as I went along, but I did a pretty terrible job of keeping track of those tests, and ensuring that the tests were thorough.  I've decided to go back and write decent tests for most of the functions, because I should have been doing this all along and now I have regrets.  The functions I am skipping in the test coverage are all used inside the functions that I'm testing, so I'm using that as the tests for those skipped functions, which should be fine, they are all getting tested across a range of scenarios.  

I did tests for `Polynomial`s first because that's where I was when I started to decide to keep track of tests, but then went back. I've finished tests for `Bigint`, `Myint` and `Fraction`.  I will double-check the `Polynomial` tests.  I've checked portability of tests to linux; `Bigint`, `Myint` and `Fraction` now pass all tests in linux.  I will test Windows next, and then I'll work on testing and ironing out the soon-to-be-renamed `Queue_2` library and the `rationalfn` library.  Then, finally, I'll be able to start thinking about generating results.  

### Estimating $\ln(2)$

At some point, I thought it would be an amusing/useful test to use the Alternating Harmonic Series as a test for the `Fraction` arithmetic.  (I was right! It was both amusing and useful.)  It is well-known that:

```math
\sum_{n=1}^\infty \frac{(-1)^{n+1}}{n} \rightarrow \ln(2)
```

Calculating this exactly is quite awful, and is a terrible way to approximate $\ln(2)$ because of the magnitude of the denominators and the fact that alternating series tend to converge much more slowly to their targets (and this one is not an exception).  But, this gave me a chance to check on accuracy, efficiency and speed.  

It prompted me to make a change in the `Fraction_add()` and `Fraction_subtract()` functions: In the first iteration, I converted both summands to have a common denominator, then did the addition or subraction, then reduced each summand and the result at the end.  But, when the numerator and/or denominator have many digits, this is a huge waste of time.  So, in the Harmonic Series example, when we get near the 10,000<sup>th</sup> term, we have over 14k bits in the running total, and then we are finding a common denominator with $\frac{1}{n}$, with $n$ having fewer than 16 bits, and then needlessly reducing that fraction with 14k bits in both numerator and denominator back down to $\frac{1}{n}$.  So, I changed it to store the summands and just replace the summands that have the common denominator with the stored values we started with when we get to the end instead of computing the reduced version.

I also remembered at some point to use the `-O2` optimizer flag in compiling the libraries and the C program that does the calculation, and this gave me a more than 8x speedup! 2 hrs, 36 mins $\rightarrow$ 18 mins, 45 secs -- I don't know if I've ever seen that big a difference using this flag before.