#include <iostream>

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Mat4.h"
#include "math/Quaternion.h"

using namespace std;

void runTest11()
{
     Vec2 a(3.0f, 4.0f);

     // Acceptance 1:
     // Vec2(3,4).length() == 5
     bool test1 = Vec2(3.0f, 4.0f).length() == 5.0f;

     // Acceptance 2:
     // Vec2(1,0).dot(Vec2(0,1)) == 0
     bool test2 = Vec2(1.0f, 0.0f).dot(Vec2(0.0f, 1.0f)) == 0.0f;

     // Acceptance 3:
     // Vec2(1,0).perp() == Vec2(0,1)
     bool test3 = Vec2(1.0f, 0.0f).perp() == Vec2(0.0f, 1.0f);

     // Acceptance 4:
     // a.normalized().length() is approximately 1
     float normalizedLength = a.normalized().length();
     bool test4 = normalizedLength >= 0.999f && normalizedLength <= 1.001f;

     cout << "TEST 1: Vec2(3,4).length() == 5 ........ "
          << (test1 ? "PASS" : "FAIL") << endl;

     cout << "TEST 2: Vec2(1,0).dot(Vec2(0,1)) == 0 ........ "
          << (test2 ? "PASS" : "FAIL") << endl;

     cout << "TEST 3: Vec2(1,0).perp() == Vec2(0,1) ........ "
          << (test3 ? "PASS" : "FAIL") << endl;

     cout << "TEST 4: a.normalized().length() ~= 1 ........ "
          << (test4 ? "PASS" : "FAIL")
          << " (actual: " << normalizedLength << ")" << endl;
}

void runTest12()
{
     Vec3 a(1.0f, 2.0f, 3.0f);
     Vec3 b(4.0f, 5.0f, 6.0f);

     // Acceptance 1:
     // Vec3(1,0,0).cross(Vec3(0,1,0)) == Vec3(0,0,1)
     bool test1 = Vec3(1.0f, 0.0f, 0.0f).cross(Vec3(0.0f, 1.0f, 0.0f)) == Vec3(0.0f, 0.0f, 1.0f);

     // Acceptance 2:
     // Vec3(1,0,0).dot(Vec3(1,0,0)) == 1
     bool test2 = Vec3(1.0f, 0.0f, 0.0f).dot(Vec3(1.0f, 0.0f, 0.0f)) == 1.0f;

     // Acceptance 3:
     // Vec3(0,3,4).length() == 5
     bool test3 = Vec3(0.0f, 3.0f, 4.0f).length() == 5.0f;

     // Acceptance 4:
     // Cross product is anti-commutative:
     // a.cross(b) == -(b.cross(a))
     bool test4 = a.cross(b) == -(b.cross(a));

     cout << "TEST 1: Vec3(1,0,0).cross(Vec3(0,1,0)) == Vec3(0,0,1) ........ "
          << (test1 ? "PASS" : "FAIL") << endl;

     cout << "TEST 2: Vec3(1,0,0).dot(Vec3(1,0,0)) == 1 ........ "
          << (test2 ? "PASS" : "FAIL") << endl;

     cout << "TEST 3: Vec3(0,3,4).length() == 5 ........ "
          << (test3 ? "PASS" : "FAIL") << endl;

     cout << "TEST 4: a.cross(b) == -(b.cross(a)) ........ "
          << (test4 ? "PASS" : "FAIL") << endl;
}

