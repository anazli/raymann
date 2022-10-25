#pragma once

#include <list>

#include "composite/traceable.h"
#include "geometry/sphere.h"

class World : public Traceable, public std::enable_shared_from_this<Traceable> {
 public:
  World();
  World(const std::string &n);
  virtual ~World();
  bool intersect(const Ray &r) override;
  Vec3f colorAt(const Ray &ray) override;
  void add(TraceablePtr item) override;
  void remove(TraceablePtr item, bool del = true) override;
  bool isWorld() const override { return true; }
  TraceablePtr closestHit(const Ray &r) override;
  std::vector<float> intersectionsSorted() const;
  bool isShadowed(const Point3f &p) override;

 private:
  std::list<TraceablePtr> m_traceable_list;
  TraceablePtr m_closest_hit;
  PointLight m_light;
};
