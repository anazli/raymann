#pragma once

#include <limits>
#include <list>
#include <memory>

#include "acceleration/bounding_box.h"
#include "composite/intersection.h"
#include "geometry/primitive.h"
#include "materials/material.h"
#include "tools/normal3.h"
#include "transformations/transformation.h"

class BaseRenderer;
class StandardMaterial;

enum SceneElementType { PRIMITIVE = 0, WORLD = 1 };

class SceneElementNode {
 public:
  SceneElementNode();
  virtual ~SceneElementNode() = default;
  virtual bool intersect(const Ray &r, Intersection &record);
  virtual void add(std::shared_ptr<SceneElementNode> item);
  virtual bool isWorld() const;
  virtual void accept(BaseRenderer &renderer, const Ray &ray);
  virtual std::vector<std::shared_ptr<SceneElementNode>> getChildren() const;
  virtual std::vector<std::shared_ptr<SceneElementNode>> &getChildren();
  virtual void setParent(SceneElementNode *parent);
  virtual SceneElementNode *getParent() const;
  virtual void setLight(const PointLight &light);
  virtual PointLight getLight() const;
  virtual BoundingBox getBoundingBox() const;
  void setMaterial(MaterialPtr mat);
  const MaterialRawPtr getMaterial() const;
  void setPrimitive(PrimitivePtr pr);
  PrimitivePtr getPrimitive();

  static std::shared_ptr<SceneElementNode> create();

 protected:
  SceneElementNode *m_parent = nullptr;
  MaterialPtr m_material;
  PrimitivePtr m_geometric_primitive;
  BoundingBox m_bBox;
  std::vector<std::shared_ptr<SceneElementNode>> m_children;
  PointLight m_light;
};

using SceneElementPtr = std::shared_ptr<SceneElementNode>;
using SceneElementRawPtr = SceneElementNode *;
using SceneElementContainer = std::vector<SceneElementPtr>;
