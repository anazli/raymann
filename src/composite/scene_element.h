#pragma once

#include <limits>
#include <list>
#include <memory>

#include "acceleration/bounding_box.h"
#include "composite/intersection.h"
#include "geometry/primitive.h"
#include "materials/material.h"
#include "math_utils/normal3.h"
#include "transformations/transformation.h"

class BaseRenderer;
class StandardMaterial;

enum SceneElementType { PRIMITIVE = 0, WORLD = 1 };

class SceneElementNode {
 public:
  SceneElementNode() = default;
  bool intersect(const Ray &r, Intersection &record);
  void add(std::shared_ptr<SceneElementNode> item);
  bool isWorld() const;
  void accept(BaseRenderer &renderer, const Ray &ray);
  std::vector<std::shared_ptr<SceneElementNode>> &getChildren();
  void setParent(SceneElementNode *parent);
  SceneElementNode *getParent() const;
  void setLight(const PointLight &light);
  PointLight getLight() const;
  BoundingBox getBounds() const;
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
