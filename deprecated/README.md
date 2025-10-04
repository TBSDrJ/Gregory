# Attempt #1, Now Deprecated

I spent a long time developing this arbitrary-precision integer and integer fraction arithmetic library.  It works great, in that it can accurately calculate integer arithmetic and integer fraction arithmetic to thousands of places, without a single byte of memory leaked.  If you run all the tests together, checking for memory leaks, it will `malloc` over 8.5 million objects and `free` every single one of them.

But, it turns out to be slow.  I'm sure that all of the slowness comes from the pointer chases, there are just so many separate structures allocated, and so many pointers to chase, it makes it slow.  But, I learned a *ton* from developing it, I have no regrets. 

The final blow was in the estimate of $ln(2)$, see below.  I ran this using my C library, and, after a few optimizations, got the time to compute 10,000 terms down to about 17 and a half minutes.  Then, I wrote a [15 line program](ln_of_2.py) in Python in about 3 minutes, and it calculated those same 10,000 terms in ... under a tenth of a second.  Wow.  Go Python!  So, I'm moving forward with Python.

Maybe someday I'll work on making a whole lot more of the memory contiguous, I'm sure that would help.  I'll have to think some about how to accomplish that.  

Now that I've thought a lot about the problem, I'll have to read what other people have done and get a better understanding of it.

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

I also remembered at some point to use the `-O2` optimizer flag in compiling the libraries and the C program that does the calculation, and this gave me a more than 8x speedup! 2 hrs, 36 mins $\rightarrow$ 18 mins, 45 secs -- I don't know if I've ever seen that big a difference using this flag before.  I also tried plugging my Mac laptop in, which switches it to 'High Power' mode, and this took another minute off the compute time.