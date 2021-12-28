#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/tools/tools.h"

using namespace testing;

//--------------------------------------------
//     VEC2
//--------------------------------------------

class Vector2 : public Test {
 public:
  Vec2<double> v;
};

TEST_F(Vector2, CreatesVector) {
  v = Vec2<double>(0., 0.);
  ASSERT_DOUBLE_EQ(v.x(), 0.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
}

TEST_F(Vector2, AssertsOutOfBounds) {
  v = Vec2<double>(1., 2.);
  ASSERT_DEATH(v[2], "");
  ASSERT_DEATH(v[-1], "");
}

TEST_F(Vector2, SetsCoords) {
  v.setX(5.);
  v[1] = 4.;
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v[1], 4.);
}

TEST_F(Vector2, SetXY) {
  v.setXY(-1.24, -5.54);
  ASSERT_DOUBLE_EQ(v.x(), -1.24);
  ASSERT_DOUBLE_EQ(v.y(), -5.54);

  v.setXY(6.68);
  ASSERT_DOUBLE_EQ(v.x(), 6.68);
  ASSERT_DOUBLE_EQ(v.y(), 6.68);
}

TEST_F(Vector2, ChangesSign) {
  v.setXY(-1.55);
  v = -v;
  ASSERT_DOUBLE_EQ(v[0], 1.55);
  ASSERT_DOUBLE_EQ(v[1], 1.55);
}

TEST_F(Vector2, AddsVectorOrNumber) {
  v = Vec2<double>(0., 0.);
  v += 4.46;
  ASSERT_DOUBLE_EQ(v.x(), 4.46);
  ASSERT_DOUBLE_EQ(v.y(), 4.46);

  v += Vec2<double>(4., 6.);
  ASSERT_DOUBLE_EQ(v.x(), 8.46);
  ASSERT_DOUBLE_EQ(v.y(), 10.46);
}

TEST_F(Vector2, SubtractsVectorOrNumber) {
  v = Vec2<double>(0., 0.);
  v -= 4.46;
  ASSERT_DOUBLE_EQ(v.x(), -4.46);
  ASSERT_DOUBLE_EQ(v.y(), -4.46);

  v -= Vec2<double>(4., 6.);
  ASSERT_DOUBLE_EQ(v.x(), -8.46);
  ASSERT_DOUBLE_EQ(v.y(), -10.46);
}

TEST_F(Vector2, GetsLenghtOfVector) {
  v = Vec2<double>(0., 0.);
  ASSERT_DOUBLE_EQ(v.length(), 0.);
  v = Vec2<double>(1., 1.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(2.));
  v = Vec2<double>(3., 3.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(18.));
  v = Vec2<double>(-5., -5.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(50.));
}

TEST_F(Vector2, MultipliesVectorWithNumber) {
  v = Vec2<double>(1., 0.);
  v *= 5.;
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
}

TEST_F(Vector2, NormalizesVector) {
  v = Vec2<double>(4.53, 93.5);
  ASSERT_DOUBLE_EQ(v.normalize().length(), 1.);
}

TEST_F(Vector2, DotProduct) {
  v = Vec2<double>(3., 3.);
  ASSERT_DOUBLE_EQ(dot(v, Vec2<double>(3., 3.)), 18.);
  v = Vec2<double>(-1., 5.);
  ASSERT_DOUBLE_EQ(dot(v, Vec2<double>(-3., 3.)), 18.);
}

TEST_F(Vector2, GetUnitVectorOf) {
  v = Vec2<double>(4.36, 7.62);
  v = getUnitVectorOf(v);
  ASSERT_DOUBLE_EQ(v.length(), 1.);
}

TEST_F(Vector2, AddsTwoVectors) {
  v = Vec2<double>(4.532, 45.67);
  v = v + Vec2<double>(0.3456, 124.67);
  ASSERT_DOUBLE_EQ(v.x(), 4.8776);
  ASSERT_DOUBLE_EQ(v.y(), 170.34);
}

TEST_F(Vector2, SubtractsTwoVectors) {
  v = Vec2<double>(40.54, 2.4);
  v = v - Vec2<double>(4.20, -1.7);
  ASSERT_DOUBLE_EQ(v[0], 36.34);
  ASSERT_DOUBLE_EQ(v[1], 4.1);
}

TEST_F(Vector2, DevidesVectorByNumber) {
  v = Vec2<double>(36.6, -30.6);
  v = v / 3.;
  ASSERT_DOUBLE_EQ(v.x(), 36.6 / 3.);
  ASSERT_DOUBLE_EQ(v.y(), -30.6 / 3.);
}

TEST_F(Vector2, DevidesVectorByVector) {
  v = Vec2<double>(434.5, 93.5);
  v = v / Vec2<double>(32.5, -16.2);
  ASSERT_DOUBLE_EQ(v.x(), 434.5 / 32.5);
  ASSERT_DOUBLE_EQ(v.y(), -93.5 / 16.2);
}

TEST_F(Vector2, TestsEquality) {
  v = Vec2<double>(4.2, -6.54);
  ASSERT_TRUE(v == Vec2<double>(4.2, -6.54));
}

TEST_F(Vector2, TestsInequality) {
  v = Vec2<double>(4.2, -6.54);
  ASSERT_TRUE(v != Vec2<double>(-4.2, -4.36));
}

//--------------------------------------------
//     VEC3
//--------------------------------------------

class Vector3 : public Test {
 public:
  Vec3<double> v;
};

TEST_F(Vector3, CreatesVector) {
  v = Vec3<double>(0., 0., 0.);
  ASSERT_DOUBLE_EQ(v.x(), 0.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
  ASSERT_DOUBLE_EQ(v.z(), 0.);
}

TEST_F(Vector3, AssertsOutOfBounds) {
  v = Vec3<double>(1., 2., 0.5);
  ASSERT_DEATH(v[3], "");
  ASSERT_DEATH(v[-1], "");
}

TEST_F(Vector3, SetsCoords) {
  v.setX(5.);
  v[1] = 4.;
  v[2] = 0.2;
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v[1], 4.);
  ASSERT_DOUBLE_EQ(v[2], 0.2);
}

