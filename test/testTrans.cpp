#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class Ttrans : public Test {
 public:
  Mat4f m;
  Vec3f v;
  Point3f p;
};

/**********************************
 *       Translation
 *********************************/

TEST_F(Ttrans, appliesTranslationToPoint) {  // TODO: better interface
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = transl(5.0f, -3.0f, 2.0f);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 1.0f);
  ASSERT_FLOAT_EQ(p.z(), 7.0f);
}

TEST_F(Ttrans, appliesTranslationToPointOverloaded) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  Vec3f trans(5.0f, -3.0f, 2.0f);
  m = transl(trans);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 1.0f);
  ASSERT_FLOAT_EQ(p.z(), 7.0f);
}

TEST_F(Ttrans, appliesInverseTranslationToPoint) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = transl(5.0f, -3.0f, 2.0f);
  m = m.inverse();
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -8.0f);
  ASSERT_FLOAT_EQ(p.y(), 7.0f);
  ASSERT_FLOAT_EQ(p.z(), 3.0f);
}

TEST_F(Ttrans, appliesTranslationToVector) {
  v = Vec3f(-3.0f, 4.0f, 5.0f);
  m = transl(5.0f, -3.0f, 2.0f);
  Vec4f v4(v);

  v4 = m * v4;
  Vec3f v3 = v4;

  ASSERT_TRUE(v == v3);  // translation does not affect vectors
}

/**********************************
 *       Scaling
 *********************************/

TEST_F(Ttrans, appliesScalingToPoint) {
  p = Point3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -8.0f);
  ASSERT_FLOAT_EQ(p.y(), 18.0f);
  ASSERT_FLOAT_EQ(p.z(), 32.0f);
}

TEST_F(Ttrans, appliesScalingToVector) {
  v = Vec3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_FLOAT_EQ(v.x(), -8.0f);
  ASSERT_FLOAT_EQ(v.y(), 18.0f);
  ASSERT_FLOAT_EQ(v.z(), 32.0f);
}

TEST_F(Ttrans, appliesInverseScalingToVector) {  // TODO: It doesn't pass with
  v = Vec3f(-4.0f, 6.0f, 8.0f);                  // integers
  m = scale(2.0f, 3.0f, 4.0f);
  m = m.inverse();
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_FLOAT_EQ(v.x(), -2.0f);
  ASSERT_FLOAT_EQ(v.y(), 2.0f);
  ASSERT_FLOAT_EQ(v.z(), 2.0f);
}

/**********************************
 *       Rotation
 *********************************/

TEST_F(Ttrans, appliesRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotX(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 0.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesInverseRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotX(PI / 4.0f);
  m = m.inverse();
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 0.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), -sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesRotationYToPoint) {
  p = Point3f(0.0f, 0.0f, 1.0f);
  m = rotY(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 0.0f);
  ASSERT_FLOAT_EQ(p.z(), sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesRotationZToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotZ(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), 0.0f);
}

TEST_F(Ttrans, appliesTransformationsInSequence) {
  p = Point3f(1.0f, 0.0f, 1.0f);

  Mat4f A = rotX(PI / 2.0f);
  Mat4f B = scale(5.0f, 5.0f, 5.0f);
  Mat4f C = transl(10.0f, 5.0f, 7.0f);
  Vec4f v4(p);
  v4 = A * v4;
  Point3f p1(v4);

  ASSERT_FLOAT_EQ(p1.x(), 1.0f);
  ASSERT_FLOAT_EQ(p1.y(), -1.0f);
  EXPECT_NEAR(p1.z(), 0.0f, 4.4E-8);

  Vec4f v41(p1);
  v41 = B * v41;
  Point3f p2(v41);

  ASSERT_FLOAT_EQ(p2.x(), 5.0f);
  ASSERT_FLOAT_EQ(p2.y(), -5.0f);
  EXPECT_NEAR(p2.z(), 0.0f, 2.2E-7);

  Vec4f v42(p2);
  v41 = C * v41;
  Point3f p3(v41);

  ASSERT_FLOAT_EQ(p3.x(), 15.0f);
  ASSERT_FLOAT_EQ(p3.y(), 0.0f);
  ASSERT_FLOAT_EQ(p3.z(), 7.0f);
}

