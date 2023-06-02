#pragma once
#include "composite/scene_element.h"

class ElementDeco : public SceneElement {
 public:
  ElementDeco(SceneElement *tr);
  virtual ~ElementDeco();
  bool intersect(const Ray &r) override;
  Vec3f normal(const Point3f &p) const override;
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(std::shared_ptr<SceneElement> item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRendererPtr &renderer, const Ray &ray) override;

 protected:
  SceneElement *m_element;
};
