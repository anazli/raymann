#pragma once
#include "composite/scene_element.h"

class SceneElementDecorator : public SceneElement {
 public:
  SceneElementDecorator(SceneElement *tr, const Mat4f &m = Mat4f());
  ~SceneElementDecorator() override;
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

 protected:
  SceneElement *m_element;
  Mat4f m_transformMatrix;
};
