#include "catch2/catch_amalgamated.hpp"

#define COMPILE_MAT4_TESTS 0

#if COMPILE_MAT4_TESTS
    #include "mlm/matrix/Mat4.h"
    #include "mlm_TestsCommon.h"

TEMPLATE_TEST_CASE("MLM.Mat4.Test constructors and assignments", "[Mat4]", float, double)
{
    STATIC_REQUIRE(sizeof(TestMat4) == sizeof(TestType) * 16);

    TestMat4 defaultMat;
    REQUIRE((defaultMat[0].isZero() && defaultMat[1].isZero() && defaultMat[2].isZero() &&
             defaultMat[3].isZero()));

    TestMat4 mat1(42);
    REQUIRE((mat1[0] == Column4{42, 0, 0, 0} && mat1[1] == Column4{0, 42, 0, 0} &&
             mat1[2] == Column4{0, 0, 42, 0} && mat1[3] == Column4{0, 0, 0, 42}));

    TestMat4 mat2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    REQUIRE((mat2[0] == Column4{1, 2, 3, 4} && mat2[1] == Column4{5, 6, 7, 8} &&
             mat2[2] == Column4{9, 10, 11, 12} && mat2[3] == Column4{13, 14, 15, 16}));

    TestMat4 mat3 = mat2;
    REQUIRE(mat3 == mat2);

    mat3 = defaultMat;
    REQUIRE((mat3[0].isZero() && mat3[1].isZero() && mat3[2].isZero() && mat3[3].isZero()));
}

TEMPLATE_TEST_CASE("MLM.Mat4.Operators", "[Mat4]", float, double)
{
    TestMat4 mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    TestMat4 mat2 = mat1 * mat1;
    REQUIRE((mat2[0] == Column4{90, 100, 110, 120} && mat2[1] == Column4{202, 228, 254, 280} &&
             mat2[2] == Column4{314, 356, 398, 440} && mat2[3] == Column4{426, 484, 542, 600}));

    mat2 = mat1;
    mat2 *= mat1;
    REQUIRE((mat2[0] == Column4{90, 100, 110, 120} && mat2[1] == Column4{202, 228, 254, 280} &&
             mat2[2] == Column4{314, 356, 398, 440} && mat2[3] == Column4{426, 484, 542, 600}));

    mat2 = mat1 * mat1;
    REQUIRE((mat2[0] == Column4{90, 100, 110, 120} && mat2[1] == Column4{202, 228, 254, 280} &&
             mat2[2] == Column4{314, 356, 398, 440} && mat2[3] == Column4{426, 484, 542, 600}));

    TestMat4 mat3(2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536);
    mat3 /= 2;
    REQUIRE((mat3[0] == Column4{1, 2, 4, 8} && mat3[1] == Column4{16, 32, 64, 128} &&
             mat3[2] == Column4{256, 512, 1024, 2048} &&
             mat3[3] == Column4{4096, 8192, 16384, 32768}));

    REQUIRE(mat1(0, 0) == 1);
    REQUIRE(mat1(1, 1) == 6);
    REQUIRE(mat1(1, 2) == 7);
    REQUIRE(mat1(2, 2) == 11);
    REQUIRE(mat1(2, 3) == 12);
    REQUIRE(mat1(3, 3) == 16);

    REQUIRE_THROWS_AS(mat1(-1, 0), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(0, -1), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(99, 2), mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1(3, 42), mlm::RuntimeException);

    REQUIRE_THROWS_AS(mat1[-1], mlm::RuntimeException);
    REQUIRE_THROWS_AS(mat1[4], mlm::RuntimeException);
}

TEMPLATE_TEST_CASE("MLM.Mat4.Conversion to Mat3", "[Mat4]", float, double)
{
    TestMat4 mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    TestMat3 mat3 = mat1.toMat3();
    REQUIRE((mat3[0] == TestVec3{1, 2, 3} && mat3[1] == TestVec3{5, 6, 7} &&
             mat3[2] == TestVec3{9, 10, 11}));
}

TEMPLATE_TEST_CASE("MLM.Mat4.Identity", "[Mat4]", float, double)
{
    TestMat4 id = TestMat4::identity();
    REQUIRE((id[0] == Column4{1, 0, 0, 0} && id[1] == Column4{0, 1, 0, 0} &&
             id[2] == Column4{0, 0, 1, 0} && id[3] == Column4{0, 0, 0, 1}));
}

