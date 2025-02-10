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
  SceneElementContainer::iterator remove(SceneElementRawPtr item,
                                         SceneElementPtr removedElem) override;
  bool isWorld() const override;
  Vec3D normal(const Point3D &p) const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() const override;
  SceneElementContainer &getChildren() override;
  void setLight(const PointLight &light) override;
  PointLight getLight() const override;
  float pdf(const Point3D &origin, const Vec3D &direction) override;
  Vec3D random(const Point3D &origin) override;

  static SceneElementPtr create();

 private:
  SceneElementContainer m_sceneElementContainer;
  std::vector<SceneElementPtr> m_vector;
  PointLight m_light;
};
