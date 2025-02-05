#pragma once

#include <limits>
#include <list>
#include <memory>

#include "acceleration/bounding_box.h"
#include "materials/material.h"

class BaseRenderer;
class Material;

enum class SceneElementType {
  CONE,
  CUBE,
  CYLINDER,
  PLANE,
  SPHERE,
  TRIANGLE,
  WORLD,
  BBOX
};

class SceneElement {
 public:
  virtual ~SceneElement() = default;
  virtual bool intersect(const Ray &r, IntersectionRecord &record) = 0;
  virtual Vec3D normal(const Point3D &p) const = 0;
  virtual void add(std::shared_ptr<SceneElement> element);
  virtual std::vector<std::shared_ptr<SceneElement>>::iterator remove(
      SceneElement *item, std::shared_ptr<SceneElement> elementToRemove);
  virtual bool isWorld() const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::vector<std::shared_ptr<SceneElement>> getChildren() const;
  virtual void setParent(SceneElement *parent);
  virtual SceneElement *getParent() const;

 protected:
  SceneElement() = default;
  SceneElement *m_parent = nullptr;
};

using SceneElementPtr = std::shared_ptr<SceneElement>;
using SceneElementRawPtr = SceneElement *;

class IntersectionRecord {
 public:
  int count = 0;
  float t1 = -limit::max();
  float t2 = -limit::max();
  float t_min() const {
    if ((t1 < limit::max() && t1 > 0.001f) ||
        (t2 < limit::max() && t2 > 0.001f)) {
      if (t1 <= 0.0f && t2 > 0.0f)
        return t2;
      else if (t2 <= 0.0f && t1 > 0.0f)
        return t1;
      else if (t1 > 0.0f && t2 > 0.0f)
        return std::min(t1, t2);
    }
    return -1.0f;  // TODO: to be fixed for negative intersections
  }
  Point3D point(const Ray &r) const { return r.position(t_min()); }
  Point3D saved_point;
  Vec3D eye(const Ray &r) const { return -r.direction(); }
  bool inside = false;
  Point3D over_point_from_refl_surf;
  Point3D under_point_from_refrac_surf;
  Vec3D normal;
  float n1, n2;
  SceneElementRawPtr object = nullptr;
  float minHitParam = limit::max();
  bool hitFound = false;
};