TEST_F(Vector3, SetXYZ) {
  v.setXYZ(-1.24, -5.54, 45.6);
  ASSERT_DOUBLE_EQ(v.x(), -1.24);
  ASSERT_DOUBLE_EQ(v.y(), -5.54);
  ASSERT_DOUBLE_EQ(v.z(), 45.6);

  v.setXYZ(6.68);
  ASSERT_DOUBLE_EQ(v.x(), 6.68);
  ASSERT_DOUBLE_EQ(v.y(), 6.68);
  ASSERT_DOUBLE_EQ(v.z(), 6.68);
}

TEST_F(Vector3, ChangesSign) {
  v.setXYZ(-1.55);
  v = -v;
  ASSERT_DOUBLE_EQ(v[0], 1.55);
  ASSERT_DOUBLE_EQ(v[1], 1.55);
  ASSERT_DOUBLE_EQ(v[2], 1.55);
}

TEST_F(Vector3, AddsVectorOrNumber) {
  v = Vec3<double>(0., 0., 5.);
  v += 4.46;
  ASSERT_DOUBLE_EQ(v.x(), 4.46);
  ASSERT_DOUBLE_EQ(v.y(), 4.46);
  ASSERT_DOUBLE_EQ(v.z(), 9.46);

  v += Vec3<double>(4., 6., 0.);
  ASSERT_DOUBLE_EQ(v.x(), 8.46);
  ASSERT_DOUBLE_EQ(v.y(), 10.46);
  ASSERT_DOUBLE_EQ(v.z(), 9.46);
}

TEST_F(Vector3, SubtractsVectorOrNumber) {
  v = Vec3<double>(0., 0., 3.);
  v -= 4.46;
  ASSERT_DOUBLE_EQ(v.x(), -4.46);
  ASSERT_DOUBLE_EQ(v.y(), -4.46);
  ASSERT_DOUBLE_EQ(v.z(), -1.46);

  v -= Vec3<double>(4., 6., 0.);
  ASSERT_DOUBLE_EQ(v.x(), -8.46);
  ASSERT_DOUBLE_EQ(v.y(), -10.46);
  ASSERT_DOUBLE_EQ(v.z(), -1.46);
}

TEST_F(Vector3, GetsLenghtOfVector) {
  v = Vec3<double>(0., 0., 0.);
  ASSERT_DOUBLE_EQ(v.length(), 0.);
  v = Vec3<double>(1., 1., 1.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(3.));
  v = Vec3<double>(3., 3., 3.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(27.));
  v = Vec3<double>(-5., -5., 5.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(75.));
}

TEST_F(Vector3, MultipliesVectorWithNumber) {
  v = Vec3<double>(1., 0., 5.);
  v *= 5.;
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
  ASSERT_DOUBLE_EQ(v.z(), 25.);
}

TEST_F(Vector3, NormalizesVector) {
  v = Vec3<double>(4.53, 93.5, -56.3);
  ASSERT_DOUBLE_EQ(v.normalize().length(), 1.);
}

TEST_F(Vector3, DotProduct) {
  v = Vec3<double>(3., 3., 4.);
  ASSERT_DOUBLE_EQ(dot(v, Vec3<double>(3., 3., 9.)), 54.);
  v = Vec3<double>(-1., 5., 9.);
  ASSERT_DOUBLE_EQ(dot(v, Vec3<double>(-3., 3., 6.)), 72.);
}

TEST_F(Vector3, GetUnitVectorOf) {
  v = Vec3<double>(4.36, 7.62, 0.466);
  v = getUnitVectorOf(v);
  ASSERT_DOUBLE_EQ(v.length(), 1.);
}

TEST_F(Vector3, AddsTwoVectors) {
  v = Vec3<double>(4.532, 45.67, 0.83);
  v = v + Vec3<double>(0.3456, 124.67, 1.);
  ASSERT_DOUBLE_EQ(v.x(), 4.8776);
  ASSERT_DOUBLE_EQ(v.y(), 170.34);
  ASSERT_DOUBLE_EQ(v.z(), 1.83);
}

TEST_F(Vector3, SubtractsTwoVectors) {
  v = Vec3<double>(40.54, 2.4, 0.62);
  v = v - Vec3<double>(4.20, -1.7, -1.);
  ASSERT_DOUBLE_EQ(v[0], 36.34);
  ASSERT_DOUBLE_EQ(v[1], 4.1);
  ASSERT_DOUBLE_EQ(v[2], 1.62);
}

TEST_F(Vector3, DevidesVectorByNumber) {
  v = Vec3<double>(36.6, -30.6, 120.2586);
  v = v / 3.;
  ASSERT_DOUBLE_EQ(v.x(), 36.6 / 3.);
  ASSERT_DOUBLE_EQ(v.y(), -30.6 / 3.);
  ASSERT_DOUBLE_EQ(v.z(), 120.2586 / 3.);
}

TEST_F(Vector3, DevidesVectorByVector) {
  v = Vec3<double>(434.5, 93.5, 3858.53);
  v = v / Vec3<double>(32.5, -16.2, 0.567);
  ASSERT_DOUBLE_EQ(v.x(), 434.5 / 32.5);
  ASSERT_DOUBLE_EQ(v.y(), -93.5 / 16.2);
  ASSERT_DOUBLE_EQ(v.z(), 3858.53 / 0.567);
}

TEST_F(Vector3, TestsEquality) {
  v = Vec3<double>(4.2, -6.54, 34855.38596);
  ASSERT_TRUE(v == Vec3<double>(4.2, -6.54, 34855.38596));
}

TEST_F(Vector3, TestsInequality) {
  v = Vec3<double>(4.2, -6.54, 0.0);
  ASSERT_TRUE(v != Vec3<double>(4.2, -4.36, 0.0));
}

//--------------------------------------------
//     VEC4
//--------------------------------------------

class Vector4 : public Test {
 public:
  Vec4<double> v;
};

TEST_F(Vector4, CreatesVector) {
  v = Vec4<double>(0., 0., 0., 0.);
  ASSERT_DOUBLE_EQ(v.x(), 0.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
  ASSERT_DOUBLE_EQ(v.z(), 0.);
  ASSERT_DOUBLE_EQ(v.w(), 0.);
}

TEST_F(Vector4, AssertsOutOfBounds) {
  v = Vec4<double>(1., 2., 0.5, 7.5);
  ASSERT_DEATH(v[4], "");
  ASSERT_DEATH(v[-1], "");
}

TEST_F(Vector4, SetsCoords) {
  v.setX(5.);
  v[1] = 4.;
  v[2] = 0.2;
  v.setW(-5.2);
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v[1], 4.);
  ASSERT_DOUBLE_EQ(v[2], 0.2);
  ASSERT_DOUBLE_EQ(v[3], -5.2);
}

