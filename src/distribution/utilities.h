// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include "tools/tools.h"

inline bool isOnSameHemisphere(const Vec3D& v1, const Vec3D& v2);
inline bool isOnSameHemisphere(const Vec3D& v, const Normal3D& n);
inline float absoluteCosTheta(const Vec3D& v);