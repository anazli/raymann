#pragma once
#include "geometry/traceable.h"

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr);
  virtual ~TraceableDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override;
  Record record() const override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

 protected:
  Traceable *m_traceable;
};
