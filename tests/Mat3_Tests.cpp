#include "catch2/catch_amalgamated.hpp"

#define COMPILE_MAT3_TESTS 0

#if COMPILE_MAT3_TESTS
    #include "mlm/matrix/Mat3.h"
    #include "mlm_TestsCommon.h"

TEMPLATE_TEST_CASE("MLM.Mat3.Test constructors and assignments", "[Mat3]", float, double)
{
    STATIC_REQUIRE(sizeof(TestMat3) == sizeof(TestType) * 9);

    TestMat3 defaultMat;
    REQUIRE((defaultMat[0].isZero() && defaultMat[1].isZero() && defaultMat[2].isZero()));

    TestMat3 mat1(42);
    REQUIRE((mat1[0] == Column3{42, 0, 0} && mat1[1] == Column3{0, 42, 0} &&
             mat1[2] == Column3{0, 0, 42}));

    TestMat3 mat2(1, 2, 3, 4, 5, 6, 7, 8, 9);
    REQUIRE((mat2[0] == Column3{1, 2, 3} && mat2[1] == Column3{4, 5, 6} &&
             mat2[2] == Column3{7, 8, 9}));

    TestMat3 mat3 = mat2;
    REQUIRE((mat3[0] == Column3{1, 2, 3} && mat3[1] == Column3{4, 5, 6} &&
             mat3[2] == Column3{7, 8, 9}));

    mat3 = defaultMat;
    REQUIRE((mat3[0].isZero() && mat3[1].isZero() && mat3[2].isZero()));
}

TEMPLATE_TEST_CASE("MLM.Mat3.Operators", "[Mat3]", float, double)
{
    TestMat3 mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
    TestMat3 mat2 = mat1 * mat1;
    REQUIRE((mat2[0] == Column3{30, 36, 42} && mat2[1] == Column3{66, 81, 96} &&
             mat2[2] == Column3{102, 126, 150}));

    mat2 = mat1;
    mat2 *= mat1;
    REQUIRE((mat2[0] == Column3{30, 36, 42} && mat2[1] == Column3{66, 81, 96} &&
             mat2[2] == Column3{102, 126, 150}));

    mat2 = mat1 * mat1;
    REQUIRE((mat2[0] == Column3{30, 36, 42} && mat2[1] == Column3{66, 81, 96} &&
             mat2[2] == Column3{102, 126, 150}));

    TestMat3 mat3(2, 4, 8, 16, 32, 64, 128, 256, 512);
    mat3 /= 2;
    REQUIRE((mat3[0] == Column3{1, 2, 4} && mat3[1] == Column3{8, 16, 32} &&
             mat3[2] == Column3{64, 128, 256}));

    REQUIRE(mat1(0, 0) == 1);
    REQUIRE(mat1(1, 1) == 5);
    REQUIRE(mat1(1, 2) == 6);
    REQUIRE(mat1(2, 2) == 9);

    REQUIRE_THROWS_AS(mat1(-1, 0), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(0, -1), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(99, 2), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(3, 42), mlm::RuntimeException);

    REQUIRE_THROWS_AS(mat1[-1], mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1[4], mlm::RuntimeException);
}

TEMPLATE_TEST_CASE("MLM.Mat3.Identity", "[Mat3]", float, double)
{
    TestMat3 id = TestMat3::identity();
    REQUIRE((id[0] == Column3{1, 0, 0} && id[1] == Column3{0, 1, 0} && id[2] == Column3{0, 0, 1}));
}

TEMPLATE_TEST_CASE("MLM.Mat3.Rotations", "[Mat3]", float, double)
{
    const auto degRads45 = (TestType)0.707106769;
    const TestMat3 matX45 = TestMat3::rotationAroundX(TestDegrees(45));
    const TestMat3 matY45 = TestMat3::rotationAroundY(TestDegrees(45));
    const TestMat3 matZ45 = TestMat3::rotationAroundZ(TestDegrees(45));

    SECTION("RotationAroundX")
    {
        const TestMat3 matX = TestMat3::rotationAroundX(TestDegrees(0));
        REQUIRE(matX == TestMat3::identity());

        REQUIRE((matX45[0] == Column3{1, 0, 0} && matX45[1] == Column3{0, degRads45, degRads45} &&
                 matX45[2] == Column3{0, -degRads45, degRads45}));
    }

    SECTION("RotationAroundY")
    {
        const TestMat3 matY = TestMat3::rotationAroundY(TestDegrees(0));
        REQUIRE(matY == TestMat3::identity());

        REQUIRE((matY45[0] == Column3{degRads45, 0, -degRads45} && matY45[1] == Column3{0, 1, 0} &&
                 matY45[2] == Column3{degRads45, 0, degRads45}));
    }

    SECTION("RotationAroundZ")
    {
        const TestMat3 matZ = TestMat3::rotationAroundZ(TestDegrees(0));
        REQUIRE(matZ == TestMat3::identity());

        REQUIRE((matZ45[0] == Column3{degRads45, degRads45, 0} &&
                 matZ45[1] == Column3{-degRads45, degRads45, 0} && matZ45[2] == Column3{0, 0, 1}));

        // Test rotating a point
        TestVec3 point(1, 0, 0);
        point = TestMat3::rotationAroundZ(TestDegrees(135)) * point;
        TestVec3 expected(-degRads45, degRads45, 0);
        REQUIRE(point == expected);
    }

    SECTION("Rotation")
    {
        // Test the general rotation against the canonical axes
        TestMat3 gMat = TestMat3::rotation({1, 0, 0}, TestDegrees(45));
        REQUIRE(gMat == matX45);

        gMat = TestMat3::rotation({0, 1, 0}, TestDegrees(45));
        REQUIRE(gMat == matY45);

        gMat = TestMat3::rotation({0, 0, 1}, TestDegrees(45));
        REQUIRE(gMat == matZ45);

        // Test the general rotation with an arbitrary axis
        const auto cosX = (TestType)0.804737866;
        const auto cosY = (TestType)0.310617208;
        const auto cosZ = (TestType)0.505879343;

        gMat = TestMat3::rotation({1, 1, -1}, TestDegrees(45));
        REQUIRE((gMat[0] == Column3{cosX, -cosY, -cosZ} && gMat[1] == Column3{cosZ, cosX, cosY} &&
                 gMat[2] == Column3{cosY, -cosZ, cosX}));
    }
}

