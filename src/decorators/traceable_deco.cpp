#include "decorators/traceable_deco.h"

TraceableDeco::TraceableDeco(Traceable* tr) : m_traceable(tr) {}

TraceableDeco::~TraceableDeco() { delete m_traceable; }

bool TraceableDeco::intersect(const Ray& r) {
  return m_traceable->intersect(r);
}

Vec3f TraceableDeco::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  return m_traceable->lighting(w, ray);
}

std::string TraceableDeco::name() const { return m_traceable->name(); }

Record& TraceableDeco::record() { return m_traceable->record(); }

Vec3f TraceableDeco::normal(const Point3f& p) const {
  return m_traceable->normal(p);
}

void TraceableDeco::checkInside(const Ray& r) {
  return m_traceable->checkInside(r);
}

bool TraceableDeco::isShadowed(std::shared_ptr<Traceable> w, const Point3f& p) {
  return m_traceable->isShadowed(w, p);
}