void runTest13()
{
     // Acceptance 1:
     // Mat4::identity() * v == v
     Vec3 v(1.0f, 2.0f, 3.0f);

     bool test1 = Mat4::identity().transformPoint(v) == v;

     // Acceptance 2:
     // Mat4::translation({1,2,3}) transforms (0,0,0) to (1,2,3)
     bool test2 = Mat4::translation(Vec3(1.0f, 2.0f, 3.0f)).transformPoint(Vec3(0.0f, 0.0f, 0.0f)) == Vec3(1.0f, 2.0f, 3.0f);

     // Acceptance 3:
     // translation(T) * rotation(R)
     // matches doing rotation then translation manually
     float pi = 3.14159265f;
     Vec3 point(1.0f, 0.0f, 0.0f);

     Mat4 rotation = Mat4::rotationZ(pi / 2.0f);
     Mat4 translation = Mat4::translation(Vec3(10.0f, 20.0f, 0.0f));

     Vec3 composed = (translation * rotation).transformPoint(point);

     Vec3 manuallyRotated = rotation.transformPoint(point);

     Vec3 manuallyTranslated(manuallyRotated.x + 10.0f, manuallyRotated.y + 20.0f, manuallyRotated.z);

     bool test3 = composed == manuallyTranslated;

     // Acceptance 4:
     // rotationZ(pi/2) transforms (1,0,0) to approximately (0,1,0)
     Vec3 rotated = Mat4::rotationZ(pi / 2.0f).transformPoint(Vec3(1.0f, 0.0f, 0.0f));

     Vec3 expected(0.0f, 1.0f, 0.0f);

     bool test4 = std::fabs(rotated.x - expected.x) < 0.001f && std::fabs(rotated.y - expected.y) < 0.001f && std::fabs(rotated.z - expected.z) < 0.001f;

     cout << "TEST 1: Mat4::identity() * v == v ........ "
          << (test1 ? "PASS" : "FAIL") << endl;

     cout << "TEST 2: Translation (1,2,3) ................. "
          << (test2 ? "PASS" : "FAIL") << endl;

     cout << "TEST 3: Translation * Rotation .............. "
          << (test3 ? "PASS" : "FAIL") << endl;

     cout << "TEST 4: rotationZ(pi/2) -> (0,1,0) ........ "
          << (test4 ? "PASS" : "FAIL")
          << " (actual: "
          << rotated.x << ", "
          << rotated.y << ", "
          << rotated.z << ")" << endl;
}

void runTest14()
{
     float pi = 3.14159265f;

     // Acceptance 1:
     // Quaternion::identity().rotate(v) == v
     Vec3 v(1.0f, 2.0f, 3.0f);

     bool test1 = Quaternion::identity().rotate(v) == v;

     // Acceptance 2:
     // Quaternion::fromAxisAngle({0,0,1}, pi/2)
     // rotates (1,0,0) to approximately (0,1,0)
     Vec3 rotated = Quaternion::fromAxisAngle(Vec3(0.0f, 0.0f, 1.0f), pi / 2.0f).rotate(Vec3(1.0f, 0.0f, 0.0f));

     Vec3 expected(0.0f, 1.0f, 0.0f);

     bool test2 = std::fabs(rotated.x - expected.x) < 0.001f && std::fabs(rotated.y - expected.y) < 0.001f && std::fabs(rotated.z - expected.z) < 0.001f;

     // Acceptance 3:
     // Multiplying two quaternions and rotating a vector
     // matches applying the two rotations sequentially.
     Quaternion q1 = Quaternion::fromAxisAngle(Vec3(0.0f, 0.0f, 1.0f), pi / 2.0f);

     Quaternion q2 = Quaternion::fromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), pi / 2.0f);

     Vec3 original(1.0f, 0.0f, 0.0f);

     Vec3 sequential = q1.rotate(q2.rotate(original));

     Vec3 composed = (q1 * q2).rotate(original);

     bool test3 = std::fabs(sequential.x - composed.x) < 0.001f && std::fabs(sequential.y - composed.y) < 0.001f && std::fabs(sequential.z - composed.z) < 0.001f;

     // Acceptance 4:
     // q.toMat4() transforming a point gives the same result
     // as q.rotate() on that point.
     Quaternion q = Quaternion::fromAxisAngle(Vec3(0.0f, 0.0f, 1.0f), pi / 2.0f);

     Vec3 quaternionResult = q.rotate(original);
     Vec3 matrixResult = q.toMat4().transformPoint(original);

     bool test4 = std::fabs(quaternionResult.x - matrixResult.x) < 0.001f && std::fabs(quaternionResult.y - matrixResult.y) < 0.001f && std::fabs(quaternionResult.z - matrixResult.z) < 0.001f;

     cout << "TEST 1: identity().rotate(v) == v ........ "
          << (test1 ? "PASS" : "FAIL") << endl;

     cout << "TEST 2: Z rotation 90 degrees ............ "
          << (test2 ? "PASS" : "FAIL")
          << " (actual: "
          << rotated.x << ", "
          << rotated.y << ", "
          << rotated.z << ")" << endl;

     cout << "TEST 3: Quaternion composition ........... "
          << (test3 ? "PASS" : "FAIL") << endl;

     cout << "TEST 4: Quaternion vs Mat4 rotation ...... "
          << (test4 ? "PASS" : "FAIL") << endl;
}