#pragma once

#include "application/error.h"
#include "geometry/primitive.h"

class Triangle : public Primitive {
 public:
  Triangle(const std::initializer_list<Point3f> &points,
           const Transformation &tr = Transformation())
      : m_points(points), Primitive(tr) {
    APP_ASSERT((points.size() == 3),
               "Wrong number of points for triangle creation!");
    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);
    m_normalVec = Normal3f(getUnitVectorOf(cross(m_edgeVec[0], m_edgeVec[1])));

    m_object_box.addPoint(m_points[0]);
    m_object_box.addPoint(m_points[1]);
    m_object_box.addPoint(m_points[2]);
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }
  ~Triangle() override = default;
  Point3f point(int idx) const { return m_points[idx]; }
  Vec3f edgeVector(int idx) const {
    APP_ASSERT((idx < 2 && idx >= 0),
               "Out of bounds index error in triangle (Edge Vector)");
    return m_edgeVec[idx];
  }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto cde = cross(direction, m_edgeVec[1]);
    auto det = dot(m_edgeVec[0], cde);
    if (det > -EPS && det < EPS) return false;

    auto inv_det = 1.f / det;

    auto s = origin - m_points[0];
    auto u = inv_det * dot(s, cde);
    if (u < 0.f || u > 1.f) return false;

    auto cross_s_edge1 = cross(s, m_edgeVec[0]);
    auto v = inv_det * dot(direction, cross_s_edge1);
    if (v < 0.f || (u + v) > 1.f) return false;

    record.thit = inv_det * dot(m_edgeVec[1], cross_s_edge1);
    record.hit_point = record.getHitPoint(transf_ray);
    record.normal = normal(record.hit_point);
    return true;
  }
  Normal3f normal(const Point3f &p) const override { return m_normalVec; }

  static PrimitivePtr create(const std::initializer_list<Point3f> &points,
                             const Transformation &tr = Transformation()) {
    return std::make_shared<Triangle>(points, tr);
  }

 private:
  std::vector<Point3f> m_points;
  std::vector<Vec3f> m_edgeVec;
  Normal3f m_normalVec;
};

//-----------------------------------------
// TODO: Refactor and remove duplicate code
//-----------------------------------------
class SmoothTriangle : public Primitive {
 public:
  SmoothTriangle(const Point3f &p1, const Point3f &p2, const Point3f &p3,
                 const Normal3f &v1, const Normal3f &v2, const Normal3f &v3,
                 const Transformation &tr = Transformation())
      : Primitive(tr) {
    m_points.push_back(p1);
    m_points.push_back(p2);
    m_points.push_back(p3);

    m_normals.push_back(v1);
    m_normals.push_back(v2);
    m_normals.push_back(v3);

    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);

    m_object_box.addPoint(m_points[0]);
    m_object_box.addPoint(m_points[1]);
    m_object_box.addPoint(m_points[2]);
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }

  Point3f points(int idx) const {
    APP_ASSERT((idx < 3 && idx >= 0),
               "Out of bounds index error in smooth triangle (Points)");
    return m_points[idx];
  }

  Normal3f normals(int idx) const {
    APP_ASSERT((idx < 3 && idx >= 0),
               "Out of bounds index error in smooth triangle (Points)");
    return m_normals[idx];
  }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto cde = cross(direction, m_edgeVec[1]);
    auto det = dot(m_edgeVec[0], cde);
    if (det > -EPS && det < EPS) return false;

    auto inv_det = 1.f / det;

    auto s = origin - m_points[0];
    m_uPar = inv_det * dot(s, cde);
    if (m_uPar < 0.f || m_uPar > 1.f) return false;

    auto cross_s_edge1 = cross(s, m_edgeVec[0]);
    m_vPar = inv_det * dot(direction, cross_s_edge1);
    if (m_vPar < 0.f || (m_uPar + m_vPar) > 1.f) return false;

    record.thit = inv_det * dot(m_edgeVec[1], cross_s_edge1);
    record.hit_point = record.getHitPoint(transf_ray);
    record.normal = normal(record.hit_point);
    return true;
  }

  Normal3f normal(const Point3f &p) const override {
    return m_normals[1] * m_uPar + m_normals[2] * m_vPar +
           m_normals[0] * (1.f - m_uPar - m_vPar);
  }

  static PrimitivePtr create(const Point3f &p1, const Point3f &p2,
                             const Point3f &p3, const Normal3f &v1,
                             const Normal3f &v2, const Normal3f &v3,
                             const Transformation &tr = Transformation()) {
    return std::make_shared<SmoothTriangle>(p1, p2, p3, v1, v2, v3, tr);
  }

 private:
  std::vector<Point3f> m_points;
  std::vector<Normal3f> m_normals;
  std::vector<Vec3f> m_edgeVec;
  float m_uPar;
  float m_vPar;
};
