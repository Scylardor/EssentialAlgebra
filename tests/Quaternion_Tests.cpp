#include "catch2/catch_amalgamated.hpp"

#define COMPILE_QUATERNION_TESTS 0

#if COMPILE_QUATERNION_TESTS
    #include "mlm/quaternion/Quaternion.h"
    #include "mlm_TestsCommon.h"

/// @brief Fixture used to hold a few variables common to a number of tests.
/// Avoids repeating ourselves
template <typename TestType>
struct QuaternionTestFixture
{
    // remember we store half the wanted angle in the quat
    const Rads<TestType> halfRightAngle{TestDegrees(45)};
    const TestType halfCos = mlm::cos(halfRightAngle)();
    const TestType halfSin = mlm::sin(halfRightAngle)();
    const TestVec3 axis = TestVec3(0, 0, 1) * halfSin;
    const TestQuat q1 = TestQuat::identity();

    TestQuat rotQuat;

    QuaternionTestFixture() : rotQuat(axis, halfCos) { REQUIRE(rotQuat.isUnit()); }
};

    #define TestFixture QuaternionTestFixture<TestType>

TEMPLATE_TEST_CASE("MLM.Quaternion.Test constructors and assignments", "[Quaternion]", float,
                   double)
{
    STATIC_REQUIRE(sizeof(TestQuat) == sizeof(TestType) * 4);

    TestQuat defaultQuat;
    REQUIRE((defaultQuat.isZero()));

    TestQuat q1(1, 2, 3, 4);
    REQUIRE((q1.v == TestVec3(1, 2, 3) && q1.w == 4));

    q1 = TestQuat(TestVec3(1, 2, 3), 4);
    REQUIRE((q1.v == TestVec3(1, 2, 3) && q1.w == 4));

    TestQuat q2(q1);
    REQUIRE((q2.v == TestVec3(1, 2, 3) && q2.w == 4));

    q2 = TestQuat::identity();
    REQUIRE((q2.v.isZero() && q2.w == 1));

    TestQuat q3("3 4 5 6");
    TestQuat q4;
    q4 = std::string("3 4 5 6");
    REQUIRE((q3 == q4 && q3 == TestQuat(3, 4, 5, 6)));
}

TEMPLATE_TEST_CASE("MLM.Quaternion.Operators", "[Quaternion]", float, double)
{
    TestQuat q1(1, 2, 3, 4);
    TestQuat q2(5, 6, 7, 8);

    TestQuat q3 = q1 + q2;
    REQUIRE(q3 == TestQuat(6, 8, 10, 12));

    q3 += TestQuat(1, 3, 3, 7);
    REQUIRE(q3 == TestQuat(7, 11, 13, 19));

    q3 = q2 - q1;
    REQUIRE(q3 == TestQuat(4, 4, 4, 4));

    q3 -= q1;
    REQUIRE(q3 == TestQuat(3, 2, 1, 0));

    q3 = q1;
    q3 *= 2;
    REQUIRE(q3 == TestQuat(2, 4, 6, 8));

    q3 = TestQuat(12, 24, 36, 48);
    q3 /= 4;
    REQUIRE(q3 == TestQuat(3, 6, 9, 12));

    REQUIRE(q3[0] == 3);
    REQUIRE(q3[1] == 6);
    REQUIRE(q3[2] == 9);
    REQUIRE(q3[3] == 12);

    REQUIRE_THROWS_AS(q3[-1], mlm::RuntimeException);
    REQUIRE_THROWS_AS(q3[4], mlm::RuntimeException);
}

TEMPLATE_TEST_CASE("MLM.Quaternion.Getters", "[Quaternion]", float, double)
{
    TestQuat q1(1, 2, 3, 0);
    REQUIRE(q1.isPure());

    q1.w += (TestType)0.5;
    REQUIRE(!q1.isPure());

    REQUIRE(!q1.isReal());

    q1.v = TestVec3(0);
    REQUIRE(q1.isReal());

    REQUIRE(!q1.isUnit());

    q1 = TestQuat::identity();
    REQUIRE(q1.isUnit());
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Rotations", "[Quaternion]", float,
                          double)
{
    TestVec3 point(1, 0, 0);
    point = TestFixture::rotQuat.rotate(point);

    TestVec3 expected(0, 1, 0);
    REQUIRE(point.equalsWithMargin(expected, (TestType)1e-7));

    // Multiplying the quaternion by itself should double the rotation amount
    TestFixture::rotQuat *= TestFixture::rotQuat;
    point = TestFixture::rotQuat.rotate(point);

    expected = TestVec3(0, -1, 0);
    REQUIRE(point.equalsWithMargin(expected, (TestType)1e-7));
}

