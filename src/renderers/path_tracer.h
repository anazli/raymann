#pragma once

#include "renderers/renderer.h"

class BasicPathTracer : public BaseRenderer {
 public:
  BasicPathTracer(const BaseCameraPtr &cam);
  void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                  const Ray &ray) override;

  Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                     int rec = 5) override;

 private:
  IntersectionRecord findClosestHit(const SceneElementPtr &world, const Ray &r);
  BaseCameraPtr m_cam;
};
