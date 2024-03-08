#pragma once

#include <list>

#include "composite/scene_element.h"

using SceneElementContainer = std::vector<SceneElementPtr>;

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
  float pdf(const Point3f &origin, const Vec3f &direction) override;
  Vec3f random(const Point3f &origin) override;

 private:
  SceneElementContainer m_sceneElementContainer;
  std::vector<SceneElementPtr> m_vector;
  PointLight m_light;
};
