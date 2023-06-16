#pragma once

/*class BasicPathTracer : public BaseRenderer {
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
*/
