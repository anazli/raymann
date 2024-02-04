#include "acceleration/bounding_box.h"

BoundingBox::BoundingBox(const Point3f& pmin, const Point3f& pmax)
    : m_minPoint(pmin), m_maxPoint(pmax) {}

Point3f& BoundingBox::minPoint() { return m_minPoint; }

Point3f& BoundingBox::maxPoint() { return m_maxPoint; }

const Point3f& BoundingBox::minPoint() const { return m_minPoint; }

const Point3f& BoundingBox::maxPoint() const { return m_maxPoint; }

void BoundingBox::addPoint(const Point3f& point) {
  if (point.x() < m_minPoint.x()) m_minPoint.setX(point.x());
  if (point.y() < m_minPoint.y()) m_minPoint.setY(point.y());
  if (point.z() < m_minPoint.z()) m_minPoint.setZ(point.z());

  if (point.x() > m_maxPoint.x()) m_maxPoint.setX(point.x());
  if (point.y() > m_maxPoint.y()) m_maxPoint.setY(point.y());
  if (point.z() > m_maxPoint.z()) m_maxPoint.setZ(point.z());
}

void BoundingBox::addBox(const BoundingBox& box) {
  addPoint(box.minPoint());
  addPoint(box.maxPoint());
}

bool BoundingBox::containsPoint(const Point3f& point) const {
  // three-way comparison doesn't work
  return point.x() >= m_minPoint.x() && point.x() <= m_maxPoint.x() &&
         point.y() >= m_minPoint.y() && point.y() <= m_maxPoint.y() &&
         point.z() >= m_minPoint.z() && point.z() <= m_maxPoint.z();
}

bool BoundingBox::containsBoundingBox(const BoundingBox& box) const {
  return containsPoint(box.minPoint()) && containsPoint(box.maxPoint());
}

bool BoundingBox::intersectsRay(const Ray& r) const {
  std::pair<float, float> xMinMax = hitAxis(r.origin().x(), r.direction().x(),
                                            m_minPoint.x(), m_maxPoint.x());
  std::pair<float, float> yMinMax = hitAxis(r.origin().y(), r.direction().y(),
                                            m_minPoint.y(), m_maxPoint.y());
  std::pair<float, float> zMinMax = hitAxis(r.origin().z(), r.direction().z(),
                                            m_minPoint.z(), m_maxPoint.z());
  auto tmin = std::max(std::max(xMinMax.first, yMinMax.first), zMinMax.first);
  auto tmax =
      std::min(std::min(xMinMax.second, yMinMax.second), zMinMax.second);
  if (tmin > tmax) {
    return false;
  }
  return true;
}

std::pair<float, float> BoundingBox::hitAxis(float origin, float direction,
                                             float min, float max) const {
  auto tmin_numerator = (min - origin);
  auto tmax_numerator = (max - origin);
  auto tmin{0.f}, tmax{0.f};
  if (fabs(direction) >= EPS) {
    tmin = tmin_numerator / direction;
    tmax = tmax_numerator / direction;
  } else {
    tmin = tmin_numerator * std::numeric_limits<float>::infinity();
    tmax = tmax_numerator * std::numeric_limits<float>::infinity();
  }
  if (tmin > tmax) {
    std::swap(tmin, tmax);
  }
  return std::make_pair(tmin, tmax);
}
