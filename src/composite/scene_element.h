#pragma once

#include <list>
#include <memory>

#include "materials/material.h"

class BaseRenderer;

class SceneElement {
 public:
  virtual ~SceneElement() = default;
  virtual bool intersect(const Ray &r, IntersectionRecord &record);
  virtual void add(std::shared_ptr<SceneElement> item);
  virtual void remove(SceneElement *item, bool del = true);
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::list<std::shared_ptr<SceneElement>> getChildren();
  virtual void setMaterial(BaseMaterialPtr mat);
  virtual BaseMaterialPtr getMaterial() const;
  virtual void setParent(SceneElement *parent);
  virtual SceneElement *getParent() const;
  virtual void setLight(const PointLight &light);
  virtual PointLight getLight() const;
  size_t getId() const;
  virtual Point3f pointFromWorldToObjectSpace(Point3f &point) const;
  virtual Vec3f vectorFromObjectToWorldSpace(Vec3f vec) const;

 protected:
  SceneElement();
  SceneElement *m_parent = nullptr;
  BaseMaterialPtr m_material;
  size_t m_id;
  static size_t m_next_id;
};

using SceneElementPtr = std::shared_ptr<SceneElement>;
using SceneElementRawPtr = SceneElement *;

class IntersectionRecord {
 public:
  int count = 0;
  float t1 = -MAXFLOAT;
  float t2 = -MAXFLOAT;
  float t_min() const {
    if (t1 <= 0.0f && t2 > 0.0f)
      return t2;
    else if (t2 <= 0.0f && t1 > 0.0f)
      return t1;
    else if (t1 > 0.0f && t2 > 0.0f)
      return std::min(t1, t2);
    return -1.0f;  // TODO: to be fixed for negative intersections
  }
  Point3f point(const Ray &r) const { return r.position(t_min()); }
  Vec3f eye(const Ray &r) const { return -r.direction(); }
  bool inside = false;
  Point3f over_point_from_refl_surf;
  Point3f under_point_from_refrac_surf;
  Vec3f normal;
  float n1, n2;
  SceneElementRawPtr object = nullptr;
};
