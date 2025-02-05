#pragma once

#include "composite/scene_element.h"

class Primitive : public SceneElement {
 public:
  ~Primitive() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override = 0;
  Vec3D normal(const Point3D &p) const override = 0;
  void setMaterial(BaseMaterialPtr material);
  BaseMaterialPtr getMaterial() const;
  void setBoundingBox(const BoundingBox &box);
  BoundingBox boundingBox() const;
  virtual float pdf(const Point3D &origin, const Vec3D &direction);
  virtual Vec3D random(const Point3D &origin);

 protected:
  Point3D pointFromWorldToObjectSpace(const Point3D &point) const;
  Vec3D vectorFromObjectToWorldSpace(const Vec3D vec) const;

  BoundingBox m_bBox;
  BaseMaterialPtr m_material;
};

using PrimitivePtr = std::shared_ptr<Primitive>;
using PrimitiveRawPtr = Primitive *;