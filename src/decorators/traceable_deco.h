#pragma once
#include "composite/traceable.h"

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr);
  virtual ~TraceableDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(const PointLight &light, const Ray &ray) override;
  Vec3f color_at(const Ray &ray, const int &rec = 5) override;
  Vec3f reflectedColor(const Ray &r, const int &rec = 5) override;
  std::string name() const override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(const Point3f &p) override;

 protected:
  Traceable *m_traceable;
};