TEST_F(Ttrans, appliesTransformationChaining) {
  p = Point3f(1.0f, 0.0f, 1.0f);
  Mat4f A = rotX(PI / 2.0f);
  Mat4f B = scale(5.0f, 5.0f, 5.0f);
  Mat4f C = transl(10.0f, 5.0f, 7.0f);
  m = C * B * A;
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 15.0f);
  ASSERT_FLOAT_EQ(p.y(), 0.0f);
  ASSERT_FLOAT_EQ(p.z(), 7.0f);
}

TEST_F(Ttrans, computesNormalOfTranslatedSphere) {
  Traceable *s = new Sphere();
  Traceable *t = new Transformer(s, transl(0.0f, 1.0f, 0.0f));
  Vec3f norm = t->normal(Point3f(0.0f, 1.70711f, -0.70711));

  Vec3f tn(0.0f, 0.70711f, -0.70711f);

  float eps = 1.E-5;
  EXPECT_NEAR(norm.x(), tn.x(), eps);
  EXPECT_NEAR(norm.y(), tn.y(), eps);
  EXPECT_NEAR(norm.z(), tn.z(), eps);

  delete t;  // Note: when a Test doesn't pass, mem is not dealocated. Flow
             // stops above.
}

TEST_F(Ttrans, computesNormalOfRotatedSphere) {
  Traceable *s = new Sphere();
  Traceable *t = new Transformer(new Transformer(s, rotZ(PI / 5.0f)),
                                 scale(1.0f, 0.5f, 1.0f));

  Vec3f norm = t->normal(Point3f(0.0f, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f));
  Vec3f tn(0.0f, 0.97014f, -0.24254f);

  float eps = 1.E-5;
  EXPECT_NEAR(norm.x(), tn.x(), eps);
  EXPECT_NEAR(norm.y(), tn.y(), eps);
  EXPECT_NEAR(norm.z(), tn.z(), eps);

  delete t;
}

TEST_F(Ttrans, transformationMatrixForDefaultOrientation) {
  Point3f from = Point3f(0.0f, 0.0f, 0.0f);
  Point3f to = Point3f(0.0f, 0.0f, -1.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4f m = view_transform(from, to, up);
  Mat4f t = Mat4f();
  t.identity();
  ASSERT_TRUE(m == t);
}

TEST_F(Ttrans, viewTransformationLookingInPositiveZ) {
  Point3f from = Point3f(0.0f, 0.0f, 0.0f);
  Point3f to = Point3f(0.0f, 0.0f, 1.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4f m = view_transform(from, to, up);
  ASSERT_TRUE(m == scale(-1.0f, 1.0f, -1.0f));
}

TEST_F(Ttrans, viewTransformationMovesTheWorld) {
  Point3f from = Point3f(0.0f, 0.0f, 8.0f);
  Point3f to = Point3f(0.0f, 0.0f, 0.0f);
  Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);

  Mat4f m = view_transform(from, to, up);
  ASSERT_TRUE(m == transl(0.0f, 0.0f, -8.0f));
}

TEST_F(Ttrans, ArbitraryViewTransformation) {
  Point3f from = Point3f(1.0f, 3.0f, 2.0f);
  Point3f to = Point3f(4.0f, -2.0f, 8.0f);
  Vec3f up = Vec3f(1.0f, 1.0f, 0.0f);

  Mat4f m = view_transform(from, to, up);
  Mat4f t(Vec4f(-0.50709, 0.50709, 0.67612, -2.36643),
          Vec4f(0.76772, 0.60609, 0.12122, -2.82843),
          Vec4f(-0.35857, 0.59761, -0.71714, 0.0f),
          Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

  float eps = 10E-6;
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
