#pragma once

#include "materials/material_properties.h"
#include "textures/texture.h"

class IntersectionRecord;
class StochasticSampler;
class StochasticPdf;

enum class MaterialType {
  STANDARD,
  LAMBERTIAN,
  ISOTROPIC,
  DIFFUSE_LIGHT,
  METAL,
  DIELECTRIC
};

class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  virtual void setTexture(TexturePtr tex);
  virtual TextureRawPtr getTexture() const;
  virtual void setProperties(const MaterialProperties& prop);
  virtual MaterialProperties getProperties() const;
  virtual bool scatter(const Ray& r_in, const IntersectionRecord& rec,
                       Vec3D& attenuation, Ray& scattered) const = 0;
  virtual Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D());
  virtual bool isEmissive() const;
  virtual float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                              const Ray& scatteredRay) const;
  std::shared_ptr<StochasticPdf> pdf() const;
  MaterialType getType() const;

 protected:
  BaseMaterial(TexturePtr tex,
               const MaterialProperties& prop = MaterialProperties())
      : m_tex(std::move(tex)), m_prop(prop) {}
  TexturePtr m_tex;
  MaterialProperties m_prop;
  std::shared_ptr<StochasticPdf> m_pdf;
  MaterialType m_type;
};

using BaseMaterialPtr = std::shared_ptr<BaseMaterial>;

class StandardMaterial : public BaseMaterial {
 public:
  StandardMaterial(TexturePtr tex,
                   const MaterialProperties& prop = MaterialProperties());
  ~StandardMaterial() override = default;
  void setTexture(TexturePtr tex) override;
  TextureRawPtr getTexture() const override;
  void setProperties(const MaterialProperties& prop) override;
  MaterialProperties getProperties() const override;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
};

class Lambertian : public BaseMaterial {
 public:
  Lambertian(TexturePtr tex,
             const MaterialProperties& prop = MaterialProperties());
  ~Lambertian() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                      const Ray& scatteredRay) const override;
};

class Isotropic : public BaseMaterial {
 public:
  Isotropic(TexturePtr tex,
            const MaterialProperties& prop = MaterialProperties());
  ~Isotropic() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                      const Ray& scatteredRay) const override;
};

class Metal : public BaseMaterial {
 public:
  Metal(float f, TexturePtr tex,
        const MaterialProperties& prop = MaterialProperties());
  ~Metal() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;

 private:
  float m_fuzz;
};

class Dielectric : public BaseMaterial {
 public:
  Dielectric(float ri, TexturePtr tex,
             const MaterialProperties& prop = MaterialProperties());
  ~Dielectric() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;

 private:
  float ref_idx;
};

class EmissiveMaterial : public BaseMaterial {
 public:
  EmissiveMaterial(TexturePtr tex,
                   const MaterialProperties& prop = MaterialProperties());
  ~EmissiveMaterial() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D()) override;
  bool isEmissive() const override;
};
