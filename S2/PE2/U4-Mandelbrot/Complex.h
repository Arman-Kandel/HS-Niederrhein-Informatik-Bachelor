//
// Created by arman on 11.04.2022.
//

#ifndef LIST_COMPLEX_H
#define LIST_COMPLEX_H


class Complex {
public:
    double real, imaginary;

    Complex(double real, double imaginary);

    void add(Complex c);
    void substract(Complex c);
    void mult(Complex c);
    void divide(Complex c);
    void print();
};


#endif //LIST_COMPLEX_H
