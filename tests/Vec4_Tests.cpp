#include "catch2/catch_amalgamated.hpp"

#define COMPILE_VEC4_TESTS 0

#if COMPILE_VEC4_TESTS
    #include "mlm/vector/Vec4.h"
    #include "mlm_TestsCommon.h"
    #include <iostream>

TEMPLATE_TEST_CASE("MLM.Vec4.Test constructors and assignments", "[Vec4]", int, float, double)
{
    STATIC_REQUIRE(sizeof(TestVec4) == sizeof(TestType) * 4);

    TestVec4 defaultVec;
    REQUIRE((defaultVec.x == 0 && defaultVec.y == 0 && defaultVec.z == 0 && defaultVec.w == 0));

    TestVec4 vec1(42);
    REQUIRE((vec1.x == 42 && vec1.y == 42 && vec1.z == 42 && vec1.w == 42));

    TestVec4 vec2(4, 2);
    REQUIRE((vec2.x == 4 && vec2.y == 2 && vec2.z == 0 && vec2.w == 0));

    TestVec4 vec3(Vector<TestType, 2>(4, 2));
    REQUIRE((vec3.x == 4 && vec3.y == 2 && vec3.z == 0 && vec3.w == 0));

    TestVec4 vec4(4, 2, 0x42);
    REQUIRE((vec4.x == 4 && vec4.y == 2 && vec4.z == 0x42 && vec4.w == 0));

    TestVec4 vec5(4, 2, 0x42, 1);
    REQUIRE((vec5.x == 4 && vec5.y == 2 && vec5.z == 0x42 && vec5.w == 1));

    TestVec4 vec6(Vector<TestType, 3>(4, 2, 0x42));
    REQUIRE((vec6.x == 4 && vec6.y == 2 && vec6.z == 0x42 && vec6.w == 0));

    TestVec4 vecstr("4 2 1 42");
    REQUIRE((vecstr.x == 4 && vecstr.y == 2 && vecstr.z == 1 && vecstr.w == 42));

    if (!std::is_same_v<TestType, int>)
    {
        TestVec4 vecstrFP("4.2 5.1 3.9 4.4");
        REQUIRE((vecstrFP.x == (TestType)4.2 && vecstrFP.y == (TestType)5.1 &&
                 vecstrFP.z == (TestType)3.9 && vecstrFP.w == (TestType)4.4));
    }

    TestVec4 copy = vec1;
    REQUIRE((vec1.x == copy.x && vec1.y == copy.y && vec1.z == copy.z && vec1.w == copy.w));

    copy = vec2;
    REQUIRE((vec2.x == copy.x && vec2.y == copy.y && vec2.z == copy.z && vec2.w == copy.w));

    copy = "4 2 1 5";
    REQUIRE((4 == copy.x && 2 == copy.y && 1 == copy.z && 5 == copy.w));
}

