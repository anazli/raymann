#pragma once

#include <cmath>
#include <limits>

#include "light.h"
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"
#include "normal3.h"
#include "orthonormal.h"
#include "point3.h"
#include "ray.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

const float PI = acos(-1.);
const auto InvPI = 1.f / PI;
constexpr auto EPS = std::numeric_limits<float>::epsilon();
constexpr auto EPS1 = 0.000002f;
