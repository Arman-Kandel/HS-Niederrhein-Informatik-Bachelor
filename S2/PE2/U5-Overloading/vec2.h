//
// Created by arman on 18.04.2022.
//

#ifndef LIST_VEC2_H
#define LIST_VEC2_H


#include <ostream>

class vec2 {
public:
    int operator[](int index) const;

    int operator*(const vec2 &rhs) const;

    vec2 operator-(const vec2 &rhs) const;

    vec2 operator+(const vec2 &rhs) const;

    bool operator<(const vec2 &rhs) const;

    bool operator>(const vec2 &rhs) const;

    bool operator<=(const vec2 &rhs) const;

    bool operator>=(const vec2 &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const vec2 &vector2);

    bool operator==(const vec2 &rhs) const;

    bool operator!=(const vec2 &rhs) const;

    /**
     * Rotates this vector by the angle a. <br>
     * Example: By rotating vec(2, 2) by 90 degrees (param a),
     * it results in vec(-2, -2).
     */
    vec2 rotate(int a);

    /**
     * Calculates the determinant of the current and given vector. <br>
     * vec(a,b) is the current vector and vec(c,d)
     * is given as param.
     * Returns the value for a*d - b*c
     */
    int determinant(vec2 v) const;

public:
    int x, y;

    vec2(int x, int y);
};


#endif //LIST_VEC2_H
