#pragma once

#include <random>

#include "renderers/renderer.h"

class StochasticSampler {
 public:
  StochasticSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  virtual ~StochasticSampler() = default;
  virtual Vec3f computeColor(BaseRendererRawPtr renderer,
                             const SceneElementRawPtr world) = 0;
  virtual float scatteringPDF(const Ray &r, const IntersectionRecord &record,
                              const Ray &scatteredRay) const;
  virtual float pdf() const;
  void setPdf(float val);
  float randomNumber();
  float randomNumber(float a, float b);

 protected:
  BaseCameraPtr m_camera;
  int m_samplesPerPixel;
  int m_materialDepth;
  std::uniform_real_distribution<float> rand;
  std::mt19937 gen;
  float m_pdf;
};

using StochasticSamplerPtr = std::unique_ptr<StochasticSampler>;

class BruteForceSampler : public StochasticSampler {
 public:
  BruteForceSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~BruteForceSampler() override = default;
  Vec3f computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
  float scatteringPDF(const Ray &r, const IntersectionRecord &record,
                      const Ray &scatteredRay) const override;
  float pdf() const override;
};

class StratifiedSampler : public StochasticSampler {
 public:
  StratifiedSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~StratifiedSampler() override = default;
  Vec3f computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
  // float scatteringPDF(const Ray &r, const IntersectionRecord &record,
  //                     const Ray &scatteredRay) const override;
  // float pdf() const override;
};
