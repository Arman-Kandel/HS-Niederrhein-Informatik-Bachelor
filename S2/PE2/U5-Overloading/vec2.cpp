//
// Created by arman on 18.04.2022.
//

#include <iostream>
#include "vec2.h"
#include <cmath>
vec2::vec2(int x, int y) : x(x), y(y) {}

int vec2::operator[](int index) const {
    if(index==0) return x;
    else return y;
}

int vec2::operator*(const vec2 &rhs) const {
    return x * rhs.x + y * rhs.y;
}

vec2 vec2::operator-(const vec2 &rhs) const {
    return {x - rhs.x, y - rhs.y}; // == to vec2(...)
}

vec2 vec2::operator+(const vec2 &rhs) const {
    return {x + rhs.x, y + rhs.y}; // == to vec2(...)
}

bool vec2::operator==(const vec2 &rhs) const {
    return x == rhs.x &&
           y == rhs.y;
}

bool vec2::operator!=(const vec2 &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const vec2 &vector2) {
    os << "x: " << vector2.x << " y: " << vector2.y;
    return os;
}

bool vec2::operator<(const vec2 &rhs) const {
    if (x < rhs.x)
        return true;
    if (rhs.x < x)
        return false;
    return y < rhs.y;
}

bool vec2::operator>(const vec2 &rhs) const {
    return rhs < *this;
}

bool vec2::operator<=(const vec2 &rhs) const {
    return !(rhs < *this);
}

bool vec2::operator>=(const vec2 &rhs) const {
    return !(*this < rhs);
}

vec2 vec2::rotate(int alpha) {
    double sinAlpha = sin(alpha);
    double cosAlpha =cos(alpha);
    x = (int) (x * cosAlpha - y * sinAlpha);
    y = (int) (x * sinAlpha + y * cosAlpha);
    return *this;
}
