#pragma once
#include "builder/builder.h"
#include "decorators/material.h"
#include "decorators/transformer.h"
#include "geometry/sphere.h"

class StandardSphere : public TraceableBuilder {
 public:
  StandardSphere() : m_currentSphere(nullptr) {}
  void addTraceable(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                    const float &r = 1.0f) override {
    m_currentSphere = new Sphere(c, r);
  }
  void transformTraceable(const Mat4f &m) override {
    m_currentSphere = new Transformer(m_currentSphere, m);
  }
  void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                float reflection = 0.0f) override {
    m_currentSphere = new Material(m_currentSphere, c, reflection);
  }
  void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                float reflection = 0.0f, float am = 0.1f, float diff = 0.9f,
                float spec = 0.9f, float shi = 200.0f) override {
    m_currentSphere =
        new Material(m_currentSphere, c, reflection, am, diff, spec, shi);
  }
  void addStripePattern(const Vec3f &colorA, const Vec3f &colorB,
                        const Mat4f &ptrans, const Mat4f &otrans) override {
    m_currentSphere =
        new StripePattern(m_currentSphere, colorA, colorB, ptrans, otrans);
  }
  void addGradientPattern(const Vec3f &colorA, const Vec3f &colorB,
                          const Mat4f &ptrans, const Mat4f &otrans) override {
    m_currentSphere =
        new GradientPattern(m_currentSphere, colorA, colorB, ptrans, otrans);
  }
  void addRingPattern(const Vec3f &colorA, const Vec3f &colorB,
                      const Mat4f &ptrans, const Mat4f &otrans) override {
    m_currentSphere =
        new RingPattern(m_currentSphere, colorA, colorB, ptrans, otrans);
  }
  void addCheckerPattern(const Vec3f &colorA, const Vec3f &colorB,
                         const Mat4f &ptrans, const Mat4f &otrans) override {
    m_currentSphere =
        new CheckerPattern(m_currentSphere, colorA, colorB, ptrans, otrans);
  }
  std::shared_ptr<Traceable> getTraceable() const override {
    std::shared_ptr<Traceable> ret(m_currentSphere);
    return ret;
  }

 private:
  Traceable *m_currentSphere;
};
