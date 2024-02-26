#pragma once

#include "renderers/renderer.h"

class PathTracer : public BaseRenderer {
 public:
  PathTracer(std::unique_ptr<StochasticSampler> stMethod);
  ~PathTracer() override = default;
  void visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                             const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementRawPtr elementComp,
                                  const Ray &ray) override;
  Vec3f computeColor(const SceneElementRawPtr world, const Ray &ray,
                     int rec = 5) override;

  void attachStochasticMethod(
      std::unique_ptr<StochasticSampler> stMethod) override;

 private:
  std::unique_ptr<StochasticSampler> m_stochasticSampler;
};
