#include "composite/builder.h"

#include "composite/world.h"
#include "geometry/bounding_box.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
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
  if (m_product) {
    SceneElementPtr elem(m_currentElement);
    m_product->add(elem);
  }
}

void WorldBuilder::processSceneElement(SceneElementRawPtr element) {
  m_currentElement = element;
}

void WorldBuilder::applyTransformation(const Mat4f& trans) {
  if (m_currentElement)
    m_currentElement = new Transformer(m_currentElement, trans);
}

void WorldBuilder::applyWorldTransformation(const Mat4f& trans) {
  m_product = new Transformer(m_product, trans);
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

SceneElementPtr WorldBuilder::getProduct() {
  SceneElementPtr product(m_product);
  return product;
}

SceneElementRawPtr WorldBuilder::getCurrentElement() const {
  return m_currentElement;
}
