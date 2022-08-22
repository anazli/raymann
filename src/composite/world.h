#pragma once

#include <list>

#include "composite/traceable.h"
#include "geometry/sphere.h"

class World : public Traceable {
 public:
  World() {}
  World(const std::string &n);
  virtual ~World();
  bool intersect(const Ray &r) override;
  void add(std::shared_ptr<Traceable> item) override;
  void remove(std::shared_ptr<Traceable> item, bool del = true) override;
  bool isWorld() const override { return true; }
  Traceable &closestHit(const Ray &r) override;
  std::vector<float> intersectionsSorted() const;
  void setLight(const PointLight &l) override { m_light = l; }
  PointLight getLight() const override { return m_light; }

 private:
  std::list<std::shared_ptr<Traceable>> m_traceable_list;
  PointLight m_light;
};
