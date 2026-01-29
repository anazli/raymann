#pragma once

#include <mat4.h>

#include "acceleration/bounding_box.h"

class Transformation {
 public:
  Transformation() = default;
  Transformation(const Mat4D& matrix);
  Mat4D getMatrix() const;
  Mat4D getInverseMatrix() const;
  Mat4D getInverseTransposeMatrix() const;
  Ray worldToObjectSpace(const Ray& ray);
  Ray objectToWorldSpace(const Ray& ray);
  BoundingBox worldToObjectSpace(const BoundingBox& b) const;
  BoundingBox objectToWorldSpace(const BoundingBox& b) const;
  Point3f worldToObjectSpace(const Point3f& p) const;
  Point3f objectToWorldSpace(const Point3f& p) const;
  Vec3f worldToObjectSpace(const Vec3f& v) const;
  Vec3f objectToWorldSpace(const Vec3f& v) const;
  Normal3f worldToObjectSpace(const Normal3f& n) const;
  Normal3f objectToWorldSpace(const Normal3f& n) const;

 protected:
  Mat4D m_matrix;
  Mat4D m_inverse_matrix;
  Mat4D m_inverse_transpose_matrix;
};
