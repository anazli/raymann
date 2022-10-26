#pragma once
#include "composite/traceable.h"

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr);
  virtual ~TraceableDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(const Ray &ray) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) override;
  Vec3f reflectedColor(const Ray &r, int rec = 5) override;
  std::string name() const override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(const Point3f &p) override;
  void setParent(TraceablePtr t) override;
  TraceablePtr getParent() const override;
  void setLight(const PointLight &l) override;
  PointLight getLight() const override;
  void setReflection(float ref) override;
  float getReflection() const override;

 protected:
  Traceable *m_traceable;
};
