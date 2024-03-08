#include "stochastic/random.h"

std::mt19937 Random::m_gen(198485949u);

float Random::randomNumber(float a, float b) {
  std::uniform_real_distribution<float> rnd(a, b);
  return rnd(m_gen);
}

float Random::randomInteger(int a, int b) {
  std::uniform_int_distribution<int> rnd(a, b);
  return rnd(m_gen);
}

Vec3f Random::randomVectorOnUnitSphere() {
  float xi1{0.f}, xi2{0.f}, dsq{2.f};
  while (dsq >= 1.f) {
    xi1 = 1.f - 2.f * randomNumber();
    xi2 = 1.f - 2.f * randomNumber();
    dsq = xi1 * xi1 + xi2 * xi2;
  }

  auto ranh = 2.f * sqrt(1.f - dsq);
  auto dmx = xi1 * ranh;
  auto dmy = xi2 * ranh;
  auto dmz = 1.f - 2.f * dsq;

  return Vec3f(dmx, dmy, dmz);
}

Vec3f Random::randomCosineDirection() {
  auto r1 = randomNumber();
  auto r2 = randomNumber();
  auto phi = 2.f * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);
  auto z = sqrt(1.f - r2);
  return Vec3f(x, y, z);
}
