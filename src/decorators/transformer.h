#pragma once
#include "decorators/element_deco.h"

class Transformer : public ElementDeco {
 public:
  Transformer(Element *tr);
  Transformer(Element *tr, const Mat4f &m);
  virtual ~Transformer();
  bool intersect(const Ray &r) override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;

 private:
  Mat4f m_transformer;
};
