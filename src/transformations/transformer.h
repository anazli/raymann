#pragma once
#include "transformations/scene_element_decorator.h"

class Transformer : public SceneElementDecorator {
 public:
  ~Transformer() override = default;
  Transformer(SceneElementRawPtr tr, const Mat4f &m = Mat4f());
  SceneElementType elementType() const override;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override;
  void add(SceneElementPtr item) override;
  SceneElementContainer::iterator remove(SceneElementRawPtr item,
                                         SceneElementPtr removedElem) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() const override;
  SceneElementContainer &getChildren() override;
  void setParent(SceneElementRawPtr parent) override;
  SceneElementRawPtr getParent() const override;
  void setBoundingBox(const BoundingBox &box) override;
  BoundingBox &boundingBox() override;
  const BoundingBox &boundingBox() const override;
  float pdf(const Point3f &origin, const Vec3f &direction) override;
  Vec3f random(const Point3f &origin) override;
  Point3f pointFromWorldToObjectSpace(const Point3f &point) const override;
  Vec3f vectorFromObjectToWorldSpace(const Vec3f vec) const override;

 private:
  void transformBox();
  Mat4f m_inverseTransf;        // caching
  Mat4f m_inverseTransfTransp;  // caching
};
