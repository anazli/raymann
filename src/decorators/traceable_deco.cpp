#include "decorators/traceable_deco.h"

TraceableDeco::TraceableDeco(Traceable* tr) : m_traceable(tr) {}

TraceableDeco::~TraceableDeco() { delete m_traceable; }

bool TraceableDeco::intersect(const Ray& r) {
  return m_traceable->intersect(r);
}

Vec3f TraceableDeco::lighting(const PointLight& light, const Ray& ray) {
  return m_traceable->lighting(light, ray);
}

Vec3f TraceableDeco::color_at(const Ray& ray, const int& rec) {
  return m_traceable->color_at(ray, rec);
}

Vec3f TraceableDeco::reflectedColor(const Ray& r, const int& rec) {
  return m_traceable->reflectedColor(r, rec);
}

std::string TraceableDeco::name() const { return m_traceable->name(); }

Record& TraceableDeco::record() { return m_traceable->record(); }

Vec3f TraceableDeco::normal(const Point3f& p) const {
  return m_traceable->normal(p);
}

void TraceableDeco::checkInside(const Ray& r) {
  return m_traceable->checkInside(r);
}

bool TraceableDeco::isShadowed(const Point3f& p) {
  return m_traceable->isShadowed(p);
}
