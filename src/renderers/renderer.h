#pragma once

#include <memory>

#include "composite/scene_element.h"
#include "tools/tools.h"

class PhongModel : public BaseRenderer {
 public:
  PhongModel();
  void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                  const Ray &ray) override;

 private:
  Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                     int rec = 5) override;
  Vec3f lighting(const SceneElementPtr &world, const Ray &ray);
  Vec3f reflectedColor(const SceneElementPtr &world, const Ray &r, int rec = 5);
  void checkInside(const Ray &r);
  bool isShadowed(const SceneElementPtr &world, const Point3f &p);
  SceneElementPtr findClosestHit(const SceneElementPtr &world, const Ray &r);
};
