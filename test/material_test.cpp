#include "composite/builder.h"
#include "composite/world.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "renderers/phong_model.h"
#include "tools/tools.h"

using namespace testing;

class MaterialTest : public Test {
 public:
  Sphere *s;
  PointLight light;
  BuilderPtr builder;
  IntersectionRecord rec;
};

TEST_F(MaterialTest, createsDefaultLight) {
  ASSERT_TRUE(light.position() == Point3f());
  ASSERT_TRUE(light.intensity() == Vec3f());
}

TEST_F(MaterialTest, createsNewLight1) {
  light.setPosition(Point3f(1.0f, 2.0f, 3.0f));
  light.setIntensity(Vec3f(0.1f, 0.1f, 0.3f));

  ASSERT_EQ(light.position().x(), 1.0f);
  ASSERT_EQ(light.position().y(), 2.0f);
  ASSERT_EQ(light.position().z(), 3.0f);

  ASSERT_EQ(light.intensity().x(), 0.1f);
  ASSERT_EQ(light.intensity().y(), 0.1f);
  ASSERT_EQ(light.intensity().z(), 0.3f);
}

TEST_F(MaterialTest, createsNewLight2) {
  light = PointLight(Point3f(0.1f, -4.0f, -0.4f), Vec3f(1.0f, 4.0f, 0.0f));

  ASSERT_EQ(light.position().x(), 0.1f);
  ASSERT_EQ(light.position().y(), -4.0f);
  ASSERT_EQ(light.position().z(), -0.4f);

  ASSERT_EQ(light.intensity().x(), 1.0f);
  ASSERT_EQ(light.intensity().y(), 4.0f);
  ASSERT_EQ(light.intensity().z(), 0.0f);
}

TEST_F(MaterialTest, lightsWithEyeBetweenLightAndSurface) {
  //----------------------------------
  // As it is in Material->lighting
  //----------------------------------

  Vec3f eye(0.0f, 0.0f, -1.0f);
  light = PointLight(Point3f(0.0f, 0.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));

  Point3f p(0.0f, 0.0f, 0.0f);
  Vec3f m_color(1.0f, 1.0f, 1.0f);
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
  if (light_normal >= 0.0f) {
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
  ASSERT_TRUE(result == Vec3f(1.9f, 1.9f, 1.9f));
}

TEST_F(MaterialTest, lightingWithSurfaceInShadow) {
  //------------------------------------------------------
  // As it is in Material->lighting | Update (With shadow)
  //------------------------------------------------------

  bool in_shadow = true;
  Vec3f eye(0.0f, 0.0f, -1.0f);
  light = PointLight(Point3f(0.0f, 0.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));

  Point3f p(0.0f, 0.0f, 0.0f);
  Vec3f m_color(1.0f, 1.0f, 1.0f);
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

  if (!in_shadow) {
    float light_normal = dot(lightv, norm);
    if (light_normal >= 0.0f) {
      ret_diffuse = effective_color * m_diffuse * light_normal;
      Vec3f reflectv = reflect(-lightv, norm);
      float reflect_dot_eye = dot(reflectv, eye);
      if (reflect_dot_eye > 0.0f) {
        float factor = pow(reflect_dot_eye, m_shininess);
        ret_specular = light.intensity() * m_specular * factor;
      }
    }
  }

  Vec3f result = ret_ambient + ret_diffuse + ret_specular;

  ASSERT_EQ(result.x(), 0.1f);
  ASSERT_EQ(result.y(), 0.1f);
  ASSERT_EQ(result.z(), 0.1f);
  ASSERT_TRUE(result == Vec3f(0.1f, 0.1f, 0.1f));
}

TEST_F(MaterialTest, precomputingTheReflectionVector) {
  MaterialProperties prop;
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Plane);
  SceneElementRawPtr plane = builder->getCurrentElement();
  Ray r(Point3f(0.f, 1.f, -1.f), Vec3f(0.f, -sqrt(2.f) / 2.f, sqrt(2.f) / 2.));
  plane->intersect(r, rec);
  Vec3f reflection_vector = reflect(r.direction(), plane->normal(rec.point(r)));

  EXPECT_TRUE(reflection_vector ==
              Vec3f(0.f, sqrt(2.f) / 2.f, sqrt(2.f) / 2.f));
}