TEST_F(Vector4, SetXYZ) {
  v.setXYZW(-1.24, -5.54, 45.6, -0.5);
  ASSERT_DOUBLE_EQ(v.x(), -1.24);
  ASSERT_DOUBLE_EQ(v.y(), -5.54);
  ASSERT_DOUBLE_EQ(v.z(), 45.6);
  ASSERT_DOUBLE_EQ(v.w(), -0.5);

  v.setXYZW(6.68);
  ASSERT_DOUBLE_EQ(v.x(), 6.68);
  ASSERT_DOUBLE_EQ(v.y(), 6.68);
  ASSERT_DOUBLE_EQ(v.z(), 6.68);
  ASSERT_DOUBLE_EQ(v.w(), 6.68);
}

TEST_F(Vector4, ChangesSign) {
  v.setXYZW(-1.55);
  v = -v;
  ASSERT_DOUBLE_EQ(v[0], 1.55);
  ASSERT_DOUBLE_EQ(v[1], 1.55);
  ASSERT_DOUBLE_EQ(v[2], 1.55);
  ASSERT_DOUBLE_EQ(v[3], 1.55);
}

TEST_F(Vector4, AddsVectorOrNumber) {
  v = Vec4<double>(0., 0., 5., 3.5);
  v += 4.46;
  ASSERT_DOUBLE_EQ(v.x(), 4.46);
  ASSERT_DOUBLE_EQ(v.y(), 4.46);
  ASSERT_DOUBLE_EQ(v.z(), 9.46);
  ASSERT_DOUBLE_EQ(v.w(), 7.96);

  v += Vec4<double>(4., 6., 0., 45.);
  ASSERT_DOUBLE_EQ(v.x(), 8.46);
  ASSERT_DOUBLE_EQ(v.y(), 10.46);
  ASSERT_DOUBLE_EQ(v.z(), 9.46);
  ASSERT_DOUBLE_EQ(v.w(), 52.96);
}

TEST_F(Vector4, SubtractsVectorOrNumber) {
  v = Vec4<double>(0., 0., 3., -5.5);
  v -= 4.46;
  ASSERT_DOUBLE_EQ(v.x(), -4.46);
  ASSERT_DOUBLE_EQ(v.y(), -4.46);
  ASSERT_DOUBLE_EQ(v.z(), -1.46);
  ASSERT_DOUBLE_EQ(v.w(), -9.96);

  v -= Vec4<double>(4., 6., 0., -1.2);
  ASSERT_DOUBLE_EQ(v.x(), -8.46);
  ASSERT_DOUBLE_EQ(v.y(), -10.46);
  ASSERT_DOUBLE_EQ(v.z(), -1.46);
  ASSERT_DOUBLE_EQ(v.w(), -8.76);
}

TEST_F(Vector4, GetsLenghtOfVector) {
  v = Vec4<double>(0., 0., 0., 0.);
  ASSERT_DOUBLE_EQ(v.length(), 0.);
  v = Vec4<double>(1., 1., 1., 1.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(4.));
  v = Vec4<double>(3., 3., 3., 3.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(36.));
  v = Vec4<double>(-5., -5., 5., 5.);
  ASSERT_DOUBLE_EQ(v.length(), sqrt(100.));
}

TEST_F(Vector4, MultipliesVectorWithNumber) {
  v = Vec4<double>(1., 0., 5., -9.);
  v *= 5.;
  ASSERT_DOUBLE_EQ(v.x(), 5.);
  ASSERT_DOUBLE_EQ(v.y(), 0.);
  ASSERT_DOUBLE_EQ(v.z(), 25.);
  ASSERT_DOUBLE_EQ(v.w(), -45.);
}

TEST_F(Vector4, NormalizesVector) {
  v = Vec4<double>(4.53, 93.5, -56.3, -100.00001);
  ASSERT_DOUBLE_EQ(v.normalize().length(), 1.);
}

TEST_F(Vector4, DotProduct) {
  v = Vec4<double>(3., 3., 4., 1.);
  ASSERT_DOUBLE_EQ(dot(v, Vec4<double>(3., 3., 9., -10.)), 44.);
  v = Vec4<double>(-1., 5., 9., -3.);
  ASSERT_DOUBLE_EQ(dot(v, Vec4<double>(-3., 3., 6., 0.)), 72.);
}

TEST_F(Vector4, GetUnitVectorOf) {
  v = Vec4<double>(4.36, 7.62, 0.466, -30485.55555555);
  v = getUnitVectorOf(v);
  ASSERT_DOUBLE_EQ(v.length(), 1.);
}

TEST_F(Vector4, AddsTwoVectors) {
  v = Vec4<double>(4.532, 45.67, 0.83, -44.6);
  v = v + Vec4<double>(0.3456, 124.67, 1., 9.);
  ASSERT_DOUBLE_EQ(v.x(), 4.8776);
  ASSERT_DOUBLE_EQ(v.y(), 170.34);
  ASSERT_DOUBLE_EQ(v.z(), 1.83);
  ASSERT_DOUBLE_EQ(v.w(), -35.6);
}

TEST_F(Vector4, SubtractsTwoVectors) {
  v = Vec4<double>(40.54, 2.4, 0.62, 0.);
  v = v - Vec4<double>(4.20, -1.7, -1., -99.99999);
  ASSERT_DOUBLE_EQ(v[0], 36.34);
  ASSERT_DOUBLE_EQ(v[1], 4.1);
  ASSERT_DOUBLE_EQ(v[2], 1.62);
  ASSERT_DOUBLE_EQ(v[3], 99.99999);
}

TEST_F(Vector4, DevidesVectorByNumber) {
  v = Vec4<double>(36.6, -30.6, 120.2586, -0.5555555);
  v = v / 3.;
  ASSERT_DOUBLE_EQ(v.x(), 36.6 / 3.);
  ASSERT_DOUBLE_EQ(v.y(), -30.6 / 3.);
  ASSERT_DOUBLE_EQ(v.z(), 120.2586 / 3.);
  ASSERT_DOUBLE_EQ(v.w(), -0.5555555 / 3.);
}

