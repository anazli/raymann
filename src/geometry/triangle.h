#pragma once

#include "application/error.h"
#include "composite/scene_element.h"

class Triangle : public SceneElement {
 public:
  Triangle(const std::initializer_list<Point3f> &points) : m_points(points) {
    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);
    m_normalVec = getUnitVectorOf(cross(m_edgeVec[0], m_edgeVec[1]));

    m_bBox.addPoint(m_points[0]);
    m_bBox.addPoint(m_points[1]);
    m_bBox.addPoint(m_points[2]);
  }
  ~Triangle() override = default;
  Point3f point(int idx) const {
    APP_ASSERT((idx < 3 && idx >= 0),
               "Out of bounds index error in triangle (Points)");
    return m_points[idx];
  }
  Vec3f edgeVector(int idx) const {
    APP_ASSERT((idx < 2 && idx >= 0),
               "Out of bounds index error in triangle (Edge Vector)");
    return m_edgeVec[idx];
  }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto cde = cross(r.direction(), m_edgeVec[1]);
    auto det = dot(m_edgeVec[0], cde);
    if (det > -EPS && det < EPS) return false;

    auto inv_det = 1.f / det;

    auto s = r.origin() - m_points[0];
    auto u = inv_det * dot(s, cde);
    if (u < 0.f || u > 1.f) return false;

    auto cross_s_edge1 = cross(s, m_edgeVec[0]);
    auto v = inv_det * dot(r.direction(), cross_s_edge1);
    if (v < 0.f || (u + v) > 1.f) return false;

    record.t1 = inv_det * dot(m_edgeVec[1], cross_s_edge1);
    record.count++;
    record.saved_point = record.point(r);
    return true;
  }
  Vec3f normal(const Point3f &p) const override { return m_normalVec; }

 private:
  std::vector<Point3f> m_points;
  std::vector<Vec3f> m_edgeVec;
  Vec3f m_normalVec;
};

//-----------------------------------------
// TODO: Refactor and remove duplicate code
//-----------------------------------------
class SmoothTriangle : public SceneElement {
 public:
  SmoothTriangle(const Point3f &p1, const Point3f &p2, const Point3f &p3,
                 const Vec3f &v1, const Vec3f &v2, const Vec3f &v3) {
    m_points.push_back(p1);
    m_points.push_back(p2);
    m_points.push_back(p3);

    m_normals.push_back(v1);
    m_normals.push_back(v2);
    m_normals.push_back(v3);

    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);

    m_bBox.addPoint(m_points[0]);
    m_bBox.addPoint(m_points[1]);
    m_bBox.addPoint(m_points[2]);
  }

  Point3f points(int idx) const {
    APP_ASSERT((idx < 3 && idx >= 0),
               "Out of bounds index error in smooth triangle (Points)");
    return m_points[idx];
  }

  Vec3f normals(int idx) const {
    APP_ASSERT((idx < 3 && idx >= 0),
               "Out of bounds index error in smooth triangle (Points)");
    return m_normals[idx];
  }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto cde = cross(r.direction(), m_edgeVec[1]);
    auto det = dot(m_edgeVec[0], cde);
    if (det > -EPS && det < EPS) return false;

    auto inv_det = 1.f / det;

    auto s = r.origin() - m_points[0];
    m_uPar = inv_det * dot(s, cde);
    if (m_uPar < 0.f || m_uPar > 1.f) return false;

    auto cross_s_edge1 = cross(s, m_edgeVec[0]);
    m_vPar = inv_det * dot(r.direction(), cross_s_edge1);
    if (m_vPar < 0.f || (m_uPar + m_vPar) > 1.f) return false;

    record.t1 = inv_det * dot(m_edgeVec[1], cross_s_edge1);
    record.count++;
    return true;
  }

  Vec3f normal(const Point3f &p) const override {
    return m_normals[1] * m_uPar + m_normals[2] * m_vPar +
           m_normals[0] * (1.f - m_uPar - m_vPar);
  }

 private:
  std::vector<Point3f> m_points;
  std::vector<Vec3f> m_normals;
  std::vector<Vec3f> m_edgeVec;
  float m_uPar;
  float m_vPar;
};
