#pragma once

#include <list>

#include "composite/scene_element.h"

using SceneElementContainer = std::vector<SceneElementPtr>;

class World : public SceneElement {
  friend class WorldIterator;

 public:
  World() = default;
  World(const PointLight &light);
  ~World() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  SceneElementContainer::iterator remove(
      SceneElementRawPtr element, SceneElementPtr elementToRemove) override;
  bool isWorld() const override;
  Vec3D normal(const Point3D &p) const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() const override;
  void setLight(const PointLight &light);
  PointLight getLight() const;

 private:
  SceneElementContainer m_sceneElementContainer;
  std::vector<SceneElementPtr> m_vector;
  PointLight m_light;
  BoundingBox m_bBox;
};
