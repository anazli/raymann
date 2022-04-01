#pragma once

#include "geometry/sphere.h"
#include "geometry/traceable.h"

class World : public Traceable {
 public:
  World() {}
  World(const std::string &n);
  virtual ~World();
  bool intersect(const Ray &r) override;
  void add(std::shared_ptr<Traceable> item) override;
  void remove(std::shared_ptr<Traceable> item, bool del = true) override;
  bool isWorld() const override { return true; }
  Traceable &closestHit() override;
  std::vector<float> intersectionsSorted() const;
  void createSphere(std::shared_ptr<SphereBuilder> b);

 private:
  std::list<std::shared_ptr<Traceable>> m_traceable_list;
};
