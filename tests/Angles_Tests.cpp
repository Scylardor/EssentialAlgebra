#include "catch2/catch_amalgamated.hpp"

#define COMPILE_ANGLES_TESTS 0

#if COMPILE_ANGLES_TESTS
    #include "mlm/angles/DegreesRadians.h"
    #include "mlm_TestsCommon.h"

TEMPLATE_TEST_CASE("MLM.Angles.radToDeg and degToRad", "[Angles]", float, double)
{
    TestType rads = PI / 4;
    TestType degs = mlm::radToDeg(rads);
    REQUIRE(degs == 45);

    TestType rads2 = mlm::degToRad(degs);
    REQUIRE(rads == rads2);
}

TEMPLATE_TEST_CASE("MLM.Angles.Degs and Rads", "[Angles]", float, double)
{
    STATIC_REQUIRE(sizeof(Degs<TestType>) == sizeof(TestType));
    STATIC_REQUIRE(sizeof(Rads<TestType>) == sizeof(TestType));

    Rads<TestType> rads(PI / 4);
    REQUIRE(rads() == PI / 4);

    Degs<TestType> degs(45); // PI / 6
    rads = degs;
    REQUIRE(rads() == degToRad(degs()));

    degs = rads;
    REQUIRE(degs() == radToDeg(rads()));
}

TEMPLATE_TEST_CASE("MLM.Angles.Operators", "[Angles]", float, double)
{
    Rads<TestType> rads(PI / 6);

    rads *= 2;
    REQUIRE(rads == Rads<TestType>(PI / 3));

    rads = rads * (TestType)0.5;
    REQUIRE(rads == Rads<TestType>(PI / 6));

    rads += PI / 6;
    REQUIRE(rads == Rads<TestType>(PI / 3));

    rads = rads + PI / -6;
    REQUIRE(rads == Rads<TestType>(PI / 6));

    rads = -rads;
    REQUIRE(rads == Rads<TestType>(-PI / 6));

    rads = Rads<TestType>(PI / 2);
    REQUIRE(rads() == PI / 2);

    Degs<TestType> degs(10);

    degs *= 2;
    REQUIRE(degs == Degs<TestType>(20));

    degs = degs * (TestType)0.5;
    REQUIRE(degs == Degs<TestType>(10));

    degs += 10;
    REQUIRE(degs == Degs<TestType>(20));

    degs = degs + -10;
    REQUIRE(degs == Degs<TestType>(10));

    degs = -degs;
    REQUIRE(degs == Degs<TestType>(-10));
}

TEST_CASE("MLM.Angles.Literals", "[Angles]")
{
    Degs_f degf45 = 45_degf;
    REQUIRE(degf45 == Degs_f(45));

    degf45 = 45.0_degf;
    REQUIRE(degf45 == Degs_f(45));

    Rads_f rad = 1_radf;
    REQUIRE(rad == Rads_f(1));

    rad = 1.0_radf;
    REQUIRE(rad == Rads_f(1));
}
#endif // #if COMPILE_ANGLES_TESTS