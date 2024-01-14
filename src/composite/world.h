#pragma once

#include <list>

#include "composite/scene_element.h"

using SceneElementContainer = std::list<SceneElementPtr>;

class World : public SceneElement,
              public std::enable_shared_from_this<SceneElement> {
  friend class WorldIterator;

 public:
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  void remove(SceneElementPtr item, bool del = true) override;
  bool isWorld() const override;
  Vec3f normal(const Point3f &p) const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getWorldList() override;
  void setLight(const PointLight &light) override;
  PointLight getLight() const override;

 private:
  SceneElementContainer m_sceneElementContainer;
  PointLight m_light;
};

class WorldIterator {
 public:
  WorldIterator(const SceneElementContainer &world)
      : m_sceneElementContainer(world) {}
  bool first() {
    m_current = m_sceneElementContainer.begin();
    if (*m_current)
      return true;
    else
      return false;
  }
  SceneElementContainer::const_iterator begin() const {
    return m_sceneElementContainer.begin();
  }
  SceneElementContainer::const_iterator end() const {
    return m_sceneElementContainer.end();
  }
  void advance() { m_current++; }
  bool notDone() { return m_current != m_sceneElementContainer.end(); }
  SceneElementPtr currentElement() { return *m_current; }

 private:
  SceneElementContainer::const_iterator m_current;
  SceneElementContainer m_sceneElementContainer;
};
