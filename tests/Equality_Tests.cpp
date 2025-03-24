#include "catch2/catch_amalgamated.hpp"

#define COMPILE_EQUALITY_TESTS 0

#if COMPILE_EQUALITY_TESTS
    #include "mlm/functions/Equality.h"

using namespace mlm;

template <typename T>
T positiveInfinity()
{
    return std::numeric_limits<T>::infinity();
}

template <typename T>
T negativeInfinity()
{
    return -std::numeric_limits<T>::infinity();
}

template <typename T>
T NaN()
{
    static_assert(std::numeric_limits<T>::has_quiet_NaN,
                  "Used type doesn't implement quiet NaNs, so it cannot give you one");
    return std::numeric_limits<T>::quiet_NaN();
}

template <typename T>
T highest()
{
    return std::numeric_limits<T>::max();
}

template <typename T>
T lowest()
{
    return std::numeric_limits<T>::lowest();
}

template <typename T>
T minPositiveNormalized()
{
    return std::numeric_limits<T>::min();
}

TEMPLATE_TEST_CASE("MLM.Equality.Test the same value", "[almostEquals]", int, float, double)
{
    TestType test = (TestType)42;
    REQUIRE(almostEquals<TestType>(test, test));
}

TEMPLATE_TEST_CASE("MLM.Equality.Completely different values", "[almostEquals]", int, float, double)
{
    TestType test1 = (TestType)42;
    TestType test2 = (TestType)0;
    REQUIRE(!almostEquals<TestType>(test1, test2));
}

TEMPLATE_TEST_CASE("MLM.Equality.Regular large numbers", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(1000000, 1000001));
    REQUIRE(almostEquals<TestType>(1000001, 1000000));
    REQUIRE(!almostEquals<TestType>(10000, 10001));
    REQUIRE(!almostEquals<TestType>(10001, 10000));
}

TEMPLATE_TEST_CASE("MLM.Equality.Negative large numbers", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(-1000000, -1000001));
    REQUIRE(almostEquals<TestType>(-1000001, -1000000));
    REQUIRE(!almostEquals<TestType>(-10000, -10001));
    REQUIRE(!almostEquals<TestType>(-10001, -10000));
}

TEMPLATE_TEST_CASE("MLM.Equality.Numbers around 1", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>((TestType)1.0000001, (TestType)1.0000002));
    REQUIRE(almostEquals<TestType>((TestType)1.0000002, (TestType)1.0000001));
    REQUIRE(!almostEquals<TestType>((TestType)1.0002, (TestType)1.0001));
    REQUIRE(!almostEquals<TestType>((TestType)1.0001, (TestType)1.0002));
}

/** Numbers around -1 */
TEMPLATE_TEST_CASE("MLM.Equality.Numbers around -1", "[almostEquals]", float, double)
{
    // Caveat! Double actually has enough precision for these to be relatively equal.
    if constexpr (std::is_same_v<TestType, double>)
    {
        REQUIRE(almostEquals<TestType>(-(TestType)1.000001, -(TestType)1.000002));
        REQUIRE(almostEquals<TestType>(-(TestType)1.000002, -(TestType)1.000001));
    }
    else
    {
        REQUIRE(!almostEquals<TestType>(-(TestType)1.000001, -(TestType)1.000002));
        REQUIRE(!almostEquals<TestType>(-(TestType)1.000002, -(TestType)1.000001));
    }

    REQUIRE(!almostEquals<TestType>(-(TestType)1.0001, -(TestType)1.0002));
    REQUIRE(!almostEquals<TestType>(-(TestType)1.0002, -(TestType)1.0001));
}

TEMPLATE_TEST_CASE("MLM.Equality.Numbers between 1 and 0", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>((TestType)0.000000001000001, (TestType)0.000000001000002));
    REQUIRE(almostEquals<TestType>((TestType)0.000000001000002, (TestType)0.000000001000001));
    REQUIRE(!almostEquals<TestType>((TestType)0.000000000001002, (TestType)0.000000000001001));
    REQUIRE(!almostEquals<TestType>((TestType)0.000000000001001, (TestType)0.000000000001002));
}

TEMPLATE_TEST_CASE("MLM.Equality.Numbers between -1 and 0", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(-(TestType)0.000000001000001, -(TestType)0.000000001000002));
    REQUIRE(almostEquals<TestType>(-(TestType)0.000000001000002, -(TestType)0.000000001000001));
    REQUIRE(!almostEquals<TestType>(-(TestType)0.000000000001002, -(TestType)0.000000000001001));
    REQUIRE(!almostEquals<TestType>(-(TestType)0.000000000001001, -(TestType)0.000000000001002));
    REQUIRE(almostEquals<TestType>(-(TestType)0.000000000001001, -(TestType)0.000000000001002,
                                   (TestType)0.1));
}

TEMPLATE_TEST_CASE("MLM.Equality.Small differences away from zero", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>((TestType)0.3, (TestType)0.30000003));
    REQUIRE(almostEquals<TestType>((TestType)-0.3, (TestType)-0.30000003));
}

