#pragma once

#include <memory>

#include "composite/properties.h"
#include "composite/traceable.h"

class Builder {
 public:
  virtual void createWorld(const PointLight &light) = 0;
  virtual void addWorld(const PointLight &light) = 0;
  virtual void addElement() = 0;
  virtual void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                            const float &r = 1.0f) = 0;
  virtual void createPlane() = 0;
  virtual void applyTransformation(const Mat4f &trans) = 0;
  virtual void applyMaterial(const Properties &prop) = 0;
  virtual void applyStripPattern(const Properties &prop) = 0;
  virtual void applyGradientPattern(const Properties &prop) = 0;
  virtual void applyRingPattern(const Properties &prop) = 0;
  virtual void applyCheckerPattern(const Properties &prop) = 0;
  virtual std::shared_ptr<Traceable> getProduct() = 0;
};

typedef std::shared_ptr<Builder> BuilderPtr;

class WorldBuilder : public Builder {
 public:
  WorldBuilder();
  void createWorld(const PointLight &light) override;
  void addWorld(const PointLight &light) override;
  void addElement() override;
  void createSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                    const float &r = 1.0f) override;
  void createPlane() override;
  void applyTransformation(const Mat4f &trans) override;
  void applyMaterial(const Properties &prop) override;
  void applyStripPattern(const Properties &prop) override;
  void applyGradientPattern(const Properties &prop) override;
  void applyRingPattern(const Properties &prop) override;
  void applyCheckerPattern(const Properties &prop) override;
  std::shared_ptr<Traceable> getProduct() override;

 private:
  Traceable *m_currentTraceable;
  TraceablePtr m_product;
};
