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
  return Ray(Point3f(object_space_origin), Vec3f(object_space_direction));
}

Ray Transformation::objectToWorldSpace(const Ray& ray) {
  auto world_space_origin = m_matrix * Vec4D(ray.origin());
  auto world_space_direction = m_matrix * Vec4D(ray.direction());
  return Ray(Point3f(world_space_origin), Vec3f(world_space_direction));
}

BoundingBox Transformation::worldToObjectSpace(const BoundingBox& b) const {
  Point3f p1 = b.minPoint();
  Point3f p2 = Point3f(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p3 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p4 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3f p5 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3f p6 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p7 = Point3f(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p8 = b.maxPoint();
  auto box = BoundingBox{};
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f& elem : v) {
    box.addPoint(Point3f(m_matrix.inverse() * Vec4D(elem)));
  }
  return box;
}

BoundingBox Transformation::objectToWorldSpace(const BoundingBox& b) const {
  Point3f p1 = b.minPoint();
  Point3f p2 = Point3f(b.minPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p3 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p4 = Point3f(b.minPoint().x(), b.maxPoint().y(), b.maxPoint().z());
  Point3f p5 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.minPoint().z());
  Point3f p6 = Point3f(b.maxPoint().x(), b.minPoint().y(), b.maxPoint().z());
  Point3f p7 = Point3f(b.maxPoint().x(), b.maxPoint().y(), b.minPoint().z());
  Point3f p8 = b.maxPoint();
  auto box = BoundingBox{};
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f& elem : v) {
    box.addPoint(Point3f(m_matrix * Vec4D(elem)));
  }
  return box;
}

Point3f Transformation::worldToObjectSpace(const Point3f& p) const {
  return Point3f(m_inverse_matrix * Vec4D(p));
}

Point3f Transformation::objectToWorldSpace(const Point3f& p) const {
  return Point3f(m_matrix * Vec4D(p));
}

Vec3f Transformation::worldToObjectSpace(const Vec3f& v) const {
  return Vec3f(m_inverse_matrix * Vec4D(v));
}

Vec3f Transformation::objectToWorldSpace(const Vec3f& v) const {
  return Vec3f(m_matrix * Vec4D(v));
}

Normal3f Transformation::worldToObjectSpace(const Normal3f& n) const {
  return Normal3f(m_inverse_matrix * Vec4D(n));
}

Normal3f Transformation::objectToWorldSpace(const Normal3f& n) const {
  return Normal3f(m_inverse_transpose_matrix * Vec4D(n));
}