TEST_F(Vector4, DevidesVectorByVector) {
  v = Vec4<double>(434.5, 93.5, 3858.53, -0.99999999);
  v = v / Vec4<double>(32.5, -16.2, 0.567, -999.99999999);
  ASSERT_DOUBLE_EQ(v.x(), 434.5 / 32.5);
  ASSERT_DOUBLE_EQ(v.y(), -93.5 / 16.2);
  ASSERT_DOUBLE_EQ(v.z(), 3858.53 / 0.567);
  ASSERT_DOUBLE_EQ(v[3], -0.99999999 / (-999.99999999));
}

TEST_F(Vector4, TestsEquality) {
  v = Vec4<double>(4.2, -6.54, 34855.38596, -0.9938375);
  ASSERT_TRUE(v == Vec4<double>(4.2, -6.54, 34855.38596, -0.9938375));
}

TEST_F(Vector4, TestsInequality) {
  v = Vec4<double>(4.2, -6.54, 0.0, 100.);
  ASSERT_TRUE(v != Vec4<double>(4.2, -4.36, 0.0, 100.));
}

//--------------------------------------------
//     MAT2
//--------------------------------------------

class Matrix2 : public Test {
 public:
  Mat2<double> m;
};

TEST_F(Matrix2, CreatesMatrix) {
  Vec2<double> v1(0., 0.);
  Vec2<double> v2(1., -1.);

  m = Mat2<double>(v1, v2);
  ASSERT_DOUBLE_EQ(m[0][0], v1[0]);
  ASSERT_DOUBLE_EQ(m[0][1], v1[1]);
  ASSERT_DOUBLE_EQ(m[1][0], v2[0]);
  ASSERT_DOUBLE_EQ(m[1][1], v2[1]);

  m = Mat2<double>(3.14);
  ASSERT_DOUBLE_EQ(m[0][0], 3.14);
  ASSERT_DOUBLE_EQ(m[0][1], 3.14);
  ASSERT_DOUBLE_EQ(m[1][0], 3.14);
  ASSERT_DOUBLE_EQ(m[1][1], 3.14);
}

