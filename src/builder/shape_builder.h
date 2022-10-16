#pragma once
#include "builder/builder.h"
#include "decorators/material.h"
#include "decorators/transformer.h"
#include "geometry/sphere.h"

class ShapeBuilder : public TraceableBuilder {
 public:
  void addTraceable(Traceable *tr) override;
  void addTransformer() override;
  void addMaterial() override;
  void addStripePattern() override;
  void addGradientPattern() override;
  void addRingPattern() override;
  void addCheckerPattern() override;
  std::shared_ptr<Traceable> getTraceable() const override;

 private:
  Traceable *m_currentTraceable;
};
