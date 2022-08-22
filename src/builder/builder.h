#pragma once

#include "geometry/traceable.h"

class TraceableBuilder {
 public:
  virtual void addTraceable(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                            const float &r = 1.0f) = 0;
  virtual void transformTraceable(const Mat4f &m) = 0;
  virtual void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                        float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
                        float shi = 200.0f) {}
  virtual void addStripePattern(const Vec3f &colorA, const Vec3f &colorB,
                                const Mat4f &ptrans, const Mat4f &otrans) = 0;
  virtual void addGradientPattern(const Vec3f &colorA, const Vec3f &colorB,
                                  const Mat4f &ptrans, const Mat4f &otrans) = 0;
  virtual void addRingPattern(const Vec3f &colorA, const Vec3f &colorB,
                              const Mat4f &ptrans, const Mat4f &otrans) = 0;
  virtual void addCheckerPattern(const Vec3f &colorA, const Vec3f &colorB,
                                 const Mat4f &ptrans, const Mat4f &otrans) = 0;
  virtual std::shared_ptr<Traceable> getTraceable() const = 0;

 protected:
  TraceableBuilder() {}
};
