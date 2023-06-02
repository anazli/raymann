#pragma once

#include <list>
#include <memory>
#include <string>

#include "materials/material.h"
#include "tools/tools.h"

//------------------------------------------------------------------------------
//---------------------------Helper Struct--------------------------------------
//------------------------------------------------------------------------------

class IntersectionRecord {
 public:
  int count = 0;
  float t1 = -1.0f;
  float t2 = -1.0f;
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
  Vec3f normal;
};

class BaseRenderer;

class SceneElement {
 public:
  virtual bool intersect(const Ray &r) { return false; }
  virtual void add(std::shared_ptr<SceneElement> item) {}
  virtual void remove(std::shared_ptr<SceneElement> item, bool del = true) {}
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual void accept(std::shared_ptr<BaseRenderer> &renderer, const Ray &ray);
  virtual std::list<std::shared_ptr<SceneElement>> getWorldList() {
    return std::list<std::shared_ptr<SceneElement>>();
  }

  virtual void setRecord(const IntersectionRecord &rec);
  virtual IntersectionRecord &getRecord();
  virtual void setMaterial(BaseMaterialPtr mat);
  virtual BaseMaterialPtr getMaterial() const;
  virtual void setParent(std::shared_ptr<SceneElement> t);
  virtual std::shared_ptr<SceneElement> getParent() const;
  virtual void setLight(const PointLight &light) {}
  virtual PointLight getLight() const { return PointLight(); }

 protected:
  SceneElement() {}
  std::shared_ptr<SceneElement> m_parent;
  IntersectionRecord m_rec;
  BaseMaterialPtr m_material;
};

typedef std::shared_ptr<SceneElement> SceneElementPtr;

class BaseRenderer {
 public:
  virtual void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) = 0;
  virtual void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                          const Ray &ray) = 0;
  virtual Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                             int rec = 5) = 0;

  Vec3f getColor() const { return m_out_color; }

 protected:
  SceneElementPtr m_closestHit;
  Vec3f m_out_color;
  float m_tmin = MAXFLOAT;
};

typedef std::shared_ptr<BaseRenderer> BaseRendererPtr;
