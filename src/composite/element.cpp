#include "composite/element.h"

Element::~Element() {}

Vec3f Element::lighting(const Ray& ray) { return Vec3f(); }

Vec3f Element::colorAt(const Ray& ray, int rec) { return Vec3f(); }

Vec3f Element::reflectedColor(const Ray& r, int rec) { return Vec3f(); }

bool Element::isWorld() const { return false; }

Vec3f Element::normal(const Point3f& p) const { return Vec3f(); }

std::string Element::name() const { return m_name; }

std::shared_ptr<Element> Element::closestHit(const Ray& r) { return nullptr; }

void Element::checkInside(const Ray& r) {
  if (dot(record().eye(r), normal(record().point(r))) < 0.0f) rec.inside = true;
}

bool Element::isShadowed(const Point3f& p) {
  ElementPtr w = getParent();
  if (w) {
    Vec3f v = p - w->getLight().position();
    float distance = v.length();
    Ray r(w->getLight().position(), v.normalize());
    if (w->intersect(r)) {
      ElementPtr t = w->closestHit(r);
      if (t) {
        if (t->record().t_min() >= 0.0f && t->record().t_min() < distance)
          return true;
      }
    }
  }
  return false;
}

void Element::setLight(const PointLight& l) { m_light = l; }

PointLight Element::getLight() const { return m_light; }

Record& Element::record() { return rec; }

void Element::setParent(std::shared_ptr<Element> t) { m_parent = t; }

std::shared_ptr<Element> Element::getParent() const { return m_parent; }

void Element::setReflection(float ref) { m_reflection = ref; }

float Element::getReflection() const { return m_reflection; }
