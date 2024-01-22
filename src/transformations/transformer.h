#pragma once
#include "transformations/scene_element_decorator.h"

class Transformer : public SceneElementDecorator {
 public:
  ~Transformer() override = default;
  Transformer(SceneElement *tr);
  Transformer(SceneElement *tr, const Mat4f &m);
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override;
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(SceneElement *item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;

  void setMaterial(BaseMaterialPtr mat) override;
  BaseMaterialPtr getMaterial() const override;
  void setParent(SceneElementRawPtr parent) override;
  SceneElementRawPtr getParent() const override;

 private:
  Mat4f m_transformMatrix;
};
