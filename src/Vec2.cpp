#include "Vec2.hpp"
#include <cmath>

Vec2::Vec2(float x, float y)
    : x(x), y(y)
{
}

bool Vec2::operator==(const Vec2& other) const
{
    return (x == other.x) && (y == other.y);
}

bool Vec2::operator!=(const Vec2& other) const
{
    return (x != other.x) || (y != other.y);
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(const float& scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(const float& scalar) const
{
    return Vec2(x / scalar, y / scalar);
}

void Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
}

void Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
}

void Vec2::operator*=(const float& scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vec2::operator/=(const float& scalar)
{
    x /= scalar;
    y /= scalar;
}

void Vec2::normalize()
{
    float len = length();
    x /= len;
    y /= len;
}

float Vec2::length()
{
    return std::sqrtf(x * x + y * y);
}

float Vec2::distance(const Vec2& other) const
{
    Vec2 diff = *this - other;
    return diff.length();
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}