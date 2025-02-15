#pragma once

#include "tools/tools.h"

inline bool isOnSameHemisphere(const Vec3D& v1, const Vec3D& v2);
inline bool isOnSameHemisphere(const Vec3D& v, const Normal3D& n);
inline float absoluteCosTheta(const Vec3D& v);