#pragma once
#include "composite/traceable.h"

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr);
  virtual ~TraceableDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(TraceablePtr w, const Ray &ray) override;
  Vec3f colorAt(const Ray &ray) override;
  Vec3f reflectedColor(TraceablePtr w, const Ray &r) override;
  std::string name() const override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(TraceablePtr w, const Point3f &p) override;
  void setParent(TraceablePtr t) override;;
  TraceablePtr getParent() const override;

 protected:
  Traceable *m_traceable;
};
