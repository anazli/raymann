#include "composite/builder.h"

#include "acceleration/bvh.h"
#include "application/error.h"
#include "composite/world.h"
#include "transformations/transformer.h"

void WorldBuilder::createWorld(const PointLight& light) {
  m_product = new World;
  m_product->setLight(light);
}

void WorldBuilder::addWorld() {
  //----------------
  /*TODO*/
  //----------------
}

void WorldBuilder::addElement() {
  APP_ASSERT(m_product, "World not created yet!");
  SceneElementPtr elem(m_currentElement);
  m_product->add(elem);
}

void WorldBuilder::processSceneElement(SceneElementRawPtr element) {
  m_currentElement = element;
}

void WorldBuilder::applyTransformation(const Mat4f& trans) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  m_currentElement = new Transformer(m_currentElement, trans);
}

void WorldBuilder::applyWorldTransformation(const Mat4f& trans) {
  APP_ASSERT(m_product, "World not created yet!");
  m_product = new Transformer(m_product, trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex,
                                 const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Material>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyLambertianMaterial(TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Lambertian>(std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyMetalMaterial(const float& f, TexturePtr tex,
                                      const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Metal>(f, std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}
void WorldBuilder::applyDielectricMaterial(const float& ri, TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr =
      std::make_shared<Dielectric>(ri, std::move(tex), prop);
  m_currentElement->setMaterial(matptr);
}

void WorldBuilder::createBBoxForElement(const BoundingBox& box) {
  m_currentElement->setBoundingBox(box);
}

SceneElementPtr WorldBuilder::getProduct() {
  SceneElementPtr product(m_product);
  return product;
}

SceneElementPtr WorldBuilder::getProductBVHierarchy() {
  SceneElementPtr product(m_product);
  BVHierarchy bvh;
  bvh.divideWorld(product);
  return product;
}

SceneElementRawPtr WorldBuilder::getCurrentElement() const {
  return m_currentElement;
}