TEMPLATE_TEST_CASE("MLM.Vec4.Operators", "[Vec4]", int, float, double)
{
    TestVec4 vec(4, 2, 0x42, 1);
    REQUIRE((vec.x == 4 && vec.y == 2 && vec.z == 0x42 && vec.w == 1));

    SECTION("Overflow/Underflow")
    {
        REQUIRE_THROWS_AS(vec[-1], mlm::RuntimeException);
        REQUIRE_THROWS_AS(vec[4], mlm::RuntimeException);
    }

    SECTION("Add")
    {
        vec = TestVec4(2, 1, 4) + TestVec4(2, 1, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 6 && vec[3] == 0));

        vec += 42;
        REQUIRE((vec[0] == 46 && vec[1] == 44 && vec[2] == 48 && vec[3] == 42));

        vec += TestVec4(13, 37, 42);
        REQUIRE((vec[0] == 59 && vec[1] == 81 && vec[2] == 90 && vec[3] == 42));
    }

    SECTION("Sub")
    {
        vec = TestVec4(6, 3, 4) - TestVec4(2, 1, 3);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 1 && vec[3] == 0));

        vec -= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 0 && vec[2] == -1 && vec[3] == -2));

        vec -= TestVec4(4, 2, 3);
        REQUIRE((vec[0] == -2 && vec[1] == -2 && vec[2] == -4 && vec[3] == -2));
    }

    SECTION("Mul")
    {
        vec = TestVec4(2, 1, 3) * 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 6 && vec[3] == 0));

        vec = TestVec4(2, 1, 3, 1) * TestVec4(2, 2, 7, 42);
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 21 && vec[3] == 42));

        vec *= 2;
        REQUIRE((vec[0] == 8 && vec[1] == 4 && vec[2] == 42 && vec[3] == 84));

        vec *= TestVec4(2, 2, 3);
        REQUIRE((vec[0] == 16 && vec[1] == 8 && vec[2] == 126 && vec[3] == 0));
    }

    SECTION("Div")
    {
        vec = TestVec4(8, 4, 6, 2) / 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2 && vec[2] == 3 && vec[3] == 1));

        vec = TestVec4(16, 8, 12, 4) / TestVec4(4, 2, 2, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 4 && vec[2] == 6 && vec[3] == 2));

        vec /= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 2 && vec[2] == 3 && vec[3] == 1));

        vec /= TestVec4(2, 2, 1, 1);
        REQUIRE((vec[0] == 1 && vec[1] == 1 && vec[2] == 3 && vec[3] == 1));
    }

    SECTION("Std")
    {
        std::string str = "Vec: " + TestVec4(4, 2, 0x42, 0x2a);
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 4 y: 2 z: 66 w: 42)");
        else
            REQUIRE(str == "Vec: (x: 4.000000 y: 2.000000 z: 66.000000 w: 42.000000)");

        // Capture cout output to check its output
        std::stringstream captureStream;
        std::streambuf* origCoutBuffer = std::cout.rdbuf(captureStream.rdbuf());

        std::cout << TestVec4(4, 2, 0x42, 0x2a);

        // Give std::cout its original buffer back ASAP
        std::cout.rdbuf(origCoutBuffer);

        // Note: output stream policy appends a \n to messages
        std::string capturedText = captureStream.str();
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 4 y: 2 z: 66 w: 42)");
        else
            REQUIRE(str == "Vec: (x: 4.000000 y: 2.000000 z: 66.000000 w: 42.000000)");
    }
}

TEMPLATE_TEST_CASE("MLM.Vec4.Basic laws and properties", "[Vec4]", int, float, double)
{
    const TestVec4 a{4, 2, 1}, b{2, 6, 8}, c{4, 93, -99};
    const TestVec4 apb = (a + b) + c;
    const TestVec4 bpc = a + (b + c);
    REQUIRE(apb == TestVec4{10, 101, -90});
    REQUIRE(apb == bpc);

    REQUIRE((a + b) == (b + a));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((TestType)6 * (a + b) == (TestType)6 * a + (TestType)6 * b);

    REQUIRE((TestType)6 * a == (TestType)2 * a + (TestType)4 * a);
}

