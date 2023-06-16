#pragma once

#include <list>
#include <memory>
#include <string>

#include "materials/material.h"
#include "tools/tools.h"

//------------------------------------------------------------------------------
//---------------------------Helper Struct--------------------------------------
//------------------------------------------------------------------------------

class BaseRenderer;

class SceneElement {
 public:
  virtual bool intersect(const Ray &r, IntersectionRecord &record) {
    return false;
  }
  virtual void add(std::shared_ptr<SceneElement> item) {}
  virtual void remove(std::shared_ptr<SceneElement> item, bool del = true) {}
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::list<std::shared_ptr<SceneElement>> getWorldList() {
    return std::list<std::shared_ptr<SceneElement>>();
  }

  virtual void setMaterial(BaseMaterialPtr mat);
  virtual BaseMaterialPtr getMaterial() const;
  virtual void setParent(std::shared_ptr<SceneElement> t);
  virtual std::shared_ptr<SceneElement> getParent() const;
  virtual void setLight(const PointLight &light) {}
  virtual PointLight getLight() const { return PointLight(); }
  size_t getId() const;

 protected:
  SceneElement();
  std::shared_ptr<SceneElement> m_parent;
  BaseMaterialPtr m_material;
  size_t m_id;
  static size_t m_next_id;
};

typedef std::shared_ptr<SceneElement> SceneElementPtr;
