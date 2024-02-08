#pragma once

#include <list>

#include "composite/scene_element.h"

using SceneElementContainer = std::list<SceneElementPtr>;

class World : public SceneElement {
  friend class WorldIterator;

 public:
  ~World() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  SceneElementContainer::iterator remove(SceneElementRawPtr item,
                                         SceneElementPtr removedElem) override;
  bool isWorld() const override;
  Vec3f normal(const Point3f &p) const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() const override;
  SceneElementContainer &getChildren() override;
  void setLight(const PointLight &light) override;
  PointLight getLight() const override;

 private:
  SceneElementContainer m_sceneElementContainer;
  PointLight m_light;
};