TEMPLATE_TEST_CASE("MLM.Equality.Comparing zero and denormals", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>((TestType)0.0, (TestType)0.0));
    REQUIRE(almostEquals<TestType>((TestType)0.0, -(TestType)0.0));
    REQUIRE(almostEquals<TestType>(-(TestType)0.0, -(TestType)0.0));
    REQUIRE(!almostEquals<TestType>((TestType)0.00000001, (TestType)0.0));
    REQUIRE(!almostEquals<TestType>((TestType)0.0, (TestType)0.00000001));
    REQUIRE(!almostEquals<TestType>(-(TestType)0.00000001, (TestType)0.0));
    REQUIRE(!almostEquals<TestType>((TestType)0.0, -(TestType)0.00000001));

    // 1e-40 is denormalized in float format
    REQUIRE(!almostEquals<TestType>((TestType)0.0, (TestType)1e-40, (TestType)0.01));
    REQUIRE(!almostEquals<TestType>((TestType)1e-40, (TestType)0.0, (TestType)0.01));
    REQUIRE(!almostEquals<TestType>((TestType)1e-40, (TestType)0.0, (TestType)0.000001));
    REQUIRE(!almostEquals<TestType>((TestType)0.0, (TestType)1e-40, (TestType)0.000001));

    REQUIRE(!almostEquals<TestType>((TestType)0.0, (TestType)-1e-40, (TestType)0.1));
    REQUIRE(!almostEquals<TestType>((TestType)-1e-40, (TestType)0.0, (TestType)0.1));
    REQUIRE(!almostEquals<TestType>((TestType)-1e-40, (TestType)0.0, (TestType)0.00000001));
    REQUIRE(!almostEquals<TestType>((TestType)0.0, (TestType)-1e-40, (TestType)0.00000001));
}

TEMPLATE_TEST_CASE("MLM.Equality.Comparisons involving extreme values (overflow potential)",
                   "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(highest<TestType>(), highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(highest<TestType>(), -highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(-highest<TestType>(), highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(highest<TestType>(), highest<TestType>() / 2));
    REQUIRE(!almostEquals<TestType>(highest<TestType>(), -highest<TestType>() / 2));
    REQUIRE(!almostEquals<TestType>(-highest<TestType>(), highest<TestType>() / 2));
}

TEMPLATE_TEST_CASE("MLM.Equality.Comparisons involving infinities", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(positiveInfinity<TestType>(), positiveInfinity<TestType>()));
    REQUIRE(almostEquals<TestType>(negativeInfinity<TestType>(), negativeInfinity<TestType>()));
    REQUIRE(!almostEquals<TestType>(negativeInfinity<TestType>(), positiveInfinity<TestType>()));
    REQUIRE(!almostEquals<TestType>(positiveInfinity<TestType>(), highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(negativeInfinity<TestType>(), -highest<TestType>()));
}

TEMPLATE_TEST_CASE("MLM.Equality.Comparisons involving NaN values", "[almostEquals]", float, double)
{
    // NaN should never be considered equal.
    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), (TestType)0.0));
    REQUIRE(!almostEquals<TestType>(-(TestType)0.0, NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), -(TestType)0.0));
    REQUIRE(!almostEquals<TestType>((TestType)0.0, NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), positiveInfinity<TestType>()));
    REQUIRE(!almostEquals<TestType>(positiveInfinity<TestType>(), NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), negativeInfinity<TestType>()));
    REQUIRE(!almostEquals<TestType>(negativeInfinity<TestType>(), NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(highest<TestType>(), NaN<TestType>()));
    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), -highest<TestType>()));
    REQUIRE(!almostEquals<TestType>(-highest<TestType>(), NaN<TestType>()));

    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(minPositiveNormalized<TestType>(), NaN<TestType>()));
    REQUIRE(!almostEquals<TestType>(NaN<TestType>(), -minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(-minPositiveNormalized<TestType>(), NaN<TestType>()));
}

TEMPLATE_TEST_CASE("MLM.Equality.Comparisons of numbers on opposite sides of 0", "[almostEquals]",
                   float, double)
{
    REQUIRE(!almostEquals<TestType>((TestType)1.000000001, -(TestType)1.0));
    REQUIRE(!almostEquals<TestType>(-(TestType)1.0, (TestType)1.000000001));
    REQUIRE(!almostEquals<TestType>(-(TestType)1.000000001, (TestType)1.0));
    REQUIRE(!almostEquals<TestType>((TestType)1.0, -(TestType)1.000000001));
    REQUIRE(!almostEquals<TestType>(10 * minPositiveNormalized<TestType>(),
                                    10 * -minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(10000 * minPositiveNormalized<TestType>(),
                                    10000 * -minPositiveNormalized<TestType>()));
}

/**
 * The really tricky part
 */
TEMPLATE_TEST_CASE("MLM.Equality.Comparing numbers close to 0", "[almostEquals]", float, double)
{
    REQUIRE(almostEquals<TestType>(minPositiveNormalized<TestType>(),
                                   minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(minPositiveNormalized<TestType>(),
                                    -minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(-minPositiveNormalized<TestType>(),
                                    minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(minPositiveNormalized<TestType>(), 0));
    REQUIRE(!almostEquals<TestType>(0, minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(-minPositiveNormalized<TestType>(), 0));
    REQUIRE(!almostEquals<TestType>(0, -minPositiveNormalized<TestType>()));

    REQUIRE(!almostEquals<TestType>((TestType)0.000000001, -minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>((TestType)0.000000001, minPositiveNormalized<TestType>()));
    REQUIRE(!almostEquals<TestType>(minPositiveNormalized<TestType>(), (TestType)0.000000001));
    REQUIRE(!almostEquals<TestType>(-minPositiveNormalized<TestType>(), (TestType)0.000000001));

    REQUIRE(almostZero<TestType>((TestType)0.00000000001));
}

#endif // COMPILE_EQUALITY_TESTS
