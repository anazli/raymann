#include "acceleration/bounding_box.h"

BoundingBox::BoundingBox(const Point3D& pmin, const Point3D& pmax)
    : m_minPoint(pmin), m_maxPoint(pmax) {}

Point3D& BoundingBox::minPoint() { return m_minPoint; }

Point3D& BoundingBox::maxPoint() { return m_maxPoint; }

const Point3D& BoundingBox::minPoint() const { return m_minPoint; }

const Point3D& BoundingBox::maxPoint() const { return m_maxPoint; }

void BoundingBox::addPoint(const Point3D& point) {
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

bool BoundingBox::containsPoint(const Point3D& point) const {
  // three-way comparison doesn't work
  return point.x() >= m_minPoint.x() && point.x() <= m_maxPoint.x() &&
         point.y() >= m_minPoint.y() && point.y() <= m_maxPoint.y() &&
         point.z() >= m_minPoint.z() && point.z() <= m_maxPoint.z();
}

bool BoundingBox::containsBoundingBox(const BoundingBox& box) const {
  return containsPoint(box.minPoint()) && containsPoint(box.maxPoint());
}

bool BoundingBox::intersectsRay(const Ray& r) const {
  auto origin = r.origin();
  auto direction = r.direction();
  auto xMinMax =
      hitAxis(origin.x(), direction.x(), m_minPoint.x(), m_maxPoint.x());
  auto yMinMax =
      hitAxis(origin.y(), direction.y(), m_minPoint.y(), m_maxPoint.y());
  auto zMinMax =
      hitAxis(origin.z(), direction.z(), m_minPoint.z(), m_maxPoint.z());
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