TEMPLATE_TEST_CASE("MLM.Mat3.Scale", "[Mat3]", float, double)
{
    TestMat3 scale = TestMat3::scale(1, 2, 3);
    REQUIRE((scale[0] == Column3{1, 0, 0} && scale[1] == Column3{0, 2, 0} &&
             scale[2] == Column3{0, 0, 3}));

    TestMat3 scale2 = TestMat3::scale(TestVec3(1, 2, 3));
    REQUIRE(scale == scale2);

    TestVec3 point(1, 2, 3);
    point = scale * point;
    TestVec3 expected(1, 4, 9);
    REQUIRE(point == expected);
}

TEMPLATE_TEST_CASE("MLM.Mat3.Transpose", "[Mat3]", float, double)
{
    TestMat3 id = TestMat3::identity();
    TestMat3 t = id.transpose();
    REQUIRE(t == id);

    TestMat3 mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
    t = mat1.transpose();
    REQUIRE((t[0] == Column3{1, 4, 7} && t[1] == Column3{2, 5, 8} && t[2] == Column3{3, 6, 9}));
}

TEMPLATE_TEST_CASE("MLM.Mat3.Inverse", "[Mat3]", float, double)
{
    // Test the inverse of a scale matrix
    TestMat3 scale = TestMat3::scale(1, 2, 3);
    TestVec3 point(2, 4, 6);
    point = scale.inverse() * point;
    TestVec3 expected(2, 2, 2);
    REQUIRE(point == expected);

    // Test the inverse of a rotation matrix
    TestMat3 rot = TestMat3::rotationAroundZ(TestDegrees(135));
    auto degRads45 = (TestType)0.707106769;
    point = TestVec3(1, 0, 0);
    point = rot * point;
    expected = TestVec3(-degRads45, degRads45, 0);
    REQUIRE(point == expected);
    TestMat3 invRot = rot.inverse();
    point = invRot * point;
    expected = TestVec3(1, 0, 0);
    REQUIRE(point == expected);

    // Actually, the inverse of a rotation matrix should be equal to its transpose.
    REQUIRE(invRot == rot.transpose());
}

    // Set to 1 to compile Euler angles conversion methods tests
    #define COMPILE_MAT3_EULER_ANGLES_TESTS 0
    #if COMPILE_MAT3_EULER_ANGLES_TESTS
