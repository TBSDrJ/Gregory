

// Functions for Fraction
struct Fraction* construct_frac() {
    struct Bigint* numerator = construct_Bigint();
    struct Bigint* denominator = construct_Bigint();
    struct Fraction* f = malloc(sizeof(struct Fraction));
    f -> numerator = numerator;
    f -> denominator = denominator;
    return f;
}
