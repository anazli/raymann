#pragma once

#include <random>

#include "tools/tools.h"

class Random {
 public:
  static float randomNumber(float a = 0.f, float b = 1.f);
  static Vec3f randomVectorOnUnitSphere();
  static Vec3f randomCosineDirection();

 private:
  static std::mt19937 m_gen;
};
