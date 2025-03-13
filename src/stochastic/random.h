#pragma once

#include <random>

#include "math_utils/math_utils.h"

class Random {
 public:
  static float randomNumber(float a = 0.f, float b = 1.f);
  static float randomInteger(int a = 0, int b = 1);
  static Vec3D randomVectorOnUnitSphere();
  static Vec3D randomCosineDirection();

 private:
  static std::mt19937 m_gen;
};
