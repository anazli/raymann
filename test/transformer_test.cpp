#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "geometry/sphere.h"
#include "transformations/transformation.h"
#include "utils.h"

using testing::Eq;

class TransformerTest : public testing::Test {
 public:
  Mat4D m;
  Vec3f v;
  Point3f p;
  float eps = 1.E-4f;
};

/**********************************
 *       Translation
 *********************************/

TEST_F(TransformerTest, appliesTranslationToPoint) {  // TODO: better interface
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(2.f, 1.f, 7.f)));
}

TEST_F(TransformerTest, appliesTranslationToPointOverloaded) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  Vec3f trans(5.0f, -3.0f, 2.0f);
  m = translation(trans);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(2.f, 1.f, 7.f)));
}

TEST_F(TransformerTest, appliesInverseTranslationToPoint) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  m = m.inverse();
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(-8.f, 7.f, 3.f)));
}

TEST_F(TransformerTest, appliesTranslationToVector) {
  v = Vec3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(v);

  v4 = m * v4;
  auto v3 = Vec3(v4);

  ASSERT_THAT(v, Eq(v3));  // translation does not affect vectors
}

/**********************************
 *       Scaling
 *********************************/

TEST_F(TransformerTest, appliesScalingToPoint) {
  p = Point3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(-8.f, 18.f, 32.f)));
}

TEST_F(TransformerTest, appliesScalingToVector) {
  v = Vec3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_THAT(v, Eq(Vec3f(-8.f, 18.f, 32.f)));
}

TEST_F(TransformerTest,
       appliesInverseScalingToVector) {  // TODO: It doesn't pass with
  v = Vec3f(-4.0f, 6.0f, 8.0f);          // integers
  m = scale(2.0f, 3.0f, 4.0f);
  m = m.inverse();
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_THAT(v, Eq(Vec3f(-2.f, 2.f, 2.f)));
}

/**********************************
 *       Rotation
 *********************************/

TEST_F(TransformerTest, appliesRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationOverX(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(0.f, sqrt(2.) / 2.f, sqrt(2.) / 2.f)));
}

TEST_F(TransformerTest, appliesInverseRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationOverX(PI / 4.0f);
  m = m.inverse();
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Vec3Near(Point3f(0.f, sqrt(2.) / 2.f, -sqrt(2.) / 2.f), eps));
}

TEST_F(TransformerTest, appliesRotationYToPoint) {
  p = Point3f(0.0f, 0.0f, 1.0f);
  m = rotationOverY(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(sqrt(2.) / 2.f, 0.f, sqrt(2.) / 2.f)));
}

TEST_F(TransformerTest, appliesRotationZToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationOverZ(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(-sqrt(2.) / 2.f, sqrt(2.) / 2.f, 0.f)));
}

TEST_F(TransformerTest, appliesTransformationsInSequence) {
  p = Point3f(1.0f, 0.0f, 1.0f);

  Mat4D A = rotationOverX(PI / 2.0f);
  Mat4D B = scale(5.0f, 5.0f, 5.0f);
  Mat4D C = translation(10.0f, 5.0f, 7.0f);
  Vec4f v4(p);
  v4 = A * v4;
  Point3f p1(v4);

  Vec4f v41(p1);
  v41 = B * v41;
  Point3f p2(v41);

  Vec4f v42(p2);
  v41 = C * v41;
  Point3f p3(v41);

  ASSERT_THAT(p1, Vec3Near(Vec3f(1.f, -1.f, 0.f), eps));
  ASSERT_THAT(p2, Vec3Near(Vec3f(5.f, -5.f, 0.f), eps));
  ASSERT_THAT(p3, Eq(Point3f(15.f, 0.f, 7.f)));
}

TEST_F(TransformerTest, appliesTransformationChaining) {
  p = Point3f(1.0f, 0.0f, 1.0f);
  Mat4D A = rotationOverX(PI / 2.0f);
  Mat4D B = scale(5.0f, 5.0f, 5.0f);
  Mat4D C = translation(10.0f, 5.0f, 7.0f);
  m = C * B * A;
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_THAT(p, Eq(Point3f(15.f, 0.f, 7.f)));
}

