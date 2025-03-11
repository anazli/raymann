#pragma once

#include <memory>

#include "composite/intersection.h"
#include "tools/normal3.h"
#include "tools/point3.h"
#include "transformations/transformation.h"

class Primitive {
 public:
  virtual ~Primitive() = default;
  void setTransformation(const Transformation &transformation) {
    m_transformation = transformation;
  }
  virtual bool intersect(const Ray &r, Intersection &record) = 0;
  virtual Normal3D normal(const Point3D &p) const = 0;
  BoundingBox objectBounds() const { return m_object_box; }
  BoundingBox worldBounds() const { return m_world_box; }

 protected:
  Primitive(const Transformation &tr) : m_transformation(tr) {}

  Transformation m_transformation;
  BoundingBox m_world_box;
  BoundingBox m_object_box;
};

using PrimitivePtr = std::shared_ptr<Primitive>;