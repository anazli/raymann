#pragma once

#include <limits>
#include <list>
#include <memory>

#include "acceleration/bounding_box.h"
#include "composite/intersection.h"
#include "materials/material.h"
#include "transformations/transformation.h"

class BaseRenderer;
class StandardMaterial;

class SceneElement {
 public:
  virtual ~SceneElement() = default;
  virtual bool intersect(const Ray &r, Intersection &record);
  virtual void add(std::shared_ptr<SceneElement> item);
  virtual std::vector<std::shared_ptr<SceneElement>>::iterator remove(
      SceneElement *item, std::shared_ptr<SceneElement> removedElem);
  virtual bool isWorld() const;
  virtual Vec3D normal(const Point3D &p) const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::vector<std::shared_ptr<SceneElement>> getChildren() const;
  virtual std::vector<std::shared_ptr<SceneElement>> &getChildren();
  virtual void setParent(SceneElement *parent);
  virtual SceneElement *getParent() const;
  virtual void setLight(const PointLight &light);
  virtual PointLight getLight() const;
  virtual void setBoundingBox(const BoundingBox &box);
  virtual BoundingBox getBoundingBox() const;
  virtual float pdf(const Point3D &origin, const Vec3D &direction);
  virtual Vec3D random(const Point3D &origin);
  void setMaterial(MaterialPtr mat);
  const MaterialRawPtr getMaterial() const;
  void setTransformation(const Transformation &transformation);

 protected:
  SceneElement();
  SceneElement(const BoundingBox &props);
  SceneElement *m_parent = nullptr;
  MaterialPtr m_material;
  BoundingBox m_bBox;
  Transformation m_transformation;
};

using SceneElementPtr = std::shared_ptr<SceneElement>;
using SceneElementRawPtr = SceneElement *;