TEMPLATE_TEST_CASE("MLM.Quaternion.Magnitude", "[Quaternion]", float, double)
{
    TestQuat q1 = TestQuat::identity();
    REQUIRE(q1.magnitude() == q1.squareMagnitude());
    REQUIRE(q1.norm() == q1.magnitude());
    REQUIRE(q1.norm() == q1.modulus());
    REQUIRE(q1.modulus() == 1);

    q1 = TestQuat(3, 0, 0, 0);
    REQUIRE(q1.modulus() == 3);

    q1 = TestQuat(4, 4, 0, 2);
    REQUIRE(mlm::almostEquals((TestType)6, q1.modulus()));
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Dot", "[Quaternion]", float,
                          double)
{
    // If the dot product is equal to 1, the two quaternions apply similar rotations.
    // Also, since a negated quaternion performs the same rotation as the original,
    // this also stands if the dot product is close to -1.
    // The quaternions yield very different rotations if the dot product is close to 0.
    TestQuat rotQuatCpy = TestFixture::rotQuat;
    TestType dp = rotQuatCpy.dot(rotQuatCpy);
    REQUIRE(mlm::almostEquals(dp, (TestType)1));
    dp = rotQuatCpy.dot(-rotQuatCpy);
    REQUIRE(mlm::almostEquals(dp, (TestType)-1));

    const TestVec3 anAxis = TestVec3(0, -5, -2) * TestFixture::halfSin;
    TestQuat rotQuat2 = TestQuat(anAxis, TestFixture::halfCos);
    dp = rotQuatCpy.dot(rotQuat2);
    REQUIRE(mlm::almostEquals(dp, (TestType)-0.5));
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Inverse", "[Quaternion]", float,
                          double)
{
    const TestVec3 point(1, 0, 0);
    TestVec3 point2 = TestFixture::rotQuat.rotate(point);

    TestVec3 expected(0, 1, 0);
    REQUIRE(point2.equalsWithMargin(expected, (TestType)1e-7));

    const TestQuat invQuat = TestFixture::rotQuat.inverse();

    // Multiplying the transformed point by the quaternion inverse should yield the original point
    point2 = invQuat.rotate(point2);
    REQUIRE(point.equalsWithMargin(point2, (TestType)1e-6));

    // Multiplying the quaternion by its inverse should give a unit quaternion.
    const TestQuat unit = invQuat * TestFixture::rotQuat;
    REQUIRE((unit.isUnit() && unit == TestQuat::identity()));

    // For a rotation quaternion, the inverse should be equal to the conjugate.
    REQUIRE(TestFixture::rotQuat.conjugate() == invQuat);

    // Since a rotation quaternion is supposed to be unit, we should be able to use inverseUnit
    // instead
    REQUIRE(invQuat == TestFixture::rotQuat.inverseUnit());
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Log/Exp/Pow", "[Quaternion]",
                          float, double)
{
    // After Log, we're supposed to have the vector scaled by the half-angle and w to 0.
    TestQuat logQuat = TestFixture::rotQuat.log();
    REQUIRE(logQuat == TestQuat(0, 0, TestFixture::halfRightAngle(), 0));

    // Exp takes the result from Log, then re-transforms it to the original quaternion.
    TestQuat expQuat = logQuat.exp();
    REQUIRE(expQuat == TestFixture::rotQuat);

    // pow(2) should double the quaternion
    TestQuat pow2Quat = TestFixture::rotQuat.pow(2);

    TestQuat doubleQuat = TestQuat::fromAngleAxis(TestDegrees(180), TestFixture::axis);
    REQUIRE(pow2Quat == doubleQuat);

    // pow(1/3) should only rotate by a third of the quaternion
    TestQuat powThirdQuat = TestFixture::rotQuat.pow(1 / (TestType)3);

    TestQuat thirdQuat = TestQuat::fromAngleAxis(TestDegrees(30), TestFixture::axis);
    REQUIRE(powThirdQuat == thirdQuat);

    // Identity quaternion tests
    logQuat = TestQuat::identity().log();
    REQUIRE(logQuat == TestQuat());
    REQUIRE(TestQuat::identity().exp() == TestQuat::identity());
    REQUIRE(TestQuat::identity().pow(2) == TestQuat::identity());
    REQUIRE(TestQuat::identity().pow(1 / (TestType)3) == TestQuat::identity());
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Matrix conversions",
                          "[Quaternion]", float, double)
{
    TestMat3 rotMatrix = TestFixture::rotQuat.toMatrix();

    // Should give the same result than the matrix computation
    TestMat3 expected = TestMat3::rotationAroundZ(TestDegrees(90));
    REQUIRE(rotMatrix.equalsWithMargin(expected, (TestType)1e-6));

    TestQuat qFromMat = TestQuat::fromMatrix(rotMatrix);

    // Rotating points by the original quaternion and the quaternion from the matrix should yield
    // approximately the same result.
    TestVec3 point1(1, 0, 0);
    TestVec3 point2(1, 0, 0);
    point1 = TestFixture::rotQuat.rotate(point1);
    point2 = qFromMat.rotate(point2);
    REQUIRE(point1.equalsWithMargin(point2, (TestType)1e-7));

    // Testing multiple conditions of FromMatrix
    // 90 degrees positive (so counterclockwise) rotation on the X-axis
    rotMatrix = TestMat3::rotationAroundX(TestDegrees(90));
    qFromMat = TestQuat::fromMatrix(rotMatrix);
    point1 = TestVec3(1, 0, 0);
    point2 = point1;
    point1 = qFromMat.rotate(point1);
    // should not change
    REQUIRE(point1.equalsWithMargin(point2, (TestType)1e-7));
}

