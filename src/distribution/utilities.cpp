#include "distribution/utilities.h"

#include "utilities.h"

bool isOnSameHemisphere(const Vec3D& v1, const Vec3D& v2) {
  return v1.z() * v2.z() > 0.f;
}

bool isOnSameHemisphere(const Vec3D& v, const Normal3D& n) {
  return v.z() * n.z() > 0.f;
}
inline float absoluteCosTheta(const Vec3D& v) { return std::abs(v.z()); }