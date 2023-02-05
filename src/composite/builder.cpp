#include "composite/builder.h"

#include "composite/world.h"
#include "decorators/material.h"
#include "decorators/transformer.h"
#include "geometry/plane.h"

WorldBuilder::WorldBuilder()
    : m_product(nullptr), m_currentTraceable(nullptr) {}

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
  m_currentTraceable = new Sphere(c, r);
}

void WorldBuilder::addElement() {
  if (m_product) {
    TraceablePtr elem(m_currentTraceable);
    m_product->add(elem);
    m_currentTraceable = nullptr;
  }
}

void WorldBuilder::createPlane() { m_currentTraceable = new Plane(); }

void WorldBuilder::applyTransformation(const Mat4f& trans) {
  if (m_currentTraceable)
    m_currentTraceable = new Transformer(m_currentTraceable, trans);
}

void WorldBuilder::applyMaterial(TexturePtr tex, const Properties& prop) {
  m_currentTraceable = new Material(m_currentTraceable, tex, prop);
}

TraceablePtr WorldBuilder::getProduct() { return m_product; }

TraceablePtr WorldBuilder::getCurrentElement() const {
  TraceablePtr ret(m_currentTraceable);
  return ret;
}
