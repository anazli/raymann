#pragma once

#include "acceleration/bounding_box.h"
#include "tools/mat4.h"

class Transformation {
 public:
  Transformation() = default;
  Transformation(const Mat4D& matrix);
  Mat4D getMatrix() const;
  Mat4D getInverseMatrix() const;
  Mat4D getInverseTransposeMatrix() const;
  Ray worldToObjectSpace(const Ray& ray);
  Ray objectToWorldSpace(const Ray& ray);
  void worldToObjectSpace(BoundingBox& b) const;
  void objectToWorldSpace(BoundingBox& b) const;
  Point3D worldToObjectSpace(Point3D& p) const;
  Point3D objectToWorldSpace(Point3D& p) const;
  Vec3D worldToObjectSpace(Vec3D& v) const;
  Vec3D objectToWorldSpace(Vec3D& v) const;

 protected:
  Mat4D m_matrix;
  Mat4D m_inverse_matrix;
  Mat4D m_inverse_transpose_matrix;
};
