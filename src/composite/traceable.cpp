#include "composite/traceable.h"

Traceable::~Traceable() {}

Vec3f Traceable::lighting(const Ray& ray) { return Vec3f(); }

Vec3f Traceable::colorAt(const Ray& ray, int rec) { return Vec3f(); }

Vec3f Traceable::reflectedColor(const Ray& r, int rec) { return Vec3f(); }

bool Traceable::isWorld() const { return false; }

Vec3f Traceable::normal(const Point3f& p) const { return Vec3f(); }

std::string Traceable::name() const { return m_name; }

std::shared_ptr<Traceable> Traceable::closestHit(const Ray& r) {
  return nullptr;
}

void Traceable::checkInside(const Ray& r) {
  if (dot(record().eye(r), normal(record().point(r))) < 0.0f) rec.inside = true;
}

bool Traceable::isShadowed(const Point3f& p) {
  TraceablePtr w = getParent();
  if (w) {
    Vec3f v = p - w->getLight().position();
    float distance = v.length();
    Ray r(w->getLight().position(), v.normalize());
    if (w->intersect(r)) {
      TraceablePtr t = w->closestHit(r);
      if (t) {
        if (t->record().t_min() >= 0.0f && t->record().t_min() < distance)
          return true;
      }
    }
  }
  return false;
}

void Traceable::setLight(const PointLight& l) { m_light = l; }

PointLight Traceable::getLight() const { return m_light; }

Record& Traceable::record() { return rec; }

void Traceable::setParent(std::shared_ptr<Traceable> t) { m_parent = t; }

std::shared_ptr<Traceable> Traceable::getParent() const { return m_parent; }
