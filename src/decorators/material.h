#pragma once
#include "composite/properties.h"
#include "decorators/traceable_deco.h"
#include "textures/texture.h"

//------------------------------------------------------------------------------
//---------------------------Material-------------------------------------------
//------------------------------------------------------------------------------

class Material : public TraceableDeco {
 public:
  Material(Traceable *tr, TexturePtr tex,
           const Properties &prop = Properties());
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
  TexturePtr m_tex;
  Properties m_prop;  // TODO: replace all members bellow with mprop variables
  float m_reflective = 0.f;
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.f;
};
