#pragma once
#include "transformations/scene_element_decorator.h"

class Transformer : public SceneElementDecorator {
 public:
  ~Transformer() override = default;
  Transformer(SceneElementRawPtr tr, const Mat4f &m = Mat4f());
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override;
  void add(SceneElementPtr item) override;
  void remove(SceneElementRawPtr item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() override;

  void setParent(SceneElementRawPtr parent) override;
  SceneElementRawPtr getParent() const override;
  Point3f pointFromWorldToObjectSpace(const Point3f &point) const override;
  Vec3f vectorFromObjectToWorldSpace(const Vec3f vec) const override;
};