TEST_F(TransformerTest, computesNormalOfTranslatedSphere) {
  PrimitivePtr s = Sphere::create();
  auto t = Transformation(translation(0.0f, 1.0f, 0.0f));
  s->setTransformation(t);
  auto norm = s->normal(Point3f(0.0f, 1.70711f, -0.70711));

  Vec3f tn(0.0f, 0.70711f, -0.70711f);

  float eps = 1.E-5;
  ASSERT_THAT(norm, Vec3Near(tn, eps));
}

TEST_F(TransformerTest, computesNormalOfRotatedSphere) {
  PrimitivePtr s = Sphere::create();
  auto t = Transformation(rotationOverZ(PI / 5.0f) * scale(1.0f, 0.5f, 1.0f));
  s->setTransformation(t);

  auto norm = s->normal(Point3f(0.0f, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f));
  Vec3f tn(0.0f, 0.97014f, -0.24254f);

  ASSERT_THAT(norm, Vec3Near(tn, eps));
}

TEST_F(TransformerTest, transformationMatrixForDefaultOrientation) {
  Point3f from = Point3f(0.0f, 0.0f, 0.0f);
  Point3f to = Point3f(0.0f, 0.0f, -1.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4D m = view_transform(from, to, up);
  Mat4D t = Mat4D();
  t.identity();
  ASSERT_THAT(m, Eq(t));
}

TEST_F(TransformerTest, viewTransformationLookingInPositiveZ) {
  Point3f from = Point3f(0.0f, 0.0f, 0.0f);
  Point3f to = Point3f(0.0f, 0.0f, 1.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4D m = view_transform(from, to, up);
  ASSERT_THAT(m, Eq(scale(-1.0f, 1.0f, -1.0f)));
}

TEST_F(TransformerTest, viewTransformationMovesTheWorld) {
  Point3f from = Point3f(0.0f, 0.0f, 8.0f);
  Point3f to = Point3f(0.0f, 0.0f, 0.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4D m = view_transform(from, to, up);
  ASSERT_THAT(m, Eq(translation(0.0f, 0.0f, -8.0f)));
}

TEST_F(TransformerTest, ArbitraryViewTransformation) {
  Point3f from = Point3f(1.0f, 3.0f, 2.0f);
  Point3f to = Point3f(4.0f, -2.0f, 8.0f);
  Vec3f up = Vec3f(1.0f, 1.0f, 0.0f);

  Mat4D m = view_transform(from, to, up);
  Mat4D t(Vec4f(-0.50709, 0.50709, 0.67612, -2.36643),
          Vec4f(0.76772, 0.60609, 0.12122, -2.82843),
          Vec4f(-0.35857, 0.59761, -0.71714, 0.0f),
          Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

  EXPECT_NEAR(m[0][0], t[0][0], eps);
  EXPECT_NEAR(m[0][1], t[0][1], eps);
  EXPECT_NEAR(m[0][2], t[0][2], eps);
  EXPECT_NEAR(m[0][3], t[0][3], eps);
  EXPECT_NEAR(m[1][0], t[1][0], eps);
  EXPECT_NEAR(m[1][1], t[1][1], eps);
  EXPECT_NEAR(m[1][2], t[1][2], eps);
  EXPECT_NEAR(m[1][3], t[1][3], eps);
  EXPECT_NEAR(m[2][0], t[2][0], eps);
  EXPECT_NEAR(m[2][1], t[2][1], eps);
  EXPECT_NEAR(m[2][2], t[2][2], eps);
  EXPECT_NEAR(m[2][3], t[2][3], eps);
  EXPECT_NEAR(m[3][0], t[3][0], eps);
  EXPECT_NEAR(m[3][1], t[3][1], eps);
  EXPECT_NEAR(m[3][2], t[3][2], eps);
  EXPECT_NEAR(m[3][3], t[3][3], eps);
}
