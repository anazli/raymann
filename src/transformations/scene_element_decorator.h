#pragma once
#include "composite/scene_element.h"
#include "composite/world.h"

class SceneElementDecorator : public SceneElement {
 public:
  SceneElementDecorator(SceneElementRawPtr tr, const Mat4f &m = Mat4f());
  ~SceneElementDecorator() override;
  SceneElementType elementType() const override;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override;
  void add(SceneElementPtr item) override;
  void remove(SceneElementRawPtr item, bool del = true) override;
  bool isWorld() const override;
  void accept(BaseRenderer &renderer, const Ray &ray) override;
  SceneElementContainer getChildren() override;
  void setParent(SceneElementRawPtr parent) override;
  SceneElementRawPtr getParent() const override;
  void setBoundingBox(const BoundingBox &box) override;
  BoundingBox &boundingBox() override;
  const BoundingBox &boundingBox() const override;

  Point3f pointFromWorldToObjectSpace(const Point3f &point) const override;
  Vec3f vectorFromObjectToWorldSpace(const Vec3f vec) const override;

 protected:
  SceneElementRawPtr m_element;
  Mat4f m_transformMatrix;
};
