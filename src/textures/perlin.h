#pragma once

#include <vec3.h>

#include <experimental/random>

//------------------------------------------------------------------------------
// Perlin implementation from Peter Shirley's  book, ray tracing the next week
//------------------------------------------------------------------------------

float trilinearInterpolation(float c[2][2][2], float u, float v, float w);
float perlinInterpolation(Vec3f c[2][2][2], float u, float v, float w);
static Vec3f *perlinGenerate();
void permute(int *p, int n);
static int *perlinGeneratePerm();

class Perlin {
 public:
  float noise(const Vec3f &p) const {
    float u = p.x() - floor(p.x());
    float v = p.y() - floor(p.y());
    float w = p.z() - floor(p.z());
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    Vec3f c[2][2][2];
    for (int di = 0; di < 2; ++di)
      for (int dj = 0; dj < 2; ++dj)
        for (int dk = 0; dk < 2; ++dk)
          c[di][dj][dk] =
              ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                     perm_z[(k + dk) & 255]];
    return perlinInterpolation(c, u, v, w);
  }
  float turb(const Vec3f &p, int depth = 7) const {
    float accum = 0.f;
    Vec3f temp_p = p;
    float weight = 1.f;
    for (int i = 0; i < depth; ++i) {
      accum += weight * noise(temp_p);
      weight *= 0.5f;
      temp_p = temp_p * 2.f;
    }
    return fabs(accum);
  }
  static Vec3f *ranvec;
  static int *perm_x;
  static int *perm_y;
  static int *perm_z;
};