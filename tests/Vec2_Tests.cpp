#include "catch2/catch_amalgamated.hpp"

#define COMPILE_VEC2_TESTS 0

#if COMPILE_VEC2_TESTS
    #include "mlm/vector/Vec2.h"
    #include "mlm_TestsCommon.h"
    #include <iostream>

TEMPLATE_TEST_CASE("MLM.Vec2.Test constructors and assignments", "[Vec2]", int, float, double)
{
    STATIC_REQUIRE(sizeof(TestVec2) == sizeof(TestType) * 2);

    TestVec2 defaultVec;
    REQUIRE((defaultVec.x == 0 && defaultVec.y == 0));

    TestVec2 vec1(42);
    REQUIRE((vec1.x == 42 && vec1.y == 42));

    TestVec2 vec2(4, 2);
    REQUIRE((vec2.x == 4 && vec2.y == 2));

    TestVec2 vecstr("4 2");
    REQUIRE((vecstr.x == 4 && vecstr.y == 2));

    if (!std::is_same_v<TestType, int>)
    {
        TestVec2 vecstrFP("4.2 5.1");
        REQUIRE((vecstrFP.x == (TestType)4.2 && vecstrFP.y == (TestType)5.1));
    }

    TestVec2 copy = vec1;
    REQUIRE((vec1.x == copy.x && vec1.y == copy.y));

    copy = vec2;
    REQUIRE((vec2.x == copy.x && vec2.y == copy.y));

    copy = "4 2";
    REQUIRE((4 == copy.x && 2 == copy.y));
}

TEMPLATE_TEST_CASE("MLM.Vec2.Operators", "[Vec2]", int, float, double)
{
    TestVec2 vec(13, 37);
    REQUIRE((vec[0] == 13 && vec[1] == 37));

    SECTION("Overflow/Underflow")
    {
        REQUIRE_THROWS_AS(vec[-1], mlm::RuntimeException);
        REQUIRE_THROWS_AS(vec[2], mlm::RuntimeException);
    }

    SECTION("Add")
    {
        vec = TestVec2(2, 1) + TestVec2(2, 1);
        REQUIRE((vec[0] == 4 && vec[1] == 2));

        vec += 42;
        REQUIRE((vec[0] == 46 && vec[1] == 44));

        vec += TestVec2(13, 37);
        REQUIRE((vec[0] == 59 && vec[1] == 81));
    }

    SECTION("Sub")
    {
        vec = TestVec2(6, 3) - TestVec2(2, 1);
        REQUIRE((vec[0] == 4 && vec[1] == 2));

        vec -= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 0));

        vec -= TestVec2(4, 2);
        REQUIRE((vec[0] == -2 && vec[1] == -2));
    }

    SECTION("Mul")
    {
        vec = TestVec2(2, 1) * 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2));

        vec = TestVec2(2, 1) * TestVec2(2, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 2));

        vec *= 2;
        REQUIRE((vec[0] == 8 && vec[1] == 4));

        vec *= TestVec2(2, 2);
        REQUIRE((vec[0] == 16 && vec[1] == 8));
    }

    SECTION("Div")
    {
        vec = TestVec2(8, 4) / 2;
        REQUIRE((vec[0] == 4 && vec[1] == 2));

        vec = TestVec2(0, 0);
        REQUIRE((vec[0] == 0 && vec[1] == 0));

        vec = TestVec2(16, 8) / TestVec2(4, 2);
        REQUIRE((vec[0] == 4 && vec[1] == 4));

        vec /= 2;
        REQUIRE((vec[0] == 2 && vec[1] == 2));

        vec /= TestVec2(2, 2);
        REQUIRE((vec[0] == 1 && vec[1] == 1));
    }

    SECTION("Std")
    {
        std::string str = "Vec: " + TestVec2(42, 42);
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 42 y: 42)");
        else
            REQUIRE(str == "Vec: (x: 42.000000 y: 42.000000)");

        // Capture cout output to check its output
        std::stringstream captureStream;
        std::streambuf* origCoutBuffer = std::cout.rdbuf(captureStream.rdbuf());

        std::cout << TestVec2(42, 42);

        // Give std::cout its original buffer back ASAP
        std::cout.rdbuf(origCoutBuffer);

        // Note: output stream policy appends a \n to messages
        std::string capturedText = captureStream.str();
        if constexpr (std::is_same_v<TestType, int>)
            REQUIRE(str == "Vec: (x: 42 y: 42)");
        else
            REQUIRE(str == "Vec: (x: 42.000000 y: 42.000000)");
    }
}

TEMPLATE_TEST_CASE("MLM.Vec2.Basic laws and properties", "[Vec2]", int, float, double)
{
    const TestVec2 a{4, 2}, b{2, 6}, c{4, 93};
    const TestVec2 apb = (a + b) + c;
    const TestVec2 bpc = a + (b + c);
    REQUIRE(apb == TestVec2{10, 101});
    REQUIRE(apb == bpc);

    REQUIRE((a + b) == (b + a));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((a * (TestType)2 * (TestType)3) == (TestType)2 * (a * (TestType)3));

    REQUIRE((TestType)6 * (a + b) == (TestType)6 * a + (TestType)6 * b);

    REQUIRE((TestType)6 * a == (TestType)2 * a + (TestType)4 * a);
}

TEMPLATE_TEST_CASE("MLM.Vec2.Dot", "[Vec2]", float, double)
{
    TestVec2 v1(1, 0);
    REQUIRE(v1.dot(v1) == 1);

    TestVec2 v2(0, 1);
    REQUIRE(v1.dot(v2) == 0);

    TestVec2 v3(-1, 0);
    REQUIRE(v1.dot(v3) == -1);

    TestVec2 v4(0, -1);
    REQUIRE(v1.dot(v4) == 0);

    TestVec2 v5(1, 1);
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

TEMPLATE_TEST_CASE("MLM.Vec2.Norm/Magnitude", "[Vec2]", float, double)
{
    TestVec2 v1(1, 0);
    TestType norm = v1.norm();
    REQUIRE((norm == v1.magnitude() && norm == 1));
    REQUIRE(v1.squareMagnitude() == 1);

    v1 = TestVec2(4, 0);
    TestType n = v1.norm();
    REQUIRE(n == 4);
    TestType m = v1.magnitude();
    REQUIRE(n == m);
    REQUIRE(v1.squareMagnitude() == 16);

    v1 = TestVec2(0, 0);
    n = v1.norm();
    REQUIRE(n == 0);
    REQUIRE(v1.squareMagnitude() == 0);

    v1 = TestVec2(4, 3);
    REQUIRE(v1.norm() == 5);

    v1 = TestVec2((TestType)23.4, (TestType)221.4);
    TestVec2 v2 = (TestType)2 * v1;
    REQUIRE(v2.norm() == (TestType)2 * v1.norm());

    v2 = v1.normalize();
    REQUIRE(v2 == v1 / v1.norm());
}

TEMPLATE_TEST_CASE("MLM.Vec2.Perpendicular", "[Vec2]", float, double)
{
    TestVec2 v1(1, 0);

    REQUIRE(v1.perpdot(v1) == 0);

    TestVec2 v2(0, 1);
    REQUIRE(v1.perpdot(v2) == 1);
}

TEMPLATE_TEST_CASE("MLM.Vec2.Project", "[Vec2]", float, double)
{
    TestVec2 v1(-2, 3);

    TestVec2 projLine(0, 1);
    TestVec2 proj = v1.project(projLine);
    REQUIRE(proj == TestVec2(0, 3));

    projLine = TestVec2(1, 0);
    proj = v1.project(projLine);
    REQUIRE(proj == TestVec2(-2, 0));
}

TEMPLATE_TEST_CASE("MLM.Vec2.Reject", "[Vec2]", float, double)
{
    TestVec2 v1(-2, 3);

    TestVec2 projLine = TestVec2(0, 1);
    TestVec2 rej = v1.reject(projLine);
    REQUIRE(rej == TestVec2(-2, 0));

    projLine = TestVec2(1, 0);
    rej = v1.reject(projLine);
    REQUIRE(rej == TestVec2(0, 3));
}

TEMPLATE_TEST_CASE("MLM.Vec2.Reflect", "[Vec2]", float, double)
{
    TestVec2 v1(-2, 3);

    TestVec2 projLine = TestVec2(0, 1);
    TestVec2 refl = v1.reflect(projLine); // projLine is this time used as the normal of the plane
    REQUIRE(refl == TestVec2(-2, -3));
}
#endif // COMPILE_VEC2_TESTS
