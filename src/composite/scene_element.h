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
  virtual SceneElementType elementType() const;
  virtual bool intersect(const Ray &r, IntersectionRecord &record);
  virtual void add(std::shared_ptr<SceneElement> item);
  virtual std::vector<std::shared_ptr<SceneElement>>::iterator remove(
      SceneElement *item, std::shared_ptr<SceneElement> removedElem);
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::vector<std::shared_ptr<SceneElement>> getChildren() const;
  virtual std::vector<std::shared_ptr<SceneElement>> &getChildren();
  virtual void setParent(SceneElement *parent);
  virtual SceneElement *getParent() const;
  virtual void setLight(const PointLight &light);
  virtual PointLight getLight() const;
  virtual void setBoundingBox(const BoundingBox &box);
  virtual BoundingBox &boundingBox();
  virtual const BoundingBox &boundingBox() const;
  virtual float pdf(const Point3f &origin, const Vec3f &direction);
  virtual Vec3f random(const Point3f &origin);
  void setMaterial(BaseMaterialPtr mat);
  BaseMaterialPtr getMaterial() const;
  // size_t getId() const;
  virtual Point3f pointFromWorldToObjectSpace(const Point3f &point) const;
  virtual Vec3f vectorFromObjectToWorldSpace(const Vec3f vec) const;

 protected:
  SceneElement() = default;
  SceneElement(const BoundingBox &props);
  SceneElement *m_parent = nullptr;
  BaseMaterialPtr m_material;
  BoundingBox m_bBox;
  SceneElementType m_elementType;
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
  Point3f point(const Ray &r) const { return r.position(t_min()); }
  Point3f saved_point;
  Vec3f eye(const Ray &r) const { return -r.direction(); }
  bool inside = false;
  Point3f over_point_from_refl_surf;
  Point3f under_point_from_refrac_surf;
  Vec3f normal;
  float n1, n2;
  SceneElementRawPtr object = nullptr;
  float minHitParam = limit::max();
  bool hitFound = false;
};
