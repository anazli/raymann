#pragma once
#include "decorators/traceable_deco.h"

//------------------------------------------------------------------------------
//---------------------------Material-------------------------------------------
//------------------------------------------------------------------------------

class Material : public TraceableDeco {
 public:
  Material(Traceable *tr, Vec3f &color);
  Material(Traceable *tr, Vec3f &color, float reflection);
  Material(Traceable *tr, const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
           float reflection = 0.0f, float am = 0.1f, float diff = 0.9f,
           float spec = 0.9f, float shi = 200.0f);
  virtual ~Material();

  bool intersect(const Ray &r) override;
  Vec3f lighting(const Ray &ray) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) override;
  Vec3f reflectedColor(const Ray &r, int rec = 5) override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(const Point3f &p) override;
  void setReflection(float ref) override;
  float getReflection() const override;

 protected:
  Vec3f m_color = Vec3f(1.0f, 1.0f, 1.0f);
  float m_reflective = 0.0f;
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.0f;
};

//------------------------------------------------------------------------------
//---------------------------Stripe Pattern Decorator---------------------------
//------------------------------------------------------------------------------

class StripePattern : public Material {
 public:
  StripePattern(Traceable *tr, const Vec3f &a, const Vec3f &b,
                const Mat4f &otrans, const Mat4f &ptrans);
  Vec3f lighting(const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
};

//------------------------------------------------------------------------------
//---------------------------Gradient Pattern Decorator-------------------------
//------------------------------------------------------------------------------

class GradientPattern : public Material {
 public:
  GradientPattern(Traceable *tr, const Vec3f &a, const Vec3f &b,
                  const Mat4f &otrans, const Mat4f &ptrans);
  Vec3f lighting(const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
};

//------------------------------------------------------------------------------
//---------------------------Ring Pattern Decorator-----------------------------
//------------------------------------------------------------------------------

class RingPattern : public Material {
 public:
  RingPattern(Traceable *tr, const Vec3f &a, const Vec3f &b,
              const Mat4f &otrans, const Mat4f &ptrans);
  Vec3f lighting(const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
};

//------------------------------------------------------------------------------
//---------------------------Ring Pattern Decorator-----------------------------
//------------------------------------------------------------------------------

class CheckerPattern : public Material {
 public:
  CheckerPattern(Traceable *tr, const Vec3f &a, const Vec3f &b,
                 const Mat4f &otrans, const Mat4f &ptrans);
  CheckerPattern(Traceable *tr, const Vec3f &a, const Vec3f &b,
                 const Mat4f &otrans, const Mat4f &ptrans, const Vec3f &c,
                 float reflection, float am, float diff, float spec, float shi);
  Vec3f lighting(const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
};

class PerlinNoise : public Material {
 public:
  PerlinNoise(Traceable *tr, const Vec3f &a, const Vec3f &b,
              const Mat4f &otrans, const Mat4f &ptrans);
  PerlinNoise(Traceable *tr, const Vec3f &a, const Vec3f &b,
              const Mat4f &otrans, const Mat4f &ptrans, const Vec3f &c,
              float reflection, float am, float diff, float spec, float shi);
  Vec3f lighting(const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
  static float fade(float t);
  static float lerp(float t, float a, float b);
  static float grad(int hash, float x, float y, float z);
  static float noise(float x, float y, float z);
  static std::vector<int> p;
  std::vector<int> permutation = {
      151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
      225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
      6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
      35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
      171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
      231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
      245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
      76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
      164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
      202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
      58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
      154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
      19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
      228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
      145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
      84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
      222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
      180};
};
