#pragma once

#include <math.h>

#include <random>

class Random {
 public:
  static float randomNumber(float a = 0.f, float b = 1.f);
  static float randomInteger(int a = 0, int b = 1);
  static Vec3f randomVectorOnUnitSphere();
  static Vec3f randomCosineDirection();

 private:
  static std::mt19937 m_gen;
};
