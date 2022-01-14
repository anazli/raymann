#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class TMat : public Test {
 public:
  Sphere *s;
  PointLight light;
};

TEST_F(TMat, createsDefaultLight) {
  ASSERT_TRUE(light.position() == Point3f());
  ASSERT_TRUE(light.intensity() == Vec3f());
}

TEST_F(TMat, createsNewLight1) {
  light.setPosition(Point3f(1.0f, 2.0f, 3.0f));
  light.setIntensity(Vec3f(0.1f, 0.1f, 0.3f));

  ASSERT_EQ(light.position().x(), 1.0f);
  ASSERT_EQ(light.position().y(), 2.0f);
  ASSERT_EQ(light.position().z(), 3.0f);

  ASSERT_EQ(light.intensity().x(), 0.1f);
  ASSERT_EQ(light.intensity().y(), 0.1f);
  ASSERT_EQ(light.intensity().z(), 0.3f);
}

TEST_F(TMat, createsNewLight2) {
  light = PointLight(Point3f(0.1f, -4.0f, -0.4f), Vec3f(1.0f, 4.0f, 0.0f));

  ASSERT_EQ(light.position().x(), 0.1f);
  ASSERT_EQ(light.position().y(), -4.0f);
  ASSERT_EQ(light.position().z(), -0.4f);

  ASSERT_EQ(light.intensity().x(), 1.0f);
  ASSERT_EQ(light.intensity().y(), 4.0f);
  ASSERT_EQ(light.intensity().z(), 0.0f);
}

TEST_F(TMat, lightsWithEyeBetweenLightAndSurface) {
  //----------------------------------
  // As it is in Material->lighting
  //----------------------------------

  Vec3f eye(0.0f, 0.0f, -1.0f);
  light = PointLight(Point3f(0.0f, 0.0f, -10.0f), Vec3f(1.0f));

  Point3f p(0.0f);
  Vec3f m_color(1.0f);
  Vec3f effective_color = m_color * light.intensity();
  Vec3f lightv = (light.position() - p).normalize();
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.0f;
  Vec3f norm(0.0f, 0.0f, -1.0f);  // norm == this->normal(p)

  Vec3f ret_ambient = effective_color * m_ambient;
  Vec3f ret_diffuse;
  Vec3f ret_specular;

  float light_normal = dot(lightv, norm);
  if (light_normal > 0.0f) {
    ret_diffuse = effective_color * m_diffuse * light_normal;
    Vec3f reflectv = reflect(-lightv, norm);
    float reflect_dot_eye = dot(reflectv, eye);
    if (reflect_dot_eye > 0.0f) {
      float factor = pow(reflect_dot_eye, m_shininess);
      ret_specular = light.intensity() * m_specular * factor;
    }
  }

  Vec3f result = ret_ambient + ret_diffuse + ret_specular;

  ASSERT_EQ(result.x(), 1.9f);
  ASSERT_EQ(result.y(), 1.9f);
  ASSERT_EQ(result.z(), 1.9f);
  ASSERT_TRUE(result == Vec3f(1.9f));
}
