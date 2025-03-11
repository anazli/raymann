#pragma once

#include <list>

#include "composite/scene_element.h"

using SceneElementContainer = std::vector<SceneElementPtr>;

class World : public SceneElement {
  friend class WorldIterator;

 public:
  ~World() override = default;
  bool intersect(const Ray &r, Intersection &record) override;
  void add(SceneElementPtr item) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() const override;
  SceneElementContainer &getChildren() override;
  void setLight(const PointLight &light) override;
  PointLight getLight() const override;
  BoundingBox getBoundingBox() const override;

  static SceneElementPtr create();

 private:
  SceneElementContainer m_sceneElementContainer;
  PointLight m_light;
  BoundingBox m_bBox;
};
