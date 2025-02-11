#include "transformation.h"

Transformation::Transformation(const Mat4D& matrix) : m_matrix(matrix) {
  m_inverse_matrix = m_matrix.inverse();
  m_inverse_transpose_matrix = m_inverse_matrix.transpose();
}

Mat4D Transformation::getMatrix() const { return m_matrix; }
Mat4D Transformation::getInverseMatrix() const { return m_inverse_matrix; }
Mat4D Transformation::getInverseTransposeMatrix() const {
  return m_inverse_transpose_matrix;
}

Ray Transformation::worldToObjectSpace(const Ray& ray) {
  auto object_space_origin = m_inverse_matrix * Vec4D(ray.origin());
  auto object_space_direction = m_inverse_matrix * Vec4D(ray.direction());
  return Ray(Point3D(object_space_origin), Vec3D(object_space_direction));
}

Ray Transformation::objectToWorldSpace(const Ray& ray) {
  auto world_space_origin = m_matrix * Vec4D(ray.origin());
  auto world_space_direction = m_matrix * Vec4D(ray.direction());
  return Ray(Point3D(world_space_origin), Vec3D(world_space_direction));
}

void Transformation::worldToObjectSpace(BoundingBox& b) const {
  Point3D p1 = b.minPoint();
  Point3D p2 = Point3D(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p3 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p4 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3D p5 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3D p6 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p7 = Point3D(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p8 = b.maxPoint();
  b = BoundingBox{};
  std::vector<Point3D> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3D& elem : v) {
    b.addPoint(Point3D(m_matrix.inverse() * Vec4D(elem)));
  }
}

void Transformation::objectToWorldSpace(BoundingBox& b) const {
  Point3D p1 = b.minPoint();
  Point3D p2 = Point3D(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p3 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p4 = Point3D(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3D p5 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3D p6 = Point3D(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3D p7 = Point3D(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3D p8 = b.maxPoint();
  b = BoundingBox{};
  std::vector<Point3D> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3D& elem : v) {
    b.addPoint(Point3D(m_matrix * Vec4D(elem)));
  }
}
