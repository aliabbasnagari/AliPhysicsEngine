#include <iostream>
#include "math/Vec2.h"
#include "math/Vec3.h"

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