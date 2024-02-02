#pragma once

#include <limits>
#include <list>
#include <memory>

#include "materials/material.h"

using limit = std::numeric_limits<float>;

class BaseRenderer;

class BoundingBoxProperties {
 public:
  BoundingBoxProperties() = default;
  BoundingBoxProperties(const Point3f &pmin, const Point3f &pmax);
  Point3f &minPoint();
  Point3f &maxPoint();

 private:
  Point3f m_minPoint;
  Point3f m_maxPoint;
};

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
  virtual void setParent(SceneElement *parent);
  virtual SceneElement *getParent() const;
  virtual void setLight(const PointLight &light);
  virtual PointLight getLight() const;
  void setBoundingBoxProperties(const BoundingBoxProperties &props);
  BoundingBoxProperties &boundingBoxProperties();
  void setMaterial(BaseMaterialPtr mat);
  BaseMaterialPtr getMaterial() const;
  // size_t getId() const;
  virtual Point3f pointFromWorldToObjectSpace(const Point3f &point) const;
  virtual Vec3f vectorFromObjectToWorldSpace(const Vec3f vec) const;

 protected:
  SceneElement() = default;
  SceneElement(const BoundingBoxProperties &props);
  SceneElement *m_parent = nullptr;
  BaseMaterialPtr m_material;
  BoundingBoxProperties m_bBoxProps;
  // size_t m_id;
  // static size_t m_next_id;
};

using SceneElementPtr = std::shared_ptr<SceneElement>;
using SceneElementRawPtr = SceneElement *;

class IntersectionRecord {
 public:
  int count = 0;
  float t1 = -limit::max();
  float t2 = -limit::max();
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
