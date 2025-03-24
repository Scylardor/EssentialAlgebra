#include "catch2/catch_amalgamated.hpp"

#define COMPILE_VEC3_TESTS 0

#if COMPILE_VEC3_TESTS
    #include "mlm/vector/Vec3.h"
    #include "mlm_TestsCommon.h"
    #include <iostream>

TEMPLATE_TEST_CASE("MLM.Vec3.Test constructors and assignments", "[Vec3]", int, float, double)
{
    STATIC_REQUIRE(sizeof(TestVec3) == sizeof(TestType) * 3);

    TestVec3 defaultVec;
    REQUIRE((defaultVec.x == 0 && defaultVec.y == 0 && defaultVec.z == 0));

    TestVec3 vec1(42);
    REQUIRE((vec1.x == 42 && vec1.y == 42 && vec1.z == 42));

    TestVec3 vec2(4, 2);
    REQUIRE((vec2.x == 4 && vec2.y == 2 && vec2.z == 0));

    TestVec3 vec3(Vector<TestType, 2>(4, 2));
    REQUIRE((vec3.x == 4 && vec3.y == 2 && vec3.z == 0));

    TestVec3 vec4(4, 2, 0x42);
    REQUIRE((vec4.x == 4 && vec4.y == 2 && vec4.z == 0x42));

    TestVec3 vec5(Vector<TestType, 2>(4, 2), 0x42);
    REQUIRE((vec5.x == 4 && vec5.y == 2 && vec5.z == 0x42));

    TestVec3 vecstr("4 2 1");
    REQUIRE((vecstr.x == 4 && vecstr.y == 2 && vecstr.z == 1));

    if (!std::is_same_v<TestType, int>)
    {
        TestVec3 vecstrFP("4.2 5.1 3.9");
        REQUIRE((vecstrFP.x == (TestType)4.2 && vecstrFP.y == (TestType)5.1 &&
                 vecstrFP.z == (TestType)3.9));
    }

    TestVec3 copy = vec1;
    REQUIRE((vec1.x == copy.x && vec1.y == copy.y && vec1.z == copy.z));

    copy = vec2;
    REQUIRE((vec2.x == copy.x && vec2.y == copy.y && vec2.z == copy.z));

    copy = "4 2 1";
    REQUIRE((4 == copy.x && 2 == copy.y && 1 == copy.z));
}

TEMPLATE_TEST_CASE("MLM.Vec3.Operators", "[Vec3]", int, float, double)
{
    TestVec3 vec(4, 2, 0x42);
    REQUIRE((vec.x == 4 && vec.y == 2 && vec.z == 0x42));

    SECTION("Overflow/Underflow")
    {
        REQUIRE_THROWS_AS(vec[-1], mlm::RuntimeException);
        REQUIRE_THROWS_AS(vec[3], mlm::RuntimeException);
    }

    SECTION("Add")
    {
        vec = TestVec3(2, 1, 4) + TestVec3(2, 1, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 6));

        vec += 42;
        REQUIRE((vec[0] == 46 && vec[1] == 44 && vec[2] == 48));

        vec += TestVec3(13, 37, 42);
        REQUIRE((vec[0] == 59 && vec[1] == 81 && vec[2] == 90));
    }

    SECTION("Sub")
    {
        vec = TestVec3(6, 3, 4) - TestVec3(2, 1, 3);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 1));

        vec -= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 0 && vec[2] == -1));

        vec -= TestVec3(4, 2, 3);
        REQUIRE((vec[0] == -2 && vec[1] == -2 && vec[2] == -4));
    }

    SECTION("Mul")
    {
        vec = TestVec3(2, 1, 3) * 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 6));

        vec = TestVec3(2, 1, 3) * TestVec3(2, 2, 7);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 21));

        vec *= 2;
        REQUIRE((vec[0] == 8 && vec[1] == 4 && vec[2] == 42));

        vec *= TestVec3(2, 2, 3);
        REQUIRE((vec[0] == 16 && vec[1] == 8 && vec[2] == 126));
    }

    SECTION("Div")
    {
        vec = TestVec3(8, 4, 6) / 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 3));

        vec = TestVec3(16, 8, 12) / TestVec3(4, 2, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 4 && vec[2] == 6));

        vec /= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 2 && vec[2] == 3));

        vec /= TestVec3(2, 2, 1);
        REQUIRE((vec[0] == 1 && vec[1] == 1 && vec[2] == 3));
    }

    SECTION("Std")
    {
        std::string str = "Vec: " + TestVec3(4, 2, 0x42);
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 4 y: 2 z: 66)");
        else
            REQUIRE(str == "Vec: (x: 4.000000 y: 2.000000 z: 66.000000)");

        // Capture cout output to check its output
        std::stringstream captureStream;
        std::streambuf* origCoutBuffer = std::cout.rdbuf(captureStream.rdbuf());

        std::cout << TestVec3(4, 2, 0x42);

        // Give std::cout its original buffer back ASAP
        std::cout.rdbuf(origCoutBuffer);

        // Note: output stream policy appends a \n to messages
        std::string capturedText = captureStream.str();
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 4 y: 2 z: 66)");
        else
            REQUIRE(str == "Vec: (x: 4.000000 y: 2.000000 z: 66.000000)");
    }
}

