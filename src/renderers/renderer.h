#pragma once

#include <memory>

#include "camera/camera.h"
#include "composite/scene_element.h"
#include "tools/tools.h"

class PhongModel : public BaseRenderer {
 public:
  PhongModel();
  void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                  const Ray &ray) override;

  Vec3f reflectedColor(const SceneElementPtr &world, const Ray &r, int rec = 5);
  Vec3f refractedColor(const SceneElementPtr &world, const Ray &r);
  void determineRefraction(const SceneElementPtr &world, const Ray &r);

  std::map<size_t, std::pair<float, float>> getContainer() const;

 private:
  Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                     int rec = 5) override;
  Vec3f lighting(const SceneElementPtr &world, const Ray &ray);
  void checkInside(const Ray &r);
  bool isShadowed(const SceneElementPtr &world, const Point3f &p);
  SceneElementPtr findClosestHit(const SceneElementPtr &world, const Ray &r);
  std::map<size_t, std::pair<size_t, float>> intersectionsSorted(
      const SceneElementPtr &world) const;
  SceneElementPtr findSceneElementById(const size_t &id,
                                       const SceneElementPtr &world);

  std::map<size_t, std::pair<float, float>> m_refract_index_collection;
};

class BasicPathTracer : public BaseRenderer {
 public:
  BasicPathTracer(const BaseCamera &cam);
  void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                  const Ray &ray) override;

  Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                     int rec = 5) override;

 private:
  SceneElementPtr findClosestHit(const SceneElementPtr &world, const Ray &r);
  BaseCamera m_cam;
};
