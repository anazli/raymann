#pragma once

#include "composite/scene_element.h"

class SceneElementProxy : public SceneElement {
 public:
  SceneElementProxy(SceneElementPtr element,
                    const Point3f &minPoint = Point3f(limit::infinity(),
                                                      limit::infinity(),
                                                      limit::infinity()),
                    const Point3f &maxPoint = Point3f(-limit::infinity(),
                                                      -limit::infinity(),
                                                      -limit::infinity()));
  ~SceneElementProxy() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override;
  void add(SceneElementPtr item) override;
  SceneElementPtr getElementOfBoundingBox() const override;
  void setElementOfBoundingBox(SceneElementPtr element) override;
  bool containsElement() const override;
  void addPoint(const Point3f &point) override;
  bool containsPoint(const Point3f &point) override;

 private:
  SceneElementPtr m_sceneElement;
};