TEMPLATE_TEST_CASE("MLM.Mat3.toEulerAngles", "[Mat3]", float, double)
{
    TestMat3 rot = TestMat3::rotationAroundX(TestDegrees(30));
    TestEulerAnglesType angles = rot.toEulerAngles();
    REQUIRE(angles.equalsWithMargin<TestType>(TestEulerAngles(30, 0, 0)));

    rot = TestMat3::rotationAroundY(TestDegrees(20));
    angles = rot.toEulerAngles();
    REQUIRE((angles.yaw() == 20 && angles.pitch() == 0 && angles.roll() == 0));

    rot = TestMat3::rotationAroundZ(TestDegrees(45));
    angles = rot.toEulerAngles();
    REQUIRE((angles.yaw() == 0 && angles.pitch() == 45 && angles.roll() == 0));

    rot = TestMat3::rotationAroundX(TestDegrees(45)) * TestMat3::rotationAroundY(TestDegrees(90));
    angles = rot.toEulerAngles();
    REQUIRE((angles.yaw() == 90 && angles.pitch() == 0 && angles.roll() == 45));

    rot = TestMat3::rotationAroundX(TestDegrees(45)) * TestMat3::rotationAroundZ(TestDegrees(30)) *
          TestMat3::rotationAroundY(TestDegrees(90));
    angles = rot.toEulerAngles();
    REQUIRE(angles.equalsWithMargin<TestType>(TestEulerAngles(45, 30, 90)));

    rot = TestMat3::rotationAroundX(TestDegrees(90)) * TestMat3::rotationAroundZ(TestDegrees(90)) *
          TestMat3::rotationAroundY(TestDegrees(90));

    angles = rot.toEulerAngles();
    TestType epsilon = static_cast<TestType>(1.0e-5);
    REQUIRE((angles.equalsWithMargin<TestType>(TestEulerAngles(-90, 90, -90), epsilon) ||
             angles.equalsWithMargin<TestType>(TestEulerAngles(90, 90, 90), epsilon)));

    // Singularity (90/-90deg of pitch)
    rot = TestMat3::rotationAroundZ(TestDegrees(90));
    angles = rot.toEulerAngles();
    // As weird as it may look, a -180deg yaw, 90deg pitch, 180deg roll rotation sequence gives the
    // same orientation as a simple 90deg pitch. It just goes "round the other way" (try it with
    // your fingers...)
    REQUIRE((angles.equalsWithMargin<TestType>(TestEulerAngles(180, 90, -180), epsilon) ||
             angles.equalsWithMargin<TestType>(TestEulerAngles(0, 90, 0), epsilon)));

    rot = TestMat3::rotationAroundZ(TestDegrees(-90));
    angles = rot.toEulerAngles();
    REQUIRE((angles.equalsWithMargin<TestType>(TestEulerAngles(180, -90, 180), epsilon) ||
             angles.equalsWithMargin<TestType>(TestEulerAngles(0, -90, 0), epsilon)));

    // To test it more manually, you can uncomment this and try it out yourself!
    // (It needs to include Quaternion.h)
    // TestVec3 test(1, 0, 0);
    // TestQuat qt = TestQuat::fromMatrix(rot);
    // TestQuat qt2(angles);
    // test = TestVec3(1, 0, 0);
    // test = rot * test;
    //
    // test = TestVec3(1, 0, 0);
    // test = qt2.rotate(test);
}

TEMPLATE_TEST_CASE("MLM.Mat3.fromEulerAngles", "[Mat3]", float, double)
{
    SECTION("Roll (X)")
    {
        TEulerAngles<TestDegrees> euler{TestDegrees(-45), TestDegrees(-0), TestDegrees(0)};

        TestMat3 rot = TestMat3::fromEulerAngles(euler);
        TestMat3 rot2 = TestMat3::rotationAroundX(euler.roll);
        REQUIRE(rot == rot2);

        TestVec3 pt(0, 1, 0);
        pt = rot * pt;
        const mlm::Rads_f quarter_pi(mlm::pi() / 4);
        const auto expected = TestVec3(0, mlm::cos(quarter_pi)(), -mlm::sin(quarter_pi)());
        REQUIRE(pt == expected);
    }

    SECTION("pitch (Z)")
    {
        TEulerAngles<TestDegrees> euler{TestDegrees(-0), TestDegrees(-45), TestDegrees(0)};

        TestMat3 rot = TestMat3::fromEulerAngles(euler);
        TestMat3 rot2 = TestMat3::rotationAroundZ(euler.pitch);
        REQUIRE(rot == rot2);

        TestVec3 pt(0, 1, 0);
        pt = rot * pt;
        const mlm::Rads_f quarter_pi(mlm::pi() / 4);
        const auto expected = TestVec3(mlm::cos(quarter_pi)(), mlm::sin(quarter_pi)(), 0);
        REQUIRE(pt == expected);
    }

    SECTION("Yaw (Y)")
    {
        TEulerAngles<TestDegrees> euler{TestDegrees(-0), TestDegrees(0), TestDegrees(-45)};

        TestMat3 rot = TestMat3::fromEulerAngles(euler);
        TestMat3 rot2 = TestMat3::rotationAroundY(euler.yaw);
        REQUIRE(rot == rot2);

        TestVec3 pt(1, 0, 0);
        pt = rot * pt;
        const mlm::Rads_f quarter_pi(mlm::pi() / 4);
        const auto expected = TestVec3(mlm::cos(quarter_pi)(), 0.f, mlm::sin(quarter_pi)());
        REQUIRE(pt == expected);
    }

    SECTION("Singularity 90 degrees")
    {
        TEulerAngles<TestDegrees> euler{TestDegrees(0), TestDegrees(90), TestDegrees(0)};

        TestMat3 rot = TestMat3::fromEulerAngles(euler);
        TestVec3 pt(1, 0, 0);
        pt = rot * pt;
        const auto expected = TestVec3(0, 1, 0);
        REQUIRE(pt.equalsWithMargin(expected));
    }

    SECTION("Singularity -90 degrees")
    {
        TEulerAngles<TestDegrees> euler{TestDegrees(0), TestDegrees(-90), TestDegrees(0)};

        TestMat3 rot = TestMat3::fromEulerAngles(euler);
        TestVec3 pt(1, 0, 0);
        pt = rot * pt;
        const auto expected = TestVec3(0, -1, 0);
        REQUIRE(pt.equalsWithMargin(expected));
    }
}
    #endif // COMPILE_MAT3_EULER_ANGLES_TESTS
#endif     // COMPILE_MAT3_TESTS
