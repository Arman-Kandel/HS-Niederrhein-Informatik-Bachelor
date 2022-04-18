//
// Created by arman on 11.04.2022.
//

#include "Complex.h"
#include "iostream"

using namespace std;

Complex::Complex(double real, double imaginary) : real(real), imaginary(imaginary) {}

void Complex::print() {
    cout << real << " + " << imaginary << "i\n";
}

void Complex::divide(Complex c) {
    real = real / c.real;
    imaginary = imaginary / c.imaginary;
}

void Complex::mult(Complex c) {
    real = real * c.real;
    imaginary = imaginary * c.imaginary;
}

void Complex::substract(Complex c) {
    real = real - c.real;
    imaginary = imaginary - c.imaginary;
}

void Complex::add(Complex c) {
    real = real + c.real;
    imaginary = imaginary + c.imaginary;
}
