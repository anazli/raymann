#include "textures/perlin.h"

Vec3f *Perlin::ranvec = perlinGenerate();
int *Perlin::perm_x = perlinGeneratePerm();
int *Perlin::perm_y = perlinGeneratePerm();
int *Perlin::perm_z = perlinGeneratePerm();

float trilinearInterpolation(float c[2][2][2], float u, float v, float w) {
  float accum = 0;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      for (int k = 0; k < 2; ++k)
        accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                 (k * w + (1 - k) * (1 - w)) * c[i][j][k];

  return accum;
}

float perlinInterpolation(Vec3f c[2][2][2], float u, float v, float w) {
  float uu = u * u * (3 - 2 * u);
  float vv = v * v * (3 - 2 * v);
  float ww = w * w * (3 - 2 * w);
  float accum = 0;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      for (int k = 0; k < 2; ++k) {
        Vec3f weight_v(u - i, v - j, w - k);
        accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
                 (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
      }
  return accum;
}

Vec3f *perlinGenerate() {
  Vec3f *p = new Vec3f[256];
  for (int i = 0; i < 256; ++i)
    p[i] = normalized(
        Vec3f(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));

  return p;
}

void permute(int *p, int n) {
  for (int i = n - 1; i > 0; --i) {
    int target = int(drand48() * (i + 1));
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
  return;
}

int *perlinGeneratePerm() {
  int *p = new int[256];
  for (int i = 0; i < 256; ++i) p[i] = i;

  permute(p, 256);
  return p;
}