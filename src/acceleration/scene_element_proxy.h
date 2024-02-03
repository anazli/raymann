#pragma once

#include "composite/scene_element.h"

class SceneElementProxy : public SceneElement {
 public:
  SceneElementProxy(SceneElementPtr element);
  SceneElementProxy(SceneElementPtr element, const Point3f &minPoint,
                    const Point3f &maxPoint);
  ~SceneElementProxy() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  SceneElementPtr getElementOfBoundingBox() const override;
  void setElementOfBoundingBox(SceneElementPtr element) override;
  bool containsElement() const override;
  void addPoint(const Point3f &point) override;
  bool containsPoint(const Point3f &point) const override;
  bool containsBoundingBox(const BoundingBoxProperties &prop) const override;

 private:
  void applyDefaultBoundsForElementType();
  void transformBox();
  SceneElementPtr m_sceneElement;
};
