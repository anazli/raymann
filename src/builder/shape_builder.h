#pragma once
#include "builder/builder.h"
#include "decorators/material.h"
#include "decorators/transformer.h"
#include "geometry/sphere.h"

class Shape : public TraceableBuilder {
 public:
  Shape();
  void addTraceable(Traceable *tr) override;
  void transformTraceable(const Mat4f &m) override;
  void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                float reflection = 0.0f) override;
  void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                float reflection = 0.0f, float am = 0.1f, float diff = 0.9f,
                float spec = 0.9f, float shi = 200.0f) override;
  void addStripePattern(const Vec3f &colorA, const Vec3f &colorB,
                        const Mat4f &ptrans, const Mat4f &otrans) override;
  void addGradientPattern(const Vec3f &colorA, const Vec3f &colorB,
                          const Mat4f &ptrans, const Mat4f &otrans) override;
  void addRingPattern(const Vec3f &colorA, const Vec3f &colorB,
                      const Mat4f &ptrans, const Mat4f &otrans) override;
  void addCheckerPattern(const Vec3f &colorA, const Vec3f &colorB,
                         const Mat4f &ptrans, const Mat4f &otrans) override;
  std::shared_ptr<Traceable> getTraceable() const override;

 private:
  Traceable *m_currentTraceable;
};
