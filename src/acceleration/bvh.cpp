#include "acceleration/bvh.h"

BoundingBoxPair BVHierarchy::splitBoundsOf(const BoundingBox &box) {
  auto dx = box.maxPoint().x() - box.minPoint().x();
  auto dy = box.maxPoint().y() - box.minPoint().y();
  auto dz = box.maxPoint().z() - box.minPoint().z();

  auto m = std::max(std::max(dx, dy), dz);

  auto x0 = box.minPoint().x();
  auto y0 = box.minPoint().y();
  auto z0 = box.minPoint().z();

  auto x1 = box.maxPoint().x();
  auto y1 = box.maxPoint().y();
  auto z1 = box.maxPoint().z();

  if (m == dx) {
    x1 = x0 + dx / 2.f;
    x0 = x1;
  } else if (m == dy) {
    y1 = y0 + dy / 2.f;
    y0 = y1;
  } else if (m == dz) {
    z1 = z0 + dz / 2.f;
    z0 = z1;
  }

  auto midMin = Point3f(x0, y0, z0);
  auto midMax = Point3f(x1, y1, z1);

  auto left = BoundingBox(box.minPoint(), midMax);
  auto right = BoundingBox(midMin, box.maxPoint());

  return std::make_pair(left, right);
}
