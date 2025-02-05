#pragma once

#include "renderers/renderer.h"

class StochasticSampler {
 public:
  StochasticSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  virtual ~StochasticSampler() = default;
  virtual Vec3D computeColor(BaseRendererRawPtr renderer,
                             const SceneElementRawPtr world) = 0;

 protected:
  Vec3D colorCorrection(Vec3D &color);
  void addRandomSample(Vec3D &outputColor, const Vec3D &randomSampleColor);
  BaseCameraPtr m_camera;
  int m_samplesPerPixel;
  int m_materialDepth;
};

using StochasticSamplerPtr = std::unique_ptr<StochasticSampler>;

class BruteForceSampler : public StochasticSampler {
 public:
  BruteForceSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~BruteForceSampler() override = default;
  Vec3D computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
};

class StratifiedSampler : public StochasticSampler {
 public:
  StratifiedSampler(const BaseCameraPtr &cam, int pixelSamples = 50,
                    int materialDepth = 30);
  ~StratifiedSampler() override = default;
  Vec3D computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
};
