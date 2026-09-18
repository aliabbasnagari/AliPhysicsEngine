#pragma once

#include <cmath>
#include "math/Vec3.h"

struct Mat4
{
    // Column-major storage.
    //
    // data[4 * column + row] = element at (row, column)
    //
    // Vectors are column vectors:
    //     result = Mat4 * Vec3
    //
    // This layout is also directly compatible with OpenGL.

    float data[16];

    // Default constructor: identity matrix
    constexpr Mat4()
        : data{
              1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f}
    {
    }

    // Identity matrix
    static constexpr Mat4 identity()
    {
        return Mat4();
    }

    // Matrix multiplication
    Mat4 operator*(const Mat4 &other) const
    {
        Mat4 result;

        for (int row = 0; row < 4; ++row)
        {
            for (int column = 0; column < 4; ++column)
            {
                result.data[column * 4 + row] =
                    data[0 * 4 + row] * other.data[column * 4 + 0] +
                    data[1 * 4 + row] * other.data[column * 4 + 1] +
                    data[2 * 4 + row] * other.data[column * 4 + 2] +
                    data[3 * 4 + row] * other.data[column * 4 + 3];
            }
        }

        return result;
    }

    // Transform a Vec3 as a POINT.
    //
    // Vec3(x,y,z) is treated as:
    // (x,y,z,1)
    //
    // Translation affects the result.
    Vec3 transformPoint(const Vec3 &v) const
    {
        float x =
            data[0] * v.x +
            data[4] * v.y +
            data[8] * v.z +
            data[12];

        float y =
            data[1] * v.x +
            data[5] * v.y +
            data[9] * v.z +
            data[13];

        float z =
            data[2] * v.x +
            data[6] * v.y +
            data[10] * v.z +
            data[14];

        float w =
            data[3] * v.x +
            data[7] * v.y +
            data[11] * v.z +
            data[15];

        // Perspective divide.
        if (w != 0.0f && w != 1.0f)
        {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vec3(x, y, z);
    }

    // Transform a Vec3 as a DIRECTION.
    //
    // Vec3(x,y,z) is treated as:
    // (x,y,z,0)
    //
    // Translation is ignored.
    Vec3 transformDirection(const Vec3 &v) const
    {
        return Vec3(
            data[0] * v.x +
                data[4] * v.y +
                data[8] * v.z,

            data[1] * v.x +
                data[5] * v.y +
                data[9] * v.z,

            data[2] * v.x +
                data[6] * v.y +
                data[10] * v.z);
    }

    // Translation matrix
    static Mat4 translation(const Vec3 &t)
    {
        Mat4 result;

        result.data[12] = t.x;
        result.data[13] = t.y;
        result.data[14] = t.z;

        return result;
    }

    // Scale matrix
    static Mat4 scale(const Vec3 &s)
    {
        Mat4 result;

        result.data[0] = s.x;
        result.data[5] = s.y;
        result.data[10] = s.z;

        return result;
    }

    // Rotation around X axis.
    //
    // Right-handed coordinate system.
    static Mat4 rotationX(float radians)
    {
        Mat4 result;

        float c = std::cos(radians);
        float s = std::sin(radians);

        result.data[5] = c;
        result.data[6] = s;
        result.data[9] = -s;
        result.data[10] = c;

        return result;
    }

    // Rotation around Y axis.
    //
    // Right-handed coordinate system.
    static Mat4 rotationY(float radians)
    {
        Mat4 result;

        float c = std::cos(radians);
        float s = std::sin(radians);

        result.data[0] = c;
        result.data[2] = -s;
        result.data[8] = s;
        result.data[10] = c;

        return result;
    }

    // Rotation around Z axis.
    //
    // Positive rotation is counter-clockwise when viewed
    // looking toward the origin along the +Z axis.
    //
    // Example:
    // rotationZ(pi / 2) * (1,0,0) -> (0,1,0)
    static Mat4 rotationZ(float radians)
    {
        Mat4 result;

        float c = std::cos(radians);
        float s = std::sin(radians);

        result.data[0] = c;
        result.data[1] = s;
        result.data[4] = -s;
        result.data[5] = c;

        return result;
    }

    // Transpose
    Mat4 transpose() const
    {
        Mat4 result;

        for (int row = 0; row < 4; ++row)
        {
            for (int column = 0; column < 4; ++column)
            {
                result.data[row * 4 + column] =
                    data[column * 4 + row];
            }
        }

        return result;
    }
};