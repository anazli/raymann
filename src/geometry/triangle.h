#pragma once

#include "application/error.h"
#include "composite/scene_element.h"

class Triangle : public SceneElement {
 public:
  Triangle(const std::initializer_list<Point3f>& points) : m_points(points) {
    preComputeParameters();
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
    return false;
  }
  Vec3f normal(const Point3f& p) const override { return m_normalVec; }

 private:
  void preComputeParameters() {
    m_edgeVec.push_back(m_points[1] - m_points[0]);
    m_edgeVec.push_back(m_points[2] - m_points[0]);
    m_normalVec = cross(m_edgeVec[1], m_edgeVec[0]).normalize();
  }
  std::vector<Point3f> m_points;
  std::vector<Vec3f> m_edgeVec;
  Vec3f m_normalVec;
};
