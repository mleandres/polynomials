#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <string>

/////////////////////////////////////////////
// This file was given as a starting point //
/////////////////////////////////////////////

//DO NOT MODIFY THIS FILE IN ANY WAY

struct Polynomial{

    int capacity;// size of allocated array
    int *coeffs; // heap allocated array
    int max_exp; // index of element with highest
                 // non-zero coefficent

    // Ctor for a polynomial with a single term
    Polynomial(int coefficient = 0, int exponent = 0);
    // Ctor for creating a Polynomial using a string representation
    Polynomial(const std::string &str);

    ~Polynomial();
    Polynomial(const Polynomial &other);
    Polynomial(Polynomial &&other);
    Polynomial &operator=(const Polynomial &other);
    Polynomial &operator=(Polynomial &&other);

    // true if and only if two polynomials have
    // the same value for any value of the dependent variable
    bool operator==(const Polynomial &other) const;

    // Arithmetic operations behave in the standard way
    Polynomial operator+(const Polynomial &rhs) const;
    Polynomial operator-(const Polynomial &rhs) const;
    Polynomial operator*(const Polynomial &rhs) const;

    // evaluates the polynomial at a specific x value.
    int operator()(int x) const;

    // returns the coefficient of the term with that exponent
    // 0 if exponent is negative
    int operator[](int exponent) const;
};

// Print with increasing exponents.
std::ostream& operator<<(std::ostream& out, const Polynomial &poly);

// Read the rest of the line, consisting of one polynomial of form specified.
std::istream& operator>>(std::istream& in, Polynomial &poly);
#endif
