#pragma once

#include <mat4.h>
#include <normal3.h>

#include "acceleration/bounding_box.h"

class Transformation {
 public:
  Transformation() = default;
  Transformation(const Mat4f& matrix);
  Mat4f getMatrix() const;
  Mat4f getInverseMatrix() const;
  Mat4f getInverseTransposeMatrix() const;
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
  Mat4f m_matrix;
  Mat4f m_inverse_matrix;
  Mat4f m_inverse_transpose_matrix;
};
