#pragma once

#include <cmath>

#include "math/Vec3.h"
#include "math/Mat4.h"

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;

    // Identity quaternion
    // Represents no rotation.
    constexpr Quaternion()
        : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
    {
    }

    constexpr Quaternion(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
    {
    }

    // Identity rotation
    static constexpr Quaternion identity()
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Create a quaternion from an axis and angle.
    //
    // axis should be normalized.
    // Positive angle follows the right-hand rule.
    static Quaternion fromAxisAngle(Vec3 axis, float radians)
    {
        axis = axis.normalized();

        float halfAngle = radians * 0.5f;

        float s = std::sin(halfAngle);
        float c = std::cos(halfAngle);

        return Quaternion(
            axis.x * s,
            axis.y * s,
            axis.z * s,
            c);
    }

    // Quaternion multiplication.
    //
    // With this convention:
    //
    //     q1 * q2
    //
    // means q2 is applied FIRST, then q1.
    //
    // Therefore:
    //
    //     (q1 * q2).rotate(v)
    //
    // is equivalent to:
    //
    //     q1.rotate(q2.rotate(v))
    constexpr Quaternion operator*(const Quaternion &other) const
    {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z);
    }

    // Squared length
    constexpr float lengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    // Length
    float length() const
    {
        return std::sqrt(lengthSquared());
    }

    // Return a normalized quaternion.
    //
    // A zero-length quaternion returns identity.
    Quaternion normalized() const
    {
        float len = length();

        if (len == 0.0f)
        {
            return Quaternion::identity();
        }

        return Quaternion(
            x / len,
            y / len,
            z / len,
            w / len);
    }

    // Rotate a vector using this quaternion.
    //
    // v' = q * v * q^-1
    //
    // Assumes this quaternion represents a rotation.
    Vec3 rotate(const Vec3 &v) const
    {
        Quaternion q = normalized();

        // Quaternion vector part
        Vec3 u(q.x, q.y, q.z);

        // Efficient quaternion-vector rotation:
        //
        // v' = v + 2w(u x v) + 2(u x (u x v))

        Vec3 uv = u.cross(v);
        Vec3 uuv = u.cross(uv);

        return v + (uv * (2.0f * q.w)) + (uuv * 2.0f);
    }

    // Convert quaternion to a 4x4 rotation matrix.
    Mat4 toMat4() const
    {
        Quaternion q = normalized();

        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;

        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;

        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        Mat4 result = Mat4::identity();

        // Column-major, right-handed rotation matrix.

        result.data[0] = 1.0f - 2.0f * (yy + zz);
        result.data[1] = 2.0f * (xy + wz);
        result.data[2] = 2.0f * (xz - wy);

        result.data[4] = 2.0f * (xy - wz);
        result.data[5] = 1.0f - 2.0f * (xx + zz);
        result.data[6] = 2.0f * (yz + wx);

        result.data[8] = 2.0f * (xz + wy);
        result.data[9] = 2.0f * (yz - wx);
        result.data[10] = 1.0f - 2.0f * (xx + yy);

        return result;
    }
};