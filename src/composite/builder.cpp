#include "composite/builder.h"

#include "composite/world.h"
#include "decorators/material.h"
#include "decorators/transformer.h"
#include "geometry/plane.h"

WorldBuilder::WorldBuilder()
    : m_product(nullptr), m_currentElement(nullptr) {}

void WorldBuilder::createWorld(const PointLight& light) {
  m_product.reset();
  m_product = std::make_shared<World>();
  m_product->setLight(light);
}

void WorldBuilder::addWorld(const PointLight& light) {
  //----------------
  /*TODO*/
  //----------------
}

void WorldBuilder::createSphere(const Point3f& c, const float& r) {
  m_currentElement = new Sphere(c, r);
}

void WorldBuilder::addElement() {
  if (m_product) {
    ElementPtr elem(m_currentElement);
    m_product->add(elem);
    m_currentElement = nullptr;
  }
}

void WorldBuilder::createPlane() { m_currentElement = new Plane(); }

void WorldBuilder::applyTransformation(const Mat4f& trans) {
  if (m_currentElement)
    m_currentElement = new Transformer(m_currentElement, trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex, const Properties& prop) {
  m_currentElement = new Material(m_currentElement, tex, prop);
}

ElementPtr WorldBuilder::getProduct() { return m_product; }

ElementPtr WorldBuilder::getCurrentElement() const {
  ElementPtr ret(m_currentElement);
  return ret;
}
