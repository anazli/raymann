#include "geometry/primitive.h"

#include "primitive.h"

void Primitive::setMaterial(BaseMaterialPtr material) { m_material = material; }

BaseMaterialPtr Primitive::getMaterial() const { return m_material; }

void Primitive::setBoundingBox(const BoundingBox& box) { m_bBox = box; }

BoundingBox Primitive::boundingBox() const { return m_bBox; }

Point3D Primitive::pointFromWorldToObjectSpace(const Point3D& point) const {
  return Mat4D().inverse() * Vec4D(point);
}

Vec3D Primitive::vectorFromObjectToWorldSpace(const Vec3D vec) const {
  Vec3D v(vec);
  v = Mat4D().inverse().transpose() * Vec4D(v);
  v.normalize();
  return v;
}