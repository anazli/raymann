#pragma once

#include <constants.h>
#include <math.h>
#include <vec3.h>

#include <random>

class Random {
 public:
  static float randomFloat(float a = 0.f, float b = 1.f);
  static float randomInteger(int a = 0, int b = 1);
  static Vec3f randomVectorOnUnitSphere();
  static Vec3f randomCosineDirection();

 private:
  static std::mt19937 m_gen;
};
