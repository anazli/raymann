#pragma once

#include "application/data_container.h"
#include "textures/texture.h"

class IntersectionRecord;
class StochasticSampler;
class StochasticPdf;

class Material {
 public:
  virtual ~Material() = default;
  virtual void setTexture(TexturePtr tex);
  virtual TextureRawPtr getTexture() const;
  virtual void setProperties(const DataContainer& prop);
  virtual DataContainer getProperties() const;
  virtual bool scatter(const Ray& r_in, const IntersectionRecord& rec,
                       Vec3D& attenuation, Ray& scattered) const = 0;
  virtual Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D());
  virtual bool isEmissive() const;
  virtual float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                              const Ray& scatteredRay) const;
  std::shared_ptr<StochasticPdf> pdf() const;
  AppParameters getType() const;

 protected:
  Material(TexturePtr tex, const DataContainer& prop);
  float schlick(float cosine, float ref_idx) const;
  Vec3D reflect(const Vec3D& v, const Vec3D& n) const;
  bool refract(const Vec3D& v, const Vec3D& n, float ni_over_nt,
               Vec3D& refracted) const;

  TexturePtr m_tex;
  DataContainer m_prop;
  std::shared_ptr<StochasticPdf> m_pdf;
  AppParameters m_type;
};

using MaterialPtr = std::shared_ptr<Material>;
