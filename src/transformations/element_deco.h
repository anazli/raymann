#pragma once
#include "composite/scene_element.h"

class ElementDeco : public SceneElement {
 public:
  ElementDeco(SceneElement *tr);
  ~ElementDeco() override;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override;
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(std::shared_ptr<SceneElement> item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;

  void setMaterial(BaseMaterialPtr mat) override;
  BaseMaterialPtr getMaterial() const override;
  void setParent(SceneElementRawPtr parent) override;
  SceneElementRawPtr getParent() const override;

 protected:
  SceneElement *m_element;
};
