#pragma once

#include "renderers/renderer.h"

class BruteForceMC : public BaseRenderer {
 public:
  BruteForceMC(const BaseCameraPtr &cam, int samples = 10);
  ~BruteForceMC() override = default;
  void visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                             const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementRawPtr elementComp,
                                  const Ray &ray) override;
  Vec3f computeColor(const SceneElementRawPtr world, const Ray &ray,
                     int rec = 5) override;

 private:
  int m_samples;
};
