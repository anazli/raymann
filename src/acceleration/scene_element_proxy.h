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
  // SceneElementPtr getSceneElement() const;
  // void setSceneElement(SceneElementPtr element);
  void addPoint(const Point3f &point);

 private:
  SceneElementPtr m_sceneElement;
};
