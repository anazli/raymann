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
  Traceable *createSphere(std::shared_ptr<SphereBuilder> b,
                          const Point3f &center = Point3f(0.0f, 0.0f, 0.0f),
                          const float &radius = 1.0f,
                          const Vec3f &color = Vec3f(1.0f, 1.0f, 1.0f),
                          float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
                          float shi = 10.0f);
  Traceable *createTransformedSphere(
      std::shared_ptr<SphereBuilder> b, const Mat4f &matrix,
      const Point3f &center = Point3f(0.0f, 0.0f, 0.0f),
      const float &radius = 1.0f, const Vec3f &color = Vec3f(1.0f, 1.0f, 1.0f),
      float am = 0.1f, float diff = 0.9f, float spec = 0.9f, float shi = 10.0f);

 private:
  std::list<std::shared_ptr<Traceable>> m_traceable_list;
};
