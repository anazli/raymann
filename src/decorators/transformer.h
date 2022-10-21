#pragma once
#include "decorators/traceable_deco.h"

class Transformer : public TraceableDeco {
 public:
  Transformer(Traceable *tr);
  Transformer(Traceable *tr, const Mat4f &m);
  virtual ~Transformer();
  bool intersect(const Ray &r) override;
  std::string name() const override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

 private:
  Mat4f m_transformer;
};
