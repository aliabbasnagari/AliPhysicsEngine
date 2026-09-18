#pragma once

#include <cmath>

struct Vec3
{
    float x;
    float y;
    float z;

    // Constructors
    constexpr Vec3()
        : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    constexpr Vec3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }

    // Addition
    constexpr Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction
    constexpr Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Unary negation
    constexpr Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    // Scalar multiplication
    constexpr Vec3 operator*(float scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    constexpr Vec3 operator/(float scalar) const
    {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Addition assignment
    constexpr Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // Subtraction assignment
    constexpr Vec3 &operator-=(const Vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Scalar multiplication assignment
    constexpr Vec3 &operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Dot product
    constexpr float dot(const Vec3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr Vec3 cross(const Vec3 &other) const
    {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    // Squared length
    constexpr float lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    // Length
    float length() const
    {
        return std::sqrt(lengthSquared());
    }

    // Normalized vector
    //
    // If this vector has zero length, return (0, 0).
    Vec3 normalized() const
    {
        const float len = length();

        if (len == 0.0f)
        {
            return Vec3(0.0f, 0.0f, 0.0f);
        }

        return *this / len;
    }

    // Rotate 90 degrees counter-clockwise
    //
    // (x, y) -> (-y, x)
    constexpr Vec3 perp() const
    {
        return Vec3(-y, x, z);
    }

    // Epsilon-based equality
    bool operator==(const Vec3 &other) const
    {
        const float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon;
    }

    bool operator!=(const Vec3 &other) const
    {
        return !(*this == other);
    }
};

// Allow scalar * vector as well as vector * scalar
constexpr Vec3 operator*(float scalar, const Vec3 &vector)
{
    return vector * scalar;
};