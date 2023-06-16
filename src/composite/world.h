#pragma once

#include <list>

#include "composite/scene_element.h"

class World : public SceneElement,
              public std::enable_shared_from_this<SceneElement> {
  friend class WorldIterator;

 public:
  World() = default;
  virtual ~World();
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  void remove(SceneElementPtr item, bool del = true) override;
  bool isWorld() const override;
  Vec3f normal(const Point3f &p) const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  std::list<std::shared_ptr<SceneElement>> getWorldList() override;
  void setLight(const PointLight &light) override;
  PointLight getLight() const override;

 private:
  std::list<SceneElementPtr> m_scene_elem_list;
  PointLight m_light;
};

class WorldIterator {
 public:
  WorldIterator(const std::list<SceneElementPtr> &world) : m_list(world) {}
  bool first() {
    m_current = m_list.begin();
    if (*m_current)
      return true;
    else
      return false;
  }
  std::list<SceneElementPtr>::const_iterator begin() const {
    return m_list.begin();
  }
  std::list<SceneElementPtr>::const_iterator end() const {
    return m_list.end();
  }
  void advance() { m_current++; }
  bool notDone() { return m_current != m_list.end(); }
  SceneElementPtr currentElement() { return *m_current; }

 private:
  std::list<SceneElementPtr>::const_iterator m_current;
  std::list<SceneElementPtr> m_list;
};