TEST_F(MaterialTest, strikeNonReflectiveSurface) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));
  MaterialProperties prop;
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(light);
  builder->processSceneElement(new Sphere);
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.f, 0.6f))
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  builder->applyMaterial(
      std::make_unique<ConstantTexture>(Vec3f(0.8f, 1.f, 0.6f)), prop);
  builder->addElement();
  prop.setProperty(Props::AMBIENT, 1.f);
  builder->processSceneElement(new Sphere);
  builder->applyMaterial(std::make_unique<ConstantTexture>(), prop);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.f, 1.f));
  BaseRendererPtr pm = std::make_unique<PhongModel>();
  SceneElementPtr w = builder->getProduct();
  w->accept(*pm, r);
  Vec3f color = pm->getColor();
  float e = 0.1f;
  EXPECT_NEAR(color.x(), 0.f, e);
  EXPECT_NEAR(color.y(), 0.f, e);
  EXPECT_NEAR(color.z(), 0.f, e);
}

/*TEST_F(TMat, determiningN1AndN2) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_unique<ConstantTexture>();
  MaterialProperties prop1;
  prop1.setProperty(Props::REFRACTIVE_INDEX, 1.5f);
  tex1->setColor(prop1.getPropertyAsVec3f(Props::COLOR));
  builder->createSphere();
  builder->applyTransformation(scale(2.f, 2.f, 2.f));
  builder->applyMaterial(std::move(tex1), prop1);
  builder->addElement();

  TexturePtr tex2 = std::make_unique<ConstantTexture>();
  MaterialProperties prop2;
  prop2.setProperty(Props::REFRACTIVE_INDEX, 2.f);
  builder->createSphere();
  builder->applyTransformation(translation(0.f, 0.f, -0.25f));
  builder->applyMaterial(std::move(tex2), prop2);
  builder->addElement();

  TexturePtr tex3 = std::make_unique<ConstantTexture>();
  MaterialProperties prop3;
  prop3.setProperty(Props::REFRACTIVE_INDEX, 2.5f);
  builder->createSphere();
  builder->applyTransformation(translation(0.f, 0.f, 0.25f));
  builder->applyMaterial(std::move(tex3), prop3);
  builder->addElement();

  Ray r(Point3f(0.f, 0.f, -4.f), Vec3f(0.f, 0.f, 1.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  pm.determineRefractionIndices(world, r);
  std::map<size_t, std::pair<float, float>> container = pm.getContainer();
  EXPECT_EQ(container[0].first, 1.f);
  EXPECT_EQ(container[0].second, 1.5f);
  EXPECT_EQ(container[1].first, 1.5f);
  EXPECT_EQ(container[1].second, 1.5f);
  EXPECT_EQ(container[2].first, 1.5f);
  EXPECT_EQ(container[2].second, 2.f);
  EXPECT_EQ(container[3].first, 2.f);
  EXPECT_EQ(container[3].second, 2.f);
  EXPECT_EQ(container[4].first, 2.f);
  EXPECT_EQ(container[4].second, 2.5f);
  EXPECT_EQ(container[5].first, 2.5f);
  EXPECT_EQ(container[5].second, 2.5f);
}*/

/*TEST_F(TMat, computingUnderPoint) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_unique<ConstantTexture>();
  MaterialProperties prop1;
  prop1.setProperty(Props::REFRACTIVE_INDEX, 1.5f)
      .setProperty(Props::TRANSPARENCY, 0.5f);
  tex1->setColor(prop1.getPropertyAsVec3f(Props::COLOR));
  builder->createSphere();
  builder->applyTransformation(translation(0.f, 0.f, 1.f));
  builder->applyMaterial(std::move(tex1), prop1);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  pm.visitSceneElementComposite(world, r);
  EXPECT_GT(world->getChildren()
                .back()
                ->getRecord()
                .under_point_from_refrac_surf.z(),
            EPS / 2.f);
  EXPECT_GT(Vec3f(world->getChildren()
                      .back()
                      ->getRecord()
                      .under_point_from_refrac_surf)
                .length(),
            Vec3f(world->getChildren().back()->getRecord().point(r)).length());
}*/

/*TEST_F(TMat, findingRefractedColorOfOpaqueObject) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_unique<ConstantTexture>();
  MaterialProperties prop;
  prop.setProperty(Props::TRANSPARENCY, 0.f);
  builder->createSphere();
  builder->applyTransformation(translation(0.f, 0.f, 1.f));
  builder->applyMaterial(std::move(tex1), prop);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  Vec3f color = pm.refractedColor(world, r);
  EXPECT_EQ(color.x(), 0.f);
  EXPECT_EQ(color.y(), 0.f);
  EXPECT_EQ(color.z(), 0.f);
}*/

