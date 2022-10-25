#include "decorators/traceable_deco.h"

TraceableDeco::TraceableDeco(Traceable* tr) : m_traceable(tr) {}

TraceableDeco::~TraceableDeco() { delete m_traceable; }

bool TraceableDeco::intersect(const Ray& r) {
  return m_traceable->intersect(r);
}

Vec3f TraceableDeco::lighting(TraceablePtr w, const Ray& ray) {
  return m_traceable->lighting(w, ray);
}

Vec3f TraceableDeco::colorAt(const Ray& ray) {
  return m_traceable->colorAt(ray);
}

Vec3f TraceableDeco::reflectedColor(TraceablePtr w, const Ray& r) {
  return m_traceable->reflectedColor(w, r);
}

std::string TraceableDeco::name() const { return m_traceable->name(); }

Record& TraceableDeco::record() { return m_traceable->record(); }

Vec3f TraceableDeco::normal(const Point3f& p) const {
  return m_traceable->normal(p);
}

void TraceableDeco::checkInside(const Ray& r) {
  return m_traceable->checkInside(r);
}

bool TraceableDeco::isShadowed(TraceablePtr w, const Point3f& p) {
  return m_traceable->isShadowed(w, p);
}

void TraceableDeco::setParent(TraceablePtr t) { m_traceable->setParent(t); }

TraceablePtr TraceableDeco::getParent() const {
  return m_traceable->getParent();
}
