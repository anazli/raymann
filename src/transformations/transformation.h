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
  void transformBoundingBox(BoundingBox& b) const;

 protected:
  Mat4D m_matrix;
  Mat4D m_inverse_matrix;
  Mat4D m_inverse_transpose_matrix;
};
