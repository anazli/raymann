#pragma once

#include "application/data_container.h"
#include "textures/texture.h"

class Intersection;
class StochasticSampler;
class StochasticPdf;

class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  // Given input with info of geometry at intersection point, compute refrective
  // properties at the point.
  virtual void evaluateScattering(const Intersection& record) = 0;

 protected:
  BaseMaterial(TexturePtr texture);
  TexturePtr m_texture;
};

class Material {
 public:
  virtual ~Material() = default;
  virtual void setTexture(TexturePtr tex);
  virtual TextureRawPtr getTexture() const;
  virtual void setProperties(const DataContainer& prop);
  virtual DataContainer getProperties() const;
  virtual bool scatter(const Ray& r_in, const Intersection& rec,
                       Vec3D& attenuation, Ray& scattered) const = 0;
  virtual Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D());
  virtual bool isEmissive() const;
  virtual float scatteringPDF(const Ray& r, const Intersection& record,
                              const Ray& scatteredRay) const;
  std::shared_ptr<StochasticPdf> pdf() const;
  AppParameters getType() const;

 protected:
  Material(TexturePtr tex);
  float schlick(float cosine, float ref_idx) const;
  Vec3D reflect(const Vec3D& v, const Vec3D& n) const;
  bool refract(const Vec3D& v, const Vec3D& n, float ni_over_nt,
               Vec3D& refracted) const;

  TexturePtr m_tex;
  std::shared_ptr<StochasticPdf> m_pdf;
  AppParameters m_type;
};

using MaterialPtr = std::shared_ptr<Material>;
using MaterialRawPtr = Material*;
