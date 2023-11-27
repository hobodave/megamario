#pragma once
#include <iostream>

class Vec2
{
public:
    // Member variables
    float x = 0;
    float y = 0;

    // Constructors
    Vec2() = default;
    Vec2(float x, float y);

    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);

    // Operator overloads
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const float& scalar) const;
    Vec2 operator/(const float& scalar) const;

    void operator+=(const Vec2& other);
    void operator-=(const Vec2& other);
    void operator*=(const float& scalar);
    void operator/=(const float& scalar);

    // Member functions
    void normalize();
    float length();
    float distance(const Vec2& other) const;
};