TEST_F(Matrix2, AddsAmatrixOrNumber) {
  m = Mat2<double>(0.);
  m += Mat2<double>(-1.);
  ASSERT_DOUBLE_EQ(m[0][0], -1.);
  ASSERT_DOUBLE_EQ(m[0][1], -1.);
  ASSERT_DOUBLE_EQ(m[1][0], -1.);
  ASSERT_DOUBLE_EQ(m[1][1], -1.);
  m += 5.05;
  ASSERT_DOUBLE_EQ(m[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m[1][1], 4.05);
}

TEST_F(Matrix2, SubtractsAmatrixOrNumber) {
  m = Mat2<double>(0.);
  m -= Mat2<double>(1.);
  ASSERT_DOUBLE_EQ(m[0][0], -1.);
  ASSERT_DOUBLE_EQ(m[0][1], -1.);
  ASSERT_DOUBLE_EQ(m[1][0], -1.);
  ASSERT_DOUBLE_EQ(m[1][1], -1.);
  m -= -5.05;
  ASSERT_DOUBLE_EQ(m[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m[1][1], 4.05);
}

TEST_F(Matrix2, MultipliesWithNumber) {
  m = Mat2<double>(6.28);
  m *= 0.;
  ASSERT_DOUBLE_EQ(m[0][0], 0.);
  ASSERT_DOUBLE_EQ(m[0][1], 0.);
  ASSERT_DOUBLE_EQ(m[1][0], 0.);
  ASSERT_DOUBLE_EQ(m[1][1], 0.);
}

TEST_F(Matrix2, GetsTheDeterminant) {
  m = Mat2<double>(Vec2<double>(5.36, 2.28), Vec2<double>(-1.5, 85.));
  ASSERT_DOUBLE_EQ(m.determinant(), 459.02);
}

TEST_F(Matrix2, AddsTwoMatrices) {
  m = Mat2<double>(Vec2<double>(1.36, 1.28), Vec2<double>(1.5, 0.));
  m = m + Mat2<double>(Vec2<double>(9., -5.8), Vec2<double>(-6.5, 5.3));
  ASSERT_DOUBLE_EQ(m[0][0], 10.36);
  ASSERT_DOUBLE_EQ(m[0][1], -4.52);
  ASSERT_DOUBLE_EQ(m[1][0], -5.);
  ASSERT_DOUBLE_EQ(m[1][1], 5.3);
}

TEST_F(Matrix2, MultipliesTwoMatrices) {
  m = Mat2<double>(Vec2<double>(1.36, 1.28), Vec2<double>(1.5, 0.));
  m = m * Mat2<double>(Vec2<double>(9., -5.8), Vec2<double>(-6.5, 5.3));
  ASSERT_DOUBLE_EQ(m[0][0], 3.92);
  ASSERT_DOUBLE_EQ(m[0][1], -1.104);
  ASSERT_DOUBLE_EQ(m[1][0], 13.5);
  ASSERT_DOUBLE_EQ(m[1][1], -8.7);
}

//--------------------------------------------
//     MAT3
//--------------------------------------------

class Matrix3 : public Test {
 public:
  Mat3<double> m;
};

TEST_F(Matrix3, CreatesMatrix) {
  Vec3<double> v1(0., 0., 0.);
  Vec3<double> v2(1., -1., 1.);
  Vec3<double> v3(5., -9., 5.);

  m = Mat3<double>(v1, v2, v3);
  ASSERT_DOUBLE_EQ(m[0][0], v1[0]);
  ASSERT_DOUBLE_EQ(m[0][1], v1[1]);
  ASSERT_DOUBLE_EQ(m[0][2], v1[2]);

  ASSERT_DOUBLE_EQ(m[1][0], v2[0]);
  ASSERT_DOUBLE_EQ(m[1][1], v2[1]);
  ASSERT_DOUBLE_EQ(m[1][2], v2[2]);

  ASSERT_DOUBLE_EQ(m[2][0], v3[0]);
  ASSERT_DOUBLE_EQ(m[2][1], v3[1]);
  ASSERT_DOUBLE_EQ(m[2][2], v3[2]);

  m = Mat3<double>(3.14);
  ASSERT_DOUBLE_EQ(m[0][0], 3.14);
  ASSERT_DOUBLE_EQ(m[0][1], 3.14);
  ASSERT_DOUBLE_EQ(m[0][2], 3.14);

  ASSERT_DOUBLE_EQ(m[1][0], 3.14);
  ASSERT_DOUBLE_EQ(m[1][1], 3.14);
  ASSERT_DOUBLE_EQ(m[1][2], 3.14);

  ASSERT_DOUBLE_EQ(m[2][0], 3.14);
  ASSERT_DOUBLE_EQ(m[2][1], 3.14);
  ASSERT_DOUBLE_EQ(m[2][2], 3.14);
}

TEST_F(Matrix3, AddsAmatrixOrNumber) {
  m = Mat3<double>(0.);
  m += Mat3<double>(-1.);
  ASSERT_DOUBLE_EQ(m[0][0], -1.);
  ASSERT_DOUBLE_EQ(m[0][1], -1.);
  ASSERT_DOUBLE_EQ(m[0][2], -1.);

  ASSERT_DOUBLE_EQ(m[1][0], -1.);
  ASSERT_DOUBLE_EQ(m[1][1], -1.);
  ASSERT_DOUBLE_EQ(m[1][2], -1.);

  ASSERT_DOUBLE_EQ(m[2][0], -1.);
  ASSERT_DOUBLE_EQ(m[2][1], -1.);
  ASSERT_DOUBLE_EQ(m[2][2], -1.);
  m += 5.05;
  ASSERT_DOUBLE_EQ(m[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m[0][2], 4.05);

  ASSERT_DOUBLE_EQ(m[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m[1][1], 4.05);
  ASSERT_DOUBLE_EQ(m[1][2], 4.05);

  ASSERT_DOUBLE_EQ(m[2][0], 4.05);
  ASSERT_DOUBLE_EQ(m[2][1], 4.05);
  ASSERT_DOUBLE_EQ(m[2][2], 4.05);
}

TEST_F(Matrix3, SubtractsAmatrixOrNumber) {
  m = Mat3<double>(0.);
  m -= Mat3<double>(1.);
  ASSERT_DOUBLE_EQ(m[0][0], -1.);
  ASSERT_DOUBLE_EQ(m[0][1], -1.);
  ASSERT_DOUBLE_EQ(m[0][2], -1.);
  ASSERT_DOUBLE_EQ(m[1][0], -1.);
  ASSERT_DOUBLE_EQ(m[1][1], -1.);
  ASSERT_DOUBLE_EQ(m[1][2], -1.);
  ASSERT_DOUBLE_EQ(m[2][0], -1.);
  ASSERT_DOUBLE_EQ(m[2][1], -1.);
  ASSERT_DOUBLE_EQ(m[2][2], -1.);
  m -= -5.05;
  ASSERT_DOUBLE_EQ(m[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m[0][2], 4.05);
  ASSERT_DOUBLE_EQ(m[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m[1][1], 4.05);
  ASSERT_DOUBLE_EQ(m[1][2], 4.05);
  ASSERT_DOUBLE_EQ(m[2][0], 4.05);
  ASSERT_DOUBLE_EQ(m[2][1], 4.05);
  ASSERT_DOUBLE_EQ(m[2][2], 4.05);
}

TEST_F(Matrix3, MultipliesWithNumber) {
  m = Mat3<double>(6.28);
  m *= 0.;
  ASSERT_DOUBLE_EQ(m[0][0], 0.);
  ASSERT_DOUBLE_EQ(m[0][1], 0.);
  ASSERT_DOUBLE_EQ(m[0][2], 0.);
  ASSERT_DOUBLE_EQ(m[1][0], 0.);
  ASSERT_DOUBLE_EQ(m[1][1], 0.);
  ASSERT_DOUBLE_EQ(m[1][2], 0.);
  ASSERT_DOUBLE_EQ(m[2][0], 0.);
  ASSERT_DOUBLE_EQ(m[2][1], 0.);
  ASSERT_DOUBLE_EQ(m[2][2], 0.);
}

TEST_F(Matrix3, GetsTheDeterminant) {
  m = Mat3<double>(Vec3<double>(5.36, 2.28, 0.93), Vec3<double>(-1.5, 85., 0.),
                   Vec3<double>(45.8, -50., 0.14));
  ASSERT_DOUBLE_EQ(m.determinant(), -3486.4772);
}

TEST_F(Matrix3, AddsTwoMatrices) {
  m = Mat3<double>(Vec3<double>(1.36, 1.28, 1.), Vec3<double>(1.5, 0., -1.),
                   Vec3<double>(6.3, 0.5, 0.8));
  m = m + Mat3<double>(Vec3<double>(9., -5.8, 0.),
                       Vec3<double>(-6.5, 5.3, -1.5),
                       Vec3<double>(-5., 0.4, 90.));
  ASSERT_DOUBLE_EQ(m[0][0], 10.36);
  ASSERT_DOUBLE_EQ(m[0][1], -4.52);
  ASSERT_DOUBLE_EQ(m[0][2], 1.);

  ASSERT_DOUBLE_EQ(m[1][0], -5.);
  ASSERT_DOUBLE_EQ(m[1][1], 5.3);
  ASSERT_DOUBLE_EQ(m[1][2], -2.5);

  ASSERT_DOUBLE_EQ(m[2][0], 1.3);
  ASSERT_DOUBLE_EQ(m[2][1], 0.9);
  ASSERT_DOUBLE_EQ(m[2][2], 90.8);
}

TEST_F(Matrix3, MultipliesTwoMatrices) {
  m = Mat3<double>(Vec3<double>(1.36, 1.28, 0.85), Vec3<double>(1.5, 0., -6.58),
                   Vec3<double>(4.5, 0., -3.));
  Mat3<double> m2(
      Vec3<double>(9., -5.8, -4.5), Vec3<double>(-6.5, 5.3, 20.),
      Vec3<double>(8.6, 33.,
                   -5.));  // With an anonymous object, it doesn't pass
  m = m * m2;

  ASSERT_DOUBLE_EQ(m[0][0], 11.23);
  ASSERT_DOUBLE_EQ(m[0][1], 26.946);
  ASSERT_DOUBLE_EQ(m[0][2], 15.23);

  ASSERT_DOUBLE_EQ(m[1][0], -43.088);
  ASSERT_DOUBLE_EQ(m[1][1], -225.84);
  ASSERT_DOUBLE_EQ(m[1][2], 26.15);

  ASSERT_DOUBLE_EQ(m[2][0], 14.7);
  ASSERT_DOUBLE_EQ(m[2][1], -125.1);
  ASSERT_DOUBLE_EQ(m[2][2], -5.25);
}

TEST_F(Matrix3, GetTransposeOfMatrix) {
  m = Mat3<double>(Vec3<double>(1.36, 1.28, 0.85), Vec3<double>(1.5, 0., -6.58),
                   Vec3<double>(4.5, 0., -3.));

  Mat3<double> mt = m.transpose();
  ASSERT_DOUBLE_EQ(mt[0][0], 1.36);
  ASSERT_DOUBLE_EQ(mt[0][1], 1.5);
  ASSERT_DOUBLE_EQ(mt[0][2], 4.5);

  ASSERT_DOUBLE_EQ(mt[1][0], 1.28);
  ASSERT_DOUBLE_EQ(mt[1][1], 0.);
  ASSERT_DOUBLE_EQ(mt[1][2], 0.);

  ASSERT_DOUBLE_EQ(mt[2][0], 0.85);
  ASSERT_DOUBLE_EQ(mt[2][1], -6.58);
  ASSERT_DOUBLE_EQ(mt[2][2], -3.);
}

TEST_F(Matrix3, GetInverseOfMatrix) {
  m = Mat3<double>(Vec3<double>(1.36, 1.28, 0.85), Vec3<double>(1.5, 0., -6.58),
                   Vec3<double>(4.5, 0., -3.));

  m = m.inverse();

  ASSERT_DOUBLE_EQ(m[0][0], 0.);
  ASSERT_DOUBLE_EQ(m[0][1], -0.11947431302270011946);
  ASSERT_DOUBLE_EQ(m[0][2], 0.26204699322978892872);

  ASSERT_DOUBLE_EQ(m[1][0], 0.78125);
  ASSERT_DOUBLE_EQ(m[1][1], 0.24594907407407407407);
  ASSERT_DOUBLE_EQ(m[1][2], -0.31809413580246913581);

  ASSERT_DOUBLE_EQ(m[2][0], 0.);
  ASSERT_DOUBLE_EQ(m[2][1], -0.17921146953405017921);
  ASSERT_DOUBLE_EQ(m[2][2], 0.059737156511350059737);
}

//--------------------------------------------
//     MAT4
//--------------------------------------------

class Matrix4 : public Test {
 public:
  Mat4<double> m4;
};

TEST_F(Matrix4, CreatesMatrix) {
  Vec4<double> v1(0., 0., 0., 0.);
  Vec4<double> v2(1., -1., 1., -1.);
  Vec4<double> v3(5., -9., 5., 9.);
  Vec4<double> v4(0., -1., 0., 0.);

  m4 = Mat4<double>(v1, v2, v3, v4);
  ASSERT_DOUBLE_EQ(m4[0][0], v1[0]);
  ASSERT_DOUBLE_EQ(m4[0][1], v1[1]);
  ASSERT_DOUBLE_EQ(m4[0][2], v1[2]);
  ASSERT_DOUBLE_EQ(m4[0][3], v1[3]);

  ASSERT_DOUBLE_EQ(m4[1][0], v2[0]);
  ASSERT_DOUBLE_EQ(m4[1][1], v2[1]);
  ASSERT_DOUBLE_EQ(m4[1][2], v2[2]);
  ASSERT_DOUBLE_EQ(m4[1][3], v2[3]);

  ASSERT_DOUBLE_EQ(m4[2][0], v3[0]);
  ASSERT_DOUBLE_EQ(m4[2][1], v3[1]);
  ASSERT_DOUBLE_EQ(m4[2][2], v3[2]);
  ASSERT_DOUBLE_EQ(m4[2][3], v3[3]);

  ASSERT_DOUBLE_EQ(m4[3][0], v4[0]);
  ASSERT_DOUBLE_EQ(m4[3][1], v4[1]);
  ASSERT_DOUBLE_EQ(m4[3][2], v4[2]);
  ASSERT_DOUBLE_EQ(m4[3][3], v4[3]);

  m4 = Mat4<double>(3.14);
  ASSERT_DOUBLE_EQ(m4[0][0], 3.14);
  ASSERT_DOUBLE_EQ(m4[0][1], 3.14);
  ASSERT_DOUBLE_EQ(m4[0][2], 3.14);
  ASSERT_DOUBLE_EQ(m4[0][3], 3.14);

  ASSERT_DOUBLE_EQ(m4[1][0], 3.14);
  ASSERT_DOUBLE_EQ(m4[1][1], 3.14);
  ASSERT_DOUBLE_EQ(m4[1][2], 3.14);
  ASSERT_DOUBLE_EQ(m4[1][3], 3.14);

  ASSERT_DOUBLE_EQ(m4[2][0], 3.14);
  ASSERT_DOUBLE_EQ(m4[2][1], 3.14);
  ASSERT_DOUBLE_EQ(m4[2][2], 3.14);
  ASSERT_DOUBLE_EQ(m4[2][3], 3.14);

  ASSERT_DOUBLE_EQ(m4[3][0], 3.14);
  ASSERT_DOUBLE_EQ(m4[3][1], 3.14);
  ASSERT_DOUBLE_EQ(m4[3][2], 3.14);
  ASSERT_DOUBLE_EQ(m4[3][3], 3.14);
}

TEST_F(Matrix4, AddsAmatrixOrNumber) {
  m4 = Mat4<double>(0.);
  m4 += Mat4<double>(-1.);
  ASSERT_DOUBLE_EQ(m4[0][0], -1.);
  ASSERT_DOUBLE_EQ(m4[0][1], -1.);
  ASSERT_DOUBLE_EQ(m4[0][2], -1.);
  ASSERT_DOUBLE_EQ(m4[0][3], -1.);

  ASSERT_DOUBLE_EQ(m4[1][0], -1.);
  ASSERT_DOUBLE_EQ(m4[1][1], -1.);
  ASSERT_DOUBLE_EQ(m4[1][2], -1.);
  ASSERT_DOUBLE_EQ(m4[1][3], -1.);

  ASSERT_DOUBLE_EQ(m4[2][0], -1.);
  ASSERT_DOUBLE_EQ(m4[2][1], -1.);
  ASSERT_DOUBLE_EQ(m4[2][2], -1.);
  ASSERT_DOUBLE_EQ(m4[2][3], -1.);

  ASSERT_DOUBLE_EQ(m4[3][0], -1.);
  ASSERT_DOUBLE_EQ(m4[3][1], -1.);
  ASSERT_DOUBLE_EQ(m4[3][2], -1.);
  ASSERT_DOUBLE_EQ(m4[3][3], -1.);
  m4 += 5.05;
  ASSERT_DOUBLE_EQ(m4[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[2][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[3][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][3], 4.05);
}

TEST_F(Matrix4, SubtractsAmatrixOrNumber) {
  m4 = Mat4<double>(0.);
  m4 -= Mat4<double>(1.);
  ASSERT_DOUBLE_EQ(m4[0][0], -1.);
  ASSERT_DOUBLE_EQ(m4[0][1], -1.);
  ASSERT_DOUBLE_EQ(m4[0][2], -1.);
  ASSERT_DOUBLE_EQ(m4[0][3], -1.);

  ASSERT_DOUBLE_EQ(m4[1][0], -1.);
  ASSERT_DOUBLE_EQ(m4[1][1], -1.);
  ASSERT_DOUBLE_EQ(m4[1][2], -1.);
  ASSERT_DOUBLE_EQ(m4[1][3], -1.);

  ASSERT_DOUBLE_EQ(m4[2][0], -1.);
  ASSERT_DOUBLE_EQ(m4[2][1], -1.);
  ASSERT_DOUBLE_EQ(m4[2][2], -1.);
  ASSERT_DOUBLE_EQ(m4[2][3], -1.);

  ASSERT_DOUBLE_EQ(m4[3][0], -1.);
  ASSERT_DOUBLE_EQ(m4[3][1], -1.);
  ASSERT_DOUBLE_EQ(m4[3][2], -1.);
  ASSERT_DOUBLE_EQ(m4[3][3], -1.);
  m4 -= -5.05;
  ASSERT_DOUBLE_EQ(m4[0][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[0][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[1][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[1][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[2][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[2][3], 4.05);

  ASSERT_DOUBLE_EQ(m4[3][0], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][1], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][2], 4.05);
  ASSERT_DOUBLE_EQ(m4[3][3], 4.05);
}

TEST_F(Matrix4, MultipliesWithNumber) {
  m4 = Mat4<double>(6.28);
  m4 *= 0.;
  ASSERT_DOUBLE_EQ(m4[0][0], 0.);
  ASSERT_DOUBLE_EQ(m4[0][1], 0.);
  ASSERT_DOUBLE_EQ(m4[0][2], 0.);
  ASSERT_DOUBLE_EQ(m4[0][3], 0.);

  ASSERT_DOUBLE_EQ(m4[1][0], 0.);
  ASSERT_DOUBLE_EQ(m4[1][1], 0.);
  ASSERT_DOUBLE_EQ(m4[1][2], 0.);
  ASSERT_DOUBLE_EQ(m4[1][3], 0.);

  ASSERT_DOUBLE_EQ(m4[2][0], 0.);
  ASSERT_DOUBLE_EQ(m4[2][1], 0.);
  ASSERT_DOUBLE_EQ(m4[2][2], 0.);
  ASSERT_DOUBLE_EQ(m4[2][3], 0.);

  ASSERT_DOUBLE_EQ(m4[3][0], 0.);
  ASSERT_DOUBLE_EQ(m4[3][1], 0.);
  ASSERT_DOUBLE_EQ(m4[3][2], 0.);
  ASSERT_DOUBLE_EQ(m4[3][3], 0.);
}

TEST_F(Matrix4, GetsTheDeterminant) {
  m4 = Mat4<double>(
      Vec4<double>(5.36, 2.28, 0.93, 13.), Vec4<double>(-1.5, 85., 0., 5.),
      Vec4<double>(45.8, -50., 0.14, -1.5), Vec4<double>(0.48, -38.5, 0., -1.));
  ASSERT_DOUBLE_EQ(m4.determinant(), -4401.55547);
}

TEST_F(Matrix4, AddsTwoMatrices) {
  m4 = Mat4<double>(
      Vec4<double>(1.36, 1.28, 1., 0.), Vec4<double>(1.5, 0., -1., -2.),
      Vec4<double>(6.3, 0.5, 0.8, -6.), Vec4<double>(0.5, 3.38, -2., 5.));

  m4 = m4 + Mat4<double>(Vec4<double>(9., -5.8, 0., 0.),
                         Vec4<double>(-6.5, 5.3, -1.5, -2.5),
                         Vec4<double>(-5., 0.4, 90., 3.5),
                         Vec4<double>(5., 0.4, 0.5, 3.5));

  ASSERT_DOUBLE_EQ(m4[0][0], 10.36);
  ASSERT_DOUBLE_EQ(m4[0][1], -4.52);
  ASSERT_DOUBLE_EQ(m4[0][2], 1.);
  ASSERT_DOUBLE_EQ(m4[0][3], 0.);

  ASSERT_DOUBLE_EQ(m4[1][0], -5.);
  ASSERT_DOUBLE_EQ(m4[1][1], 5.3);
  ASSERT_DOUBLE_EQ(m4[1][2], -2.5);
  ASSERT_DOUBLE_EQ(m4[1][3], -4.5);

  ASSERT_DOUBLE_EQ(m4[2][0], 1.3);
  ASSERT_DOUBLE_EQ(m4[2][1], 0.9);
  ASSERT_DOUBLE_EQ(m4[2][2], 90.8);
  ASSERT_DOUBLE_EQ(m4[2][3], -2.5);

  ASSERT_DOUBLE_EQ(m4[3][0], 5.5);
  ASSERT_DOUBLE_EQ(m4[3][1], 3.78);
  ASSERT_DOUBLE_EQ(m4[3][2], -1.5);
  ASSERT_DOUBLE_EQ(m4[3][3], 8.5);
}

TEST_F(Matrix4, MultipliesTwoMatrices) {
  m4 = Mat4<double>(
      Vec4<double>(1.36, 1.28, 0.85, -1.), Vec4<double>(1.5, 0., -6.58, 0.),
      Vec4<double>(4.5, 0., -3., 0.), Vec4<double>(5.8, -3.4, 0., -1));
  Mat4<double> m2(
      Vec4<double>(9., -5.8, -4.5, -1.1), Vec4<double>(-6.5, 5.3, 20., 2.2),
      Vec4<double>(8.6, 33., -5., -4.),
      Vec4<double>(0., -5., 8.,
                   -3.));  // With an anonymous object, it doesn't pass
  m4 = m4 * m2;

  ASSERT_DOUBLE_EQ(m4[0][0], 11.23);
  ASSERT_DOUBLE_EQ(m4[0][1], 31.946);
  ASSERT_DOUBLE_EQ(m4[0][2], 7.23);
  ASSERT_DOUBLE_EQ(m4[0][3], 0.92);

  ASSERT_DOUBLE_EQ(m4[1][0], -43.088);
  ASSERT_DOUBLE_EQ(m4[1][1], -225.84);
  ASSERT_DOUBLE_EQ(m4[1][2], 26.15);
  ASSERT_DOUBLE_EQ(m4[1][3], 24.67);

  ASSERT_DOUBLE_EQ(m4[2][0], 14.7);
  ASSERT_DOUBLE_EQ(m4[2][1], -125.1);
  ASSERT_DOUBLE_EQ(m4[2][2], -5.25);
  ASSERT_DOUBLE_EQ(m4[2][3], 7.05);

  ASSERT_DOUBLE_EQ(m4[3][0], 74.3);
  ASSERT_DOUBLE_EQ(m4[3][1], -46.66);
  ASSERT_DOUBLE_EQ(m4[3][2], -102.1);
  ASSERT_DOUBLE_EQ(m4[3][3], -10.86);
}

TEST_F(Matrix4, GetTransposeOfMatrix) {
  m4 = Mat4<double>(
      Vec4<double>(1.36, 1.28, 0.85, -1.), Vec4<double>(1.5, 0., -6.58, -5.),
      Vec4<double>(4.5, 0., -3., 0.), Vec4<double>(3., 4., 5., 6.));

  Mat4<double> mt = m4.transpose();
  ASSERT_DOUBLE_EQ(mt[0][0], 1.36);
  ASSERT_DOUBLE_EQ(mt[0][1], 1.5);
  ASSERT_DOUBLE_EQ(mt[0][2], 4.5);
  ASSERT_DOUBLE_EQ(mt[0][3], 3.);

  ASSERT_DOUBLE_EQ(mt[1][0], 1.28);
  ASSERT_DOUBLE_EQ(mt[1][1], 0.);
  ASSERT_DOUBLE_EQ(mt[1][2], 0.);
  ASSERT_DOUBLE_EQ(mt[1][3], 4.);

  ASSERT_DOUBLE_EQ(mt[2][0], 0.85);
  ASSERT_DOUBLE_EQ(mt[2][1], -6.58);
  ASSERT_DOUBLE_EQ(mt[2][2], -3.);
  ASSERT_DOUBLE_EQ(mt[2][3], 5.);

  ASSERT_DOUBLE_EQ(mt[3][0], -1.);
  ASSERT_DOUBLE_EQ(mt[3][1], -5.);
  ASSERT_DOUBLE_EQ(mt[3][2], 0.);
  ASSERT_DOUBLE_EQ(mt[3][3], 6.);
}

TEST_F(Matrix4, GetInverseOfMatrix) {
  m4 = Mat4<double>(
      Vec4<double>(1.36, 1.28, 0.85, -7.), Vec4<double>(1.5, 0., -6.58, 1.),
      Vec4<double>(4.5, 0., -3., 10.), Vec4<double>(0., 1., 6.68, -9.));

  m4 = m4.inverse();

  ASSERT_DOUBLE_EQ(m4[0][0], -0.57644611789241018751);
  ASSERT_DOUBLE_EQ(m4[0][1], 0.58235744432430050861);
  ASSERT_DOUBLE_EQ(m4[0][2], 0.20231790085493935382);
  ASSERT_DOUBLE_EQ(m4[0][3], 0.73785103090228503992);

  ASSERT_DOUBLE_EQ(m4[1][0], 2.7182004600554049173);
  ASSERT_DOUBLE_EQ(m4[1][1], -2.1123170380215781781);
  ASSERT_DOUBLE_EQ(m4[1][2], -0.1173949041428852046);
  ASSERT_DOUBLE_EQ(m4[1][3], -2.4792965888709182942);

  ASSERT_DOUBLE_EQ(m4[2][0], -0.096380322259081321122);
  ASSERT_DOUBLE_EQ(m4[2][1], -0.06186698781201981947);
  ASSERT_DOUBLE_EQ(m4[2][2], 0.049750604442306739096);
  ASSERT_DOUBLE_EQ(m4[2][3], 0.12336681249162409103);

  ASSERT_DOUBLE_EQ(m4[3][0], 0.23048665637386018803);
  ASSERT_DOUBLE_EQ(m4[3][1], -0.28062094628954117469);
  ASSERT_DOUBLE_EQ(m4[3][2], 0.023882125947969312508);
  ASSERT_DOUBLE_EQ(m4[3][3], -0.29502292015854104069);
}

//--------------------------------------------
//     Point3
//--------------------------------------------

class point3 : public Test {
 public:
  Point3i p;
};

TEST_F(point3, createsPoint) {
  p = Point3i();
  ASSERT_EQ(p.x(), 0);
  ASSERT_EQ(p.y(), 0);
  ASSERT_EQ(p.z(), 0);
  p = Point3i(1, -8);
  ASSERT_EQ(p.x(), 1);
  ASSERT_EQ(p.y(), -8);
  ASSERT_EQ(p.z(), 0);
  p = Point3i(9);
  ASSERT_EQ(p.x(), 9);
  ASSERT_EQ(p.y(), 0);
  ASSERT_EQ(p.z(), 0);
}

TEST_F(point3, subtractsPointFromPoint) {
  p = Point3i(1, 0, 4);
  Point3i p1 = Point3i(0, 2, 4);
  Vec3i v = p1 - p;
  ASSERT_EQ(v.x(), -1);
  ASSERT_EQ(v.y(), 2);
  ASSERT_EQ(v.z(), 0);
  ASSERT_FLOAT_EQ(v.length(), sqrt(5));
}

TEST_F(point3, subtractsVectorFromPoint) {
  p = Point3i(3, 4, 5);
  Vec3i v = Vec3i(3, 4, 5);
  p -= v;
  ASSERT_EQ(p.x(), 0);
  ASSERT_EQ(p.y(), 0);
  ASSERT_EQ(p.z(), 0);
}

TEST_F(point3, addsVectorToPoint) {
  p = Point3i(0, -1, -8);
  Vec3i v = Vec3i(-4, 5, 9);
  p += v;
  ASSERT_EQ(p.x(), -4);
  ASSERT_EQ(p.y(), 4);
  ASSERT_EQ(p.z(), 1);
}
