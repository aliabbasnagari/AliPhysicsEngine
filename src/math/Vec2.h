#pragma once

#include <cmath>

struct Vec2
{
    float x;
    float y;

    // Constructors
    constexpr Vec2()
        : x(0.0f), y(0.0f)
    {
    }

    constexpr Vec2(float x, float y)
        : x(x), y(y)
    {
    }

    // Addition
    constexpr Vec2 operator+(const Vec2 &other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    // Subtraction
    constexpr Vec2 operator-(const Vec2 &other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    // Unary negation
    constexpr Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }

    // Scalar multiplication
    constexpr Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    // Scalar division
    constexpr Vec2 operator/(float scalar) const
    {
        return Vec2(x / scalar, y / scalar);
    }

    // Addition assignment
    constexpr Vec2 &operator+=(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Subtraction assignment
    constexpr Vec2 &operator-=(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Scalar multiplication assignment
    constexpr Vec2 &operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Dot product
    constexpr float dot(const Vec2 &other) const
    {
        return x * other.x + y * other.y;
    }

    // Squared length
    constexpr float lengthSquared() const
    {
        return x * x + y * y;
    }

    // Length
    float length() const
    {
        return std::sqrt(lengthSquared());
    }

    // Normalized vector
    //
    // If this vector has zero length, return (0, 0).
    Vec2 normalized() const
    {
        const float len = length();

        if (len == 0.0f)
        {
            return Vec2(0.0f, 0.0f);
        }

        return *this / len;
    }

    // Rotate 90 degrees counter-clockwise
    //
    // (x, y) -> (-y, x)
    constexpr Vec2 perp() const
    {
        return Vec2(-y, x);
    }

    // Epsilon-based equality
    bool operator==(const Vec2 &other) const
    {
        constexpr float epsilon = 1e-5f;

        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon;
    }

    bool operator!=(const Vec2 &other) const
    {
        return !(*this == other);
    }
};

// Allow scalar * vector as well as vector * scalar
constexpr Vec2 operator*(float scalar, const Vec2 &vector)
{
    return vector * scalar;
};