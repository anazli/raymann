#include "composite/builder.h"

#include "acceleration/bvh.h"
#include "application/error.h"
#include "builder.h"
#include "composite/world.h"
#include "geometry/cone.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"
#include "geometry/quad.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "materials/dielectric.h"
#include "materials/emissive.h"
#include "materials/isotropic.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/standard.h"
#include "transformations/transformation.h"

void WorldBuilder::createWorld() {
  m_product = World::create();
  m_product->setLight(m_light);
}

void WorldBuilder::addWorldToProduct() {
  //----------------
  /*TODO*/
  //----------------
}

void WorldBuilder::addElementToProduct() {
  APP_ASSERT(m_product, "World not created yet!");
  SceneElementPtr elem(m_currentElement);
  m_product->add(elem);
}

void WorldBuilder::addLight(const PointLight& light) { m_light = light; }

void WorldBuilder::createPrimitive(const AppParameters& type) {
  switch (type) {
    case AppParameters::CONE:
      m_currentElement = Cone::create();
      break;
    case AppParameters::CUBE:
      m_currentElement = Cube::create();
      break;
    case AppParameters::CYLINDER:
      m_currentElement = Cylinder::create();
      break;
    case AppParameters::PLANE:
      m_currentElement = Plane::create();
      break;
    case AppParameters::SPHERE:
      m_currentElement = Sphere::create();
      break;
    default:
      APP_ASSERT(false, "Cannot create the specified type!");
  }
}

void WorldBuilder::createPrimitive(SceneElementRawPtr primitive) {
  SceneElementPtr elem(primitive);
  m_currentElement = elem;
}
void WorldBuilder::applyTransformation(const Mat4D& trans) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  m_currentElement->setTransformation(trans);
}

void WorldBuilder::applyWorldTransformation(const Mat4D& trans) {
  APP_ASSERT(m_product, "World not created yet!");
  m_product->setTransformation(trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex, const DataContainer& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  MaterialPtr matptr = StandardMaterial::create(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyLambertianMaterial(TexturePtr tex,
                                           const DataContainer& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  MaterialPtr matptr = Lambertian::create(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::applyEmissiveMaterial(TexturePtr tex,
                                         const DataContainer& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = EmissiveMaterial::create(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::applyMetalMaterial(TexturePtr tex,
                                      const DataContainer& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = Metal::create(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyDielectricMaterial(TexturePtr tex,
                                           const DataContainer& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = Dielectric::create(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::createBBoxForElement(const BoundingBox& box) {
  m_currentElement->setBoundingBox(box);
}

SceneElementPtr WorldBuilder::getProduct() { return m_product; }

SceneElementPtr WorldBuilder::getProductBVHierarchy() {
  SceneElementPtr product(m_product);
  BVHierarchy bvh;
  bvh.divideWorld(product);
  return product;
}

const SceneElementPtr WorldBuilder::getCurrentElement() const {
  return m_currentElement;
}
