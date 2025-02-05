#pragma once
#include "composite/scene_element.h"
#include "composite/world.h"

class SceneElementDecorator : public SceneElement {
 public:
  SceneElementDecorator(SceneElementRawPtr tr, const Mat4D &m = Mat4D());
  ~SceneElementDecorator() override;
  SceneElementType elementType() const override;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3D normal(const Point3D &p) const override;
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
  float pdf(const Point3D &origin, const Vec3D &direction) override;
  Vec3D random(const Point3D &origin) override;

  Point3D pointFromWorldToObjectSpace(const Point3D &point) const override;
  Vec3D vectorFromObjectToWorldSpace(const Vec3D vec) const override;

 protected:
  SceneElementRawPtr m_element;
  Mat4D m_transformMatrix;
};
