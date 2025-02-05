#include "composite/builder.h"

#include "acceleration/bvh.h"
#include "application/error.h"
#include "composite/world.h"
#include "transformations/transformer.h"

void WorldBuilder::createWorld() { m_product = new World(m_light); }

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

void WorldBuilder::addLight(const PointLight& light) { m_light = light; }

void WorldBuilder::processSceneElement(PrimitiveRawPtr element) {
  m_currentElement = element;
}

void WorldBuilder::applyTransformation(const Mat4D& trans) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  m_currentElement = new Transformer(m_currentElement, trans);
}

void WorldBuilder::applyWorldTransformation(const Mat4D& trans) {
  APP_ASSERT(m_product, "World not created yet!");
  m_product = new Transformer(m_product, trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex,
                                 const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Material>(std::move(tex), prop);
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setMaterial(matptr);
  }
}
void WorldBuilder::applyLambertianMaterial(TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Lambertian>(std::move(tex), prop);
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setMaterial(matptr);
  }
}

void WorldBuilder::applyEmissiveMaterial(TexturePtr tex,
                                         const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr =
      std::make_shared<EmissiveMaterial>(std::move(tex), prop);
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setMaterial(matptr);
  }
}
void WorldBuilder::applyMetalMaterial(const float& f, TexturePtr tex,
                                      const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr = std::make_shared<Metal>(f, std::move(tex), prop);
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setMaterial(matptr);
  }
}
void WorldBuilder::applyDielectricMaterial(const float& ri, TexturePtr tex,
                                           const MaterialProperties& prop) {
  APP_ASSERT(m_currentElement, "SceneElement not created yet!");
  BaseMaterialPtr matptr =
      std::make_shared<Dielectric>(ri, std::move(tex), prop);
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setMaterial(matptr);
  }
}

void WorldBuilder::createBBoxForElement(const BoundingBox& box) {
  if (auto primitive = dynamic_cast<Primitive*>(m_currentElement)) {
    primitive->setBoundingBox(box);
  }
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