TEMPLATE_TEST_CASE_METHOD(QuaternionTestFixture, "MLM.Quaternion.Angle-axis conversions",
                          "[Quaternion]", float, double)
{
    AngleAxis<TestType> anglax = TestFixture::rotQuat.toAngleAxis();
    REQUIRE((anglax.Angle() == 90 && anglax.Axis == TestVec3(0, 0, 1)));

    const TestQuat qFromAA = TestQuat::fromAngleAxis(anglax);
    REQUIRE(qFromAA == TestFixture::rotQuat);

    anglax = ((TestFixture::rotQuat * 2).normalize()).toAngleAxis();
}

TEMPLATE_TEST_CASE("MLM.Quaternion.Conversion from Euler angles", "[Quaternion]", float, double)
{
    // TEST 1 - 1 non-zero axis
    TestQuat q(TestEulerAngles(0, 45, 0));
    REQUIRE(q == TestQuat(0, 0, (TestType)0.382683456, (TestType)0.923879504));

    // TEST 2 - 2 non-zero axes
    q = TestQuat(TestEulerAngles(0, 45, 90));
    REQUIRE(q == TestQuat((TestType)-0.270598054, (TestType)0.653281450, (TestType)0.270598054,
                          (TestType)0.653281450));

    // TEST 3 - 3 non-zero axes
    q = TestQuat(TestEulerAngles(90, -45, 90));
    REQUIRE(q == TestQuat((TestType)0.653281450, (TestType)0.653281450, (TestType)0.270598054,
                          (TestType)0.270598054));

    // TEST 4 - 3 non-zero axes 2
    q = TestQuat(TestEulerAngles(10, 95, 67));
    REQUIRE(q == TestQuat((TestType)-0.356281668, (TestType)0.317880481, (TestType)0.644964516,
                          (TestType)0.596687675));

    // TEST 5 - Identity quaternion
    q = TestQuat(TestEulerAngles(0, 0, 0));
    REQUIRE(q == TestQuat::identity());
}

TEMPLATE_TEST_CASE("MLM.Quaternion.Conversion to Euler angles", "[Quaternion]", float, double)
{
    // TEST 1 - 1 non-zero axis
    TestQuat q(TestEulerAngles(0, 45, 0));
    TestEulerAnglesType testEuler = q.toEulerAngles();
    REQUIRE(testEuler == TestEulerAngles(0, 45, 0));

    // TEST 2 - 2 non-zero axes
    q = TestQuat(TestEulerAngles(0, 45, 90));
    testEuler = q.toEulerAngles();
    REQUIRE(testEuler == TestEulerAngles(0, 45, 90));

    // TEST 3 - 3 non-zero axes
    q = TestQuat(TestEulerAngles(90, -45, 90));
    testEuler = q.toEulerAngles();
    REQUIRE(testEuler == TestEulerAngles(90, -45, 90));

    // TEST 4 - Shows that multiple sets of Euler angles can lead to the same orientation.
    // The input Euler angles may not always be equal to the output ones, but lead to the same
    // orientation.
    q = TestQuat(TestEulerAngles(10, 95, 67));
    testEuler = q.toEulerAngles();
    const TestType epsilon = static_cast<TestType>(1.0e-4);
    TestQuat q2(testEuler);
    REQUIRE(q == q2);

    // TEST 5 - Identity quaternion
    q = TestQuat(TestEulerAngles(0, 0, 0));
    testEuler = q.toEulerAngles();
    REQUIRE(testEuler == TestEulerAngles(0, 0, 0));
}
#endif // COMPILE_QUATERNION_TESTS
