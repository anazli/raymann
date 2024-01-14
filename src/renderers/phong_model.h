#pragma once

#include "renderers/renderer.h"

class PhongModel : public BaseRenderer {
 public:
  void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) override;
  void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                  const Ray &ray) override;
  Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                     int rec = 5) override;

 private:
  Vec3f reflectedColor(const SceneElementPtr &world, const Ray &ray,
                       int rec = 5);
  Vec3f lighting(const SceneElementPtr &world, const Ray &ray);
  void checkInside(const Ray &r);
  bool isShadowed(const SceneElementPtr &world, const Point3f &p);
  // Vec3f refractedColor(const SceneElementPtr &world, const Ray &ray,
  //                      int rec = 5);
  //  void determineRefractionIndices(const SceneElementPtr &world, const Ray
  //  &r); std::map<size_t, std::pair<size_t, float>> intersectionsSorted(
  //      const SceneElementPtr &world, const Ray &ray) const;
  //  SceneElementPtr findSceneElementById(const size_t &id,
  //                                       const SceneElementPtr &world);
  //  void findRefractionIndicesForClosestHit(const SceneElementPtr &world,
  //                                          const Ray &ray);
  // std::map<size_t, std::pair<float, float>> getContainer() const;

  //std::map<size_t, std::pair<float, float>> m_refract_index_collection;
};
