#pragma once

#include "application/error.h"
#include "composite/scene_element.h"

class Triangle : public SceneElement {
 public:
  Triangle(const std::initializer_list<Point3f>& points) : m_points(points) {
    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);
    m_normalVec = cross(m_edgeVec[1], m_edgeVec[0]).normalize();

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

  bool intersect(const Ray& r, IntersectionRecord& record) override {
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
    return true;
  }
  Vec3f normal(const Point3f& p) const override { return m_normalVec; }

 private:
  std::vector<Point3f> m_points;
  std::vector<Vec3f> m_edgeVec;
  Vec3f m_normalVec;
};