TEMPLATE_TEST_CASE("MLM.Vec3.Basic laws and properties", "[Vec3]", int, float, double)
{
    const TestVec3 a{4, 2, 1}, b{2, 6, 8}, c{4, 93, -99};
    const TestVec3 apb = (a + b) + c;
    const TestVec3 bpc = a + (b + c);
    REQUIRE(apb == TestVec3{10, 101, -90});
    REQUIRE(apb == bpc);

    REQUIRE((a + b) == (b + a));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((TestType)6 * (a + b) == (TestType)6 * a + (TestType)6 * b);

    REQUIRE((TestType)6 * a == (TestType)2 * a + (TestType)4 * a);
}

TEMPLATE_TEST_CASE("MLM.Vec3.Dot", "[Vec3]", float, double)
{
    TestVec3 v1(1, 0);
    REQUIRE(v1.dot(v1) == 1);

    TestVec3 v2(0, 1);
    REQUIRE(v1.dot(v2) == 0);

    TestVec3 v3(-1, 0);
    REQUIRE(v1.dot(v3) == -1);

    TestVec3 v4(0, -1);
    REQUIRE(v1.dot(v4) == 0);

    // z != 0
    TestVec3 v5(2, 2, 3);
    TestVec3 v6(1, 2, 5);
    REQUIRE(v5.dot(v6) == 21);

    v5 = TestVec3(1, 1);
    v5 = v5.normalize();
    TestType dp = v1.dot(v5);
    REQUIRE(mlm::almostEquals(dp, (TestType)0.707106769));

    REQUIRE(mlm::almostEquals(dp, v5.dot(v1))); // a.b = b.a

    REQUIRE(mlm::almostEquals(v1.dot(v2 + v5), v1.dot(v2) + v1.dot(v5))); // a.(b+c) = a.b + a.c

    REQUIRE(mlm::almostEquals(((TestType)3 * v1).dot(v5),
                              v1.dot((TestType)3 * v5))); // (ta).b = a.(tb) = t(a.b)
    REQUIRE(mlm::almostEquals(((TestType)3 * v1).dot(v5), (TestType)3 * v5.dot(v1)));

    // a.b = ||a|| ||b|| cos T
    Degs<TestType> degs(45);
    Rads<TestType> cos45 = mlm::cos(Rads<TestType>(degs));
    REQUIRE(mlm::almostEquals(dp, v1.norm() * v5.norm() * cos45()));
}

TEMPLATE_TEST_CASE("MLM.Vec3.Cross", "[Vec3]", float, double)
{
    TestVec3 x{1, 0, 0};
    TestVec3 y{0, 1, 0};
    TestVec3 z{0, 0, 1};
    REQUIRE(x.cross(y) == z);
    REQUIRE(y.cross(x) == -z);

    // ||a x b|| = ||a|| ||b|| sin T
    x = TestVec3(3, 0, 0);
    y = TestVec3(0, 5, 0);
    z = x.cross(y);
    TestType m = z.magnitude();
    REQUIRE(m == 3 * 5 * 1); // right angle = sin 1

    // a x b = -b x a
    REQUIRE(x.cross(y) == -y.cross(x));

    // a x (b + c) = a x b + a x c
    REQUIRE(x.cross(y + z) == x.cross(y) + x.cross(z));

    // (ta) x b = a x (tb) = t (a x b)
    bool equal = (x * 5).cross(y) == x.cross(y * 5) && (x * 5).cross(y) == x.cross(y) * 5;
    REQUIRE(equal);

    // Lagrange identity: ||(a x b)||^2 = ||a||^2 ||b||^2 - (a . b)^2
    x = TestVec3(12, 3, -3);
    y = TestVec3(-12, 1, 8);
    z = x.cross(y);
    TestType dp = x.dot(y);
    TestType x2 = x.magnitude() * x.magnitude();
    TestType y2 = y.magnitude() * y.magnitude();
    TestType z2 = z.magnitude() * z.magnitude();
    REQUIRE(mlm::almostEquals(z2, x2 * y2 - dp * dp));

    // Finding the triangle area using the cross product, with x, y, and z the vertices.
    x = TestVec3(0, 0, 0);
    y = TestVec3(18, 0, 0);
    z = TestVec3(18, 22, 0);
    // It's a right triangle, so we can use the property: area = (base*height)/2
    TestType area = 18 * 22 / 2;

    TestVec3 p1p0 = y - x;
    TestVec3 p2p0 = z - x;
    TestType crossArea = (TestType)0.5 * p1p0.cross(p2p0).magnitude();
    REQUIRE(area == crossArea);

    // Testing with zero
    REQUIRE(z.cross(TestVec3(0)) == TestVec3(0));
    REQUIRE(TestVec3(0).cross(z) == TestVec3(0));
    REQUIRE(z.cross(z) == TestVec3(0));
}

