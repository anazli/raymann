#include "decorators/element_deco.h"

ElementDeco::ElementDeco(Element* tr) : m_element(tr) {}

ElementDeco::~ElementDeco() { delete m_element; }

bool ElementDeco::intersect(const Ray& r) { return m_element->intersect(r); }

Vec3f ElementDeco::lighting(const Ray& ray) { return m_element->lighting(ray); }

Vec3f ElementDeco::colorAt(const Ray& ray, int rec) {
  return m_element->colorAt(ray, rec);
}

Vec3f ElementDeco::reflectedColor(const Ray& r, int rec) {
  return m_element->reflectedColor(r, rec);
}

std::string ElementDeco::name() const { return m_element->name(); }

Record& ElementDeco::record() { return m_element->record(); }

Vec3f ElementDeco::normal(const Point3f& p) const {
  return m_element->normal(p);
}

void ElementDeco::checkInside(const Ray& r) {
  return m_element->checkInside(r);
}

bool ElementDeco::isShadowed(const Point3f& p) {
  return m_element->isShadowed(p);
}

void ElementDeco::setParent(ElementPtr t) { m_element->setParent(t); }

ElementPtr ElementDeco::getParent() const { return m_element->getParent(); }

void ElementDeco::setLight(const PointLight& l) { m_element->setLight(l); }

PointLight ElementDeco::getLight() const { return m_element->getLight(); }

void ElementDeco::setReflection(float ref) { m_element->setReflection(ref); }

float ElementDeco::getReflection() const { return m_element->getReflection(); }