TEMPLATE_TEST_CASE("MLM.Vec4.Dot", "[Vec4]", float, double)
{
    TestVec4 v1(1, 0);
    REQUIRE(v1.dot(v1) == 1);

    TestVec4 v2(0, 1);
    REQUIRE(v1.dot(v2) == 0);

    TestVec4 v3(-1, 0);
    REQUIRE(v1.dot(v3) == -1);

    TestVec4 v4(0, -1);
    REQUIRE(v1.dot(v4) == 0);

    // z != 0
    TestVec4 v5(2, 2, 3);
    TestVec4 v6(1, 2, 5);
    REQUIRE(v5.dot(v6) == 21);

    v5 = TestVec4(1, 1);
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

TEMPLATE_TEST_CASE("MLM.Vec4.Cross", "[Vec4]", float, double)
{
    TestVec4 x{1, 0, 0};
    TestVec4 y{0, 1, 0};
    TestVec4 z{0, 0, 1};
    REQUIRE(x.cross(y) == z);
    REQUIRE(y.cross(x) == -z);

    // ||a x b|| = ||a|| ||b|| sin T
    x = TestVec4(3, 0, 0);
    y = TestVec4(0, 5, 0);
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
    x = TestVec4(12, 3, -3);
    y = TestVec4(-12, 1, 8);
    z = x.cross(y);
    TestType dp = x.dot(y);
    TestType x2 = x.magnitude() * x.magnitude();
    TestType y2 = y.magnitude() * y.magnitude();
    TestType z2 = z.magnitude() * z.magnitude();
    REQUIRE(mlm::almostEquals(z2, x2 * y2 - dp * dp));

    // Finding the triangle area using the cross product,
    // with x, y, and z the vertices of a triangle.
    x = TestVec4(0, 0, 0);
    y = TestVec4(18, 0, 0);
    z = TestVec4(18, 22, 0);
    // It's a right triangle, so we can use the property: area = (base*height)/2
    TestType area = 18 * 22 / 2;

    TestVec4 p1p0 = y - x;
    TestVec4 p2p0 = z - x;
    TestType crossArea = (TestType)0.5 * p1p0.cross(p2p0).magnitude();
    REQUIRE(area == crossArea);

    // Testing with zero
    REQUIRE(z.cross(TestVec4(0)) == TestVec4(0));
    REQUIRE(TestVec4(0).cross(z) == TestVec4(0));
    REQUIRE(z.cross(z) == TestVec4(0));
}

TEMPLATE_TEST_CASE("MLM.Vec4.Norm/Magnitude", "[Vec4]", float, double)
{
    TestVec4 v1(0, 0, 1);
    TestType norm = v1.norm();
    REQUIRE((norm == v1.magnitude() && norm == 1));
    REQUIRE(v1.squareMagnitude() == 1);

    v1 = TestVec4(0, 0, 4);
    TestType n = v1.norm();
    REQUIRE(n == 4);
    TestType m = v1.magnitude();
    REQUIRE(n == m);
    REQUIRE(v1.squareMagnitude() == 16);

    v1 = TestVec4(0);
    n = v1.norm();
    REQUIRE(n == 0);
    REQUIRE(v1.squareMagnitude() == 0);

    v1 = TestVec4(4, 3);
    REQUIRE(v1.norm() == 5);

    v1 = TestVec4((TestType)23.4, (TestType)221.4);
    TestVec4 v2 = (TestType)2 * v1;
    REQUIRE(v2.norm() == (TestType)2 * v1.norm());

    v2 = v1.normalize();
    REQUIRE(v2 == v1 / v1.norm());
}

TEMPLATE_TEST_CASE("MLM.Vec4.Project", "[Vec4]", float, double)
{
    TestVec4 v1(-2, 3);

    TestVec4 projLine(0, 1);
    TestVec4 proj = v1.project(projLine);
    REQUIRE(proj == TestVec4(0, 3));

    projLine = TestVec4(1, 0);
    proj = v1.project(projLine);
    REQUIRE(proj == TestVec4(-2, 0));
}

TEMPLATE_TEST_CASE("MLM.Vec4.Reject", "[Vec4]", float, double)
{
    TestVec4 v1(-2, 3);

    TestVec4 projLine = TestVec4(0, 1);
    TestVec4 rej = v1.reject(projLine);
    REQUIRE(rej == TestVec4(-2, 0));

    projLine = TestVec4(1, 0);
    rej = v1.reject(projLine);
    REQUIRE(rej == TestVec4(0, 3));
}

TEMPLATE_TEST_CASE("MLM.Vec4.Reflect", "[Vec4]", float, double)
{
    TestVec4 v1(-2, 3);

    TestVec4 projLine = TestVec4(0, 1);
    TestVec4 refl = v1.reflect(projLine); // projLine is this time used as the normal of the plane
    REQUIRE(refl == TestVec4(-2, -3));
}
#endif // COMPILE_VEC4_TESTS
