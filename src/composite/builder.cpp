#include "composite/builder.h"

#include "composite/world.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "transformations//transformer.h"

void WorldBuilder::createWorld(const PointLight& light) {
  m_product.reset();
  m_product = std::make_shared<World>();
  m_product->setLight(light);
}

void WorldBuilder::addWorld() {
  //----------------
  /*TODO*/
  //----------------
}

void WorldBuilder::createSphere(const Point3f& c, const float& r) {
  m_currentElement = new Sphere(c, r);
}

void WorldBuilder::addElement() {
  if (m_product) {
    SceneElementPtr elem(m_currentElement);
    m_product->add(elem);
    m_currentElement = nullptr;
  }
  // TODO: Assert that world is already created
}

void WorldBuilder::createPlane() { m_currentElement = new Plane(); }

void WorldBuilder::applyTransformation(const Mat4f& trans) {
  if (m_currentElement)
    m_currentElement = new Transformer(m_currentElement, trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex,
                                 const MaterialProperties& prop) {
  BaseMaterialPtr matptr = std::make_shared<Material>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyLambertianMaterial(TexturePtr tex,
                                           const MaterialProperties& prop) {
  BaseMaterialPtr matptr = std::make_shared<Lambertian>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyMetalMaterial(const float& f, TexturePtr tex,
                                      const MaterialProperties& prop) {
  BaseMaterialPtr matptr = std::make_shared<Metal>(f, std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyDielectricMaterial(const float& ri, TexturePtr tex,
                                           const MaterialProperties& prop) {
  BaseMaterialPtr matptr =
      std::make_shared<Dielectric>(ri, std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

SceneElementPtr WorldBuilder::getProduct() { return m_product; }

SceneElementPtr WorldBuilder::getCurrentElement() const {
  SceneElementPtr ret(m_currentElement);
  return ret;
}
