# Gregory Numbers

This problem was brought to me by Jay Zhao, I was trying to help him run it in Maple on one of my computers, his computer couldn't handle the level of computation.

The Conjecture is this: 

For $n > 1$, the Generalized $n$<sup>th</sup> Gregory Numbers are eventually alternating in sign.

For $n = 1$, they are known to be alternating in sign for the entire sequence.

The Generalized $n$<sup>th</sup> Gregory Numbers can be computed as the coefficients of the Maclaurin Series of:

```math
\frac{x^n}{(\ln(x+1))^n}
```

This library is an attempt to provide test data for that Conjecture.  

We ran this in Maple, but it eventually choked on it, using over 70GB of RAM.  So, the goal is to write a version that can go much farther because we can control exactly how much we are keeping in memory vs. storing to disk.

### Attempt #1
I decided at the outset to attempt to do this in possibly the most difficult and complicated way imaginable, but for reasons that are not totally crazy.  See [Deprecated](deprecated/README.md) for a detailed description of what I attempted and how that went.

### Attempt #2
I am now developing a Python library to carry out these calculations.

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

## The Computer Science of the library

This time, I'm going to use Test-Driven Development from the start.

In my first attempt, I spent most of the time developing an arbitrary-precision integer arithmetic library and then a library that can do arithmetic with fractions of those.  This is all done for me in the `fractions` library provided in Python.

So, I'll start with a Polynomial library that handles arithmetic with polynomials in $\mathbb{Z}$, and its unit tests.  Then, pairs of the form $a(x)b(L)$, then fractions of those.