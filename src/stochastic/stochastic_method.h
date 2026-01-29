#pragma once

#include "renderers/renderer.h"

class StochasticSampler {
 public:
  StochasticSampler(const Camera &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  virtual ~StochasticSampler() = default;
  virtual Vec3f computeColor(BaseRendererRawPtr renderer,
                             const SceneElementRawPtr world) = 0;

 protected:
  Vec3f colorCorrection(Vec3f &color);
  void addRandomSample(Vec3f &outputColor, const Vec3f &randomSampleColor);
  Camera m_camera;
  int m_samplesPerPixel;
  int m_materialDepth;
};

using StochasticSamplerPtr = std::unique_ptr<StochasticSampler>;

class BruteForceSampler : public StochasticSampler {
 public:
  BruteForceSampler(const Camera &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~BruteForceSampler() override = default;
  Vec3f computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
};

class StratifiedSampler : public StochasticSampler {
 public:
  StratifiedSampler(const Camera &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~StratifiedSampler() override = default;
  Vec3f computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
};
