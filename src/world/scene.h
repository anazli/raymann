#pragma once

#include "geometry/sphere.h"

class Scene {
 public:
  std::shared_ptr<Traceable> createSphere(
      std::shared_ptr<TraceableBuilder> b,
      const Vec3f &color = Vec3f(1.0f, 1.0f, 1.0f), float am = 0.1f,
      float diff = 0.9f, float spec = 0.9f, float shi = 200.0f,
      const Point3f &center = Point3f(0.0f, 0.0f, 0.0f),
      const float &radius = 1.0f);

  std::shared_ptr<Traceable> createTransformedSphere(
      std::shared_ptr<TraceableBuilder> b, const Mat4f &matrix,
      const Vec3f &color = Vec3f(1.0f, 1.0f, 1.0f), float am = 0.1f,
      float diff = 0.9f, float spec = 0.9f, float shi = 200.0f,
      const Point3f &center = Point3f(0.0f, 0.0f, 0.0f),
      const float &radius = 1.0f);

 private:
};
