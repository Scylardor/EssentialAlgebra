#pragma once

using namespace mlm;

#define PI mlm::pi<TestType>()

#define TestDegrees Degs<TestType>
#define TestEulerAnglesType TEulerAngles<TestDegrees>
#define TestEulerAngles(roll, pitch, yaw)                                                          \
    TestEulerAnglesType(TestDegrees(roll), TestDegrees(pitch), TestDegrees(yaw))

#define TestVec2 Vector<TestType, 2>
#define TestVec3 Vector<TestType, 3>
#define TestVec4 Vector<TestType, 4>

#define TestMat3 Matrix<TestType, 3, 3>
#define TestMat4 Matrix<TestType, 4, 4>
#define Column3 Vector<TestType, 3>
#define Column4 Vector<TestType, 4>

#define TestQuat Quaternion<TestType>