#pragma once

#include <random>

#include "renderers/renderer.h"

class StochasticMethod {
 public:
  StochasticMethod(const BaseCameraPtr &cam, int pixelSamples = 50,
                   int materialDepth = 30);
  virtual ~StochasticMethod() = default;
  virtual Vec3f computeColor(BaseRendererRawPtr renderer,
                             const SceneElementRawPtr world) = 0;
  float randomNumber();

 protected:
  BaseCameraPtr m_camera;
  int m_samplesPerPixel;
  int m_materialDepth;
  std::uniform_real_distribution<float> rand;
  std::mt19937 gen;
};

using StochasticMethodPtr = std::unique_ptr<StochasticMethod>();

class BruteForceMC : public StochasticMethod {
 public:
  BruteForceMC(const BaseCameraPtr &cam, int pixelSamples = 50,
               int materialDepth = 30);
  ~BruteForceMC() override = default;
  Vec3f computeColor(BaseRendererRawPtr renderer,
                     const SceneElementRawPtr world) override;
};
