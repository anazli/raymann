#pragma once
#include "decorators/element_deco.h"

class Transformer : public ElementDeco {
 public:
  Transformer(SceneElement *tr);
  Transformer(SceneElement *tr, const Mat4f &m);
  virtual ~Transformer();
  bool intersect(const Ray &r) override;
  Vec3f normal(const Point3f &p) const override;
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(std::shared_ptr<SceneElement> item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRendererPtr &renderer, const Ray &ray) override;

 private:
  Mat4f m_transformer;
};
