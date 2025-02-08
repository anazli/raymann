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

void WorldBuilder::createPrimitive(const PrimitiveType& type) {
  switch (type) {
    case PrimitiveType::CONE:
      m_currentElement = Cone::create();
      break;
    case PrimitiveType::CUBE:
      m_currentElement = Cube::create();
      break;
    case PrimitiveType::CYLINDER:
      m_currentElement = Cylinder::create();
      break;
    case PrimitiveType::PLANE:
      m_currentElement = Plane::create();
      break;
    case PrimitiveType::SPHERE:
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

void WorldBuilder::applyMaterial(TexturePtr tex,
                                 const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr =
      std::make_shared<StandardMaterial>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyLambertianMaterial(TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Lambertian>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::applyEmissiveMaterial(TexturePtr tex,
                                         const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = std::make_shared<EmissiveMaterial>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::applyMetalMaterial(const float& f, TexturePtr tex,
                                      const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = std::make_shared<Metal>(f, std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyDielectricMaterial(const float& ri, TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  auto matptr = std::make_shared<Dielectric>(ri, std::move(tex), prop);
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