TEMPLATE_TEST_CASE("MLM.Vec3.Norm/Magnitude", "[Vec3]", float, double)
{
    TestVec3 v1(0, 0, 1);
    TestType norm = v1.norm();
    REQUIRE((norm == v1.magnitude() && norm == 1));
    REQUIRE(v1.squareMagnitude() == 1);

    v1 = TestVec3(0, 0, 4);
    TestType n = v1.norm();
    REQUIRE(n == 4);
    TestType m = v1.magnitude();
    REQUIRE(n == m);
    REQUIRE(v1.squareMagnitude() == 16);

    v1 = TestVec3(0);
    n = v1.norm();
    REQUIRE(n == 0);
    REQUIRE(v1.squareMagnitude() == 0);

    v1 = TestVec3(4, 3);
    REQUIRE(v1.norm() == 5);

    v1 = TestVec3((TestType)23.4, (TestType)221.4);
    TestVec3 v2 = (TestType)2 * v1;
    REQUIRE(v2.norm() == (TestType)2 * v1.norm());

    v2 = v1.normalize();
    REQUIRE(v2 == v1 / v1.norm());
}

TEMPLATE_TEST_CASE("MLM.Vec3.Project", "[Vec3]", float, double)
{
    TestVec3 v1(-2, 3);

    TestVec3 projLine(0, 1);
    TestVec3 proj = v1.project(projLine);
    REQUIRE(proj == TestVec3(0, 3));

    projLine = TestVec3(1, 0);
    proj = v1.project(projLine);
    REQUIRE(proj == TestVec3(-2, 0));
}

TEMPLATE_TEST_CASE("MLM.Vec3.Reject", "[Vec3]", float, double)
{
    TestVec3 v1(-2, 3);

    TestVec3 projLine = TestVec3(0, 1);
    TestVec3 rej = v1.reject(projLine);
    REQUIRE(rej == TestVec3(-2, 0));

    projLine = TestVec3(1, 0);
    rej = v1.reject(projLine);
    REQUIRE(rej == TestVec3(0, 3));
}

TEMPLATE_TEST_CASE("MLM.Vec3.Reflect", "[Vec3]", float, double)
{
    TestVec3 v1(-2, 3);

    TestVec3 projLine = TestVec3(0, 1);
    TestVec3 refl = v1.reflect(projLine); // projLine is this time used as the normal of the plane
    REQUIRE(refl == TestVec3(-2, -3));
}

TEMPLATE_TEST_CASE("MLM.Vec3.scalarTripleProduct", "[Vec3]", float, double)
{
    TestVec3 x = TestVec3(12, 3, -3);
    TestVec3 y = TestVec3(-12, 1, 8);
    TestVec3 z = TestVec3(9, -5, 2);
    REQUIRE(x.cross(y).dot(z) == y.cross(z).dot(x));
    REQUIRE(x.cross(y).dot(z) == z.cross(x).dot(y));
}

TEMPLATE_TEST_CASE("MLM.Vec3.vectorTripleProduct", "[Vec3]", float, double)
{
    TestVec3 x = TestVec3(12, 3, -3);
    TestVec3 y = TestVec3(-12, 1, 8);
    TestVec3 z = TestVec3(9, -5, 2);
    TestVec3 yz = y.cross(z);
    TestVec3 xyz = x.cross(yz);
    REQUIRE(xyz == y * x.dot(z) - z * x.dot(y));
}
#endif // COMPILE_VEC3_TESTS