TEMPLATE_TEST_CASE("MLM.Mat4.Rotations", "[Mat4]", float, double)
{
    const auto degRads45 = (TestType)0.707106769;
    const TestMat4 matX45 = TestMat4::rotationAroundX(TestDegrees(45));
    const TestMat4 matY45 = TestMat4::rotationAroundY(TestDegrees(45));
    const TestMat4 matZ45 = TestMat4::rotationAroundZ(TestDegrees(45));

    SECTION("RotationAroundX")
    {
        const TestMat4 matX = TestMat4::rotationAroundX(TestDegrees(0));
        REQUIRE(matX == TestMat4::identity());

        REQUIRE((matX45[0] == Column4{1, 0, 0} && matX45[1] == Column4{0, degRads45, degRads45} &&
                 matX45[2] == Column4{0, -degRads45, degRads45}));
    }

    SECTION("RotationAroundY")
    {
        const TestMat4 matY = TestMat4::rotationAroundY(TestDegrees(0));
        REQUIRE(matY == TestMat4::identity());

        REQUIRE((matY45[0] == Column4{degRads45, 0, -degRads45} && matY45[1] == Column4{0, 1, 0} &&
                 matY45[2] == Column4{degRads45, 0, degRads45}));
    }

    SECTION("RotationAroundZ")
    {
        const TestMat4 matZ = TestMat4::rotationAroundZ(TestDegrees(0));
        REQUIRE(matZ == TestMat4::identity());

        REQUIRE((matZ45[0] == Column4{degRads45, degRads45, 0} &&
                 matZ45[1] == Column4{-degRads45, degRads45, 0} && matZ45[2] == Column4{0, 0, 1}));

        // Test rotating a point
        TestVec4 point(1, 0, 0);
        point = TestMat4::rotationAroundZ(TestDegrees(135)) * point;
        TestVec4 expected(-degRads45, degRads45, 0);
        REQUIRE(point == expected);
    }

    SECTION("Rotation")
    {
        // Test the general rotation against the canonical axes
        TestMat4 gMat = TestMat4::rotation({1, 0, 0}, TestDegrees(45));
        REQUIRE(gMat == matX45);

        gMat = TestMat4::rotation({0, 1, 0}, TestDegrees(45));
        REQUIRE(gMat == matY45);

        gMat = TestMat4::rotation({0, 0, 1}, TestDegrees(45));
        REQUIRE(gMat == matZ45);

        // Test the general rotation with an arbitrary axis
        const auto cosX = (TestType)0.804737866;
        const auto cosY = (TestType)0.310617208;
        const auto cosZ = (TestType)0.505879343;

        gMat = TestMat4::rotation({1, 1, -1}, TestDegrees(45));
        REQUIRE((gMat[0] == Column4{cosX, -cosY, -cosZ} && gMat[1] == Column4{cosZ, cosX, cosY} &&
                 gMat[2] == Column4{cosY, -cosZ, cosX}));
    }
}

TEMPLATE_TEST_CASE("MLM.Mat4.Scale", "[Mat4]", float, double)
{
    TestMat4 scale = TestMat4::scale(1, 2, 3);
    REQUIRE((scale[0] == Column4{1, 0, 0} && scale[1] == Column4{0, 2, 0} &&
             scale[2] == Column4{0, 0, 3}));

    TestMat4 scale2 = TestMat4::scale(TestVec3(1, 2, 3));
    REQUIRE(scale == scale2);

    TestVec4 point(1, 2, 3);
    point = scale * point;
    TestVec4 expected(1, 4, 9);
    REQUIRE(point == expected);
}

TEMPLATE_TEST_CASE("MLM.Mat4.Translation", "[Mat4]", float, double)
{
    TestMat4 translation = TestMat4::translation(1, 2, 3);
    REQUIRE((translation.toMat3() == TestMat3::identity() &&
             translation.getColumn(3) == Column4{1, 2, 3, 1}));

    translation = TestMat4::translation(TestVec3(1, 2, 3));
    REQUIRE((translation.toMat3() == TestMat3::identity() &&
             translation.getColumn(3) == Column4{1, 2, 3, 1}));

    // Try scaling a translation
    translation = TestMat4::scale(1, 2, 3) * TestMat4::translation(1, 2, 3);
    REQUIRE((translation[0] == Column4{1, 0, 0, 0} && translation[1] == Column4{0, 2, 0, 0} &&
             translation[2] == Column4{0, 0, 3, 0} && translation[3] == Column4{1, 4, 9, 1}));
}

TEMPLATE_TEST_CASE("MLM.Mat4.Transpose", "[Mat4]", float, double)
{
    TestMat4 id = TestMat4::identity();
    TestMat4 t = id.transpose();
    REQUIRE(t == id);

    TestMat4 mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    t = mat1.transpose();
    REQUIRE((t[0] == Column4{1, 5, 9, 13} && t[1] == Column4{2, 6, 10, 14} &&
             t[2] == Column4{3, 7, 11, 15} && t[3] == Column4(4, 8, 12, 16)));
}

TEMPLATE_TEST_CASE("MLM.Mat4.Inverse", "[Mat4]", float, double)
{
    // Test the inverse of a scale matrix
    TestMat4 scale = TestMat4::scale(1, 2, 3);
    TestVec4 point(2, 4, 6, 1);
    point = scale.inverse() * point;
    TestVec4 expected(2, 2, 2, 1);
    REQUIRE(point == expected);

    // Test the inverse of a rotation matrix
    TestMat4 rot = TestMat4::rotationAroundZ(Degs<TestType>(135));
    auto degRads45 = (TestType)0.707106769;
    point = TestVec3(1, 0, 0);
    point = rot * point;
    expected = TestVec3(-degRads45, degRads45, 0);
    REQUIRE(point == expected);
    TestMat4 invRot = rot.inverse();
    point = invRot * point;
    expected = TestVec3(1, 0, 0);
    REQUIRE(point == expected);

    // Actually, the inverse of a rotation matrix should be equal to its transpose.
    REQUIRE(invRot == rot.transpose());
}
#endif // COMPILE_MAT4_TESTS
