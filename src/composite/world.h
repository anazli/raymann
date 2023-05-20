#pragma once

#include <list>

#include "composite/element.h"
#include "geometry/sphere.h"

class World : public Element, public std::enable_shared_from_this<Element> {
 public:
  World();
  World(const std::string &n);
  virtual ~World();
  bool intersect(const Ray &r) override;
  Vec3f lighting(const Ray &ray) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) override;
  Vec3f reflectedColor(const Ray &r, int rec = 5) override;
  void add(ElementPtr item) override;
  void remove(ElementPtr item, bool del = true) override;
  bool isWorld() const override { return true; }
  ElementPtr closestHit(const Ray &r) override;
  std::vector<float> intersectionsSorted() const;
  bool isShadowed(const Point3f &p) override;

 private:
  std::list<ElementPtr> m_traceable_list;
  ElementPtr m_closest_hit;
  PointLight m_light;
};
