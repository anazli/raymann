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
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  Vec3f reflectedColor(std::shared_ptr<Traceable> w, const Ray &r) override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

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
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

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
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

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
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

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
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;
  Mat4f m_object_trans;
  Mat4f m_pattern_trans;

  Vec3f pattern_at(const Point3f &p) const;
};
