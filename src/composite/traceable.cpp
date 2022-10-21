#include "composite/traceable.h"

Traceable::~Traceable() {}

Vec3f Traceable::lighting(const PointLight& light, const Ray& ray) {
  return Vec3f();
}

Vec3f Traceable::color_at(const Ray& ray, const int& rec) { return Vec3f(); }

Vec3f Traceable::reflectedColor(const Ray& r, const int& rec) {
  return Vec3f();
}

bool Traceable::isWorld() const { return false; }

Vec3f Traceable::normal(const Point3f& p) const { return Vec3f(); }

std::string Traceable::name() const { return m_name; }

Traceable& Traceable::closestHit(const Ray& r) { return *this; }

void Traceable::checkInside(const Ray& r) {
  if (dot(record().eye(r), normal(record().point(r))) < 0.0f) rec.inside = true;
}

bool Traceable::isShadowed(const Point3f& p) { return false; }

PointLight Traceable::getLight() const { return PointLight(); }

Record& Traceable::record() { return rec; }

void Traceable::setParent(Traceable* t) { m_parent = t; }

Traceable* Traceable::getParent() const { return m_parent; }