/*TEST_F(TMat, whenRecursionIsZeroThenRefractiveColorIsBlack) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_unique<ConstantTexture>();
  MaterialProperties prop;
  prop.setProperty(Props::TRANSPARENCY, 1.f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.5f);
  builder->createSphere();
  builder->applyTransformation(translation(0.f, 0.f, 1.f));
  builder->applyMaterial(std::move(tex1), prop);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  Vec3f color = pm.refractedColor(world, r, 0);
  EXPECT_EQ(color.x(), 0.f);
  EXPECT_EQ(color.y(), 0.f);
  EXPECT_EQ(color.z(), 0.f);
}*/

/*TEST_F(TMat, findRefractedColorUnderTotalReflection) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_shared<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_shared<ConstantTexture>();
  MaterialProperties prop;
  prop.setProperty(Props::TRANSPARENCY, 1.f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.5f)
      .setProperty(Props::COLOR, Vec3f(0.8f, 1.f, 0.6f))
      .setProperty(Props::SPECULAR, 0.2f)
      .setProperty(Props::DIFFUSE, 0.7f);
  builder->createSphere();
  builder->applyTransformation(scale(0.5f, 0.5f, 0.5f));
  builder->applyMaterial(tex1, prop);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, sqrt(2.f) / 2.f), Vec3f(0.f, 1.f, 0.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  Vec3f color = pm.refractedColor(world, r, 0);
  EXPECT_EQ(color.x(), 0.f);
  EXPECT_EQ(color.y(), 0.f);
  EXPECT_EQ(color.z(), 0.f);
}

TEST_F(TMat, findingRefractedColor) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));

  BuilderPtr builder = std::make_shared<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex1 = std::make_shared<ConstantTexture>();
  MaterialProperties prop;
  prop.setProperty(Props::TRANSPARENCY, 1.f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.5f)
      .setProperty(Props::COLOR, Vec3f(0.8f, 1.f, 0.6f))
      .setProperty(Props::SPECULAR, 0.2f)
      .setProperty(Props::DIFFUSE, 0.7f);
  builder->createSphere();
  builder->applyTransformation(scale(0.5f, 0.5f, 0.5f));
  builder->applyMaterial(tex1, prop);
  builder->addElement();
  Ray r(Point3f(0.f, 0.f, 1.f), Vec3f(0.f, 1.f, 0.f));
  PhongModel pm;
  SceneElementPtr world = builder->getProduct();
  Vec3f color = pm.refractedColor(world, r, 0);
  EXPECT_EQ(color.x(), 0.f);
  EXPECT_EQ(color.y(), 0.f);
  EXPECT_EQ(color.z(), 0.f);
}*/

// TODO: Fix the test case
/*TEST_F(TMat, strikeReflectiveSurface) {
  PointLight light(Point3f(-10.f, 10.f, -10.f), Vec3f(1.f, 1.f, 1.f));
  Properties prop;
  BuilderPtr builder = std::make_shared<WorldBuilder>();
  builder->createWorld(light);

  TexturePtr tex = std::make_shared<ConstantTexture>();

  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.f, 0.6f));
  builder->createSphere();
  tex->setColor(prop.getPropertyAsVec3f(Props::COLOR));
  builder->applyMaterial(tex, prop);
  ElementPtr s = builder->getCurrentElement();
  builder->addElement();

  prop.setProperty(Props::COLOR, Vec3f(1.f, 1.f, 1.f))
      .setProperty(Props::REFLECTION, 0.5f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, translation(0.f, -1.f, 0.f));
  builder->createPlane();
  builder->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  tex->setColor(prop.getPropertyAsVec3f(Props::COLOR));
  builder->applyMaterial(tex, prop);
  ElementPtr p = builder->getCurrentElement();
  builder->addElement();

  ElementPtr w = builder->getProduct();

  Ray r(Point3f(0.f, 0.f, -3.f), Vec3f(0.f, -sqrt(2.f) / 2.f, sqrt(2.f) / 2.f));
  w->intersect(r);
  ElementPtr t = w->closestHit(r);
  if (t) ASSERT_TRUE(t == p);
  Vec3f color = w->colorAt(r);
  float eps = 1.E-2f;
  EXPECT_NEAR(color.x(), 0.87677f, eps);
  EXPECT_NEAR(color.y(), 0.92436f, eps);
  EXPECT_NEAR(color.z(), 0.82918f, eps);
}
*/
