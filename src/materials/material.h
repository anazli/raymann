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
  MaterialType getType() const;

 protected:
  Material(TexturePtr tex, const DataContainer& prop);
  TexturePtr m_tex;
  DataContainer m_prop;
  std::shared_ptr<StochasticPdf> m_pdf;
  MaterialType m_type;
};

using MaterialPtr = std::shared_ptr<Material>;

class StandardMaterial : public Material {
 public:
  StandardMaterial(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~StandardMaterial() override = default;
  void setTexture(TexturePtr tex) override;
  TextureRawPtr getTexture() const override;
  void setProperties(const DataContainer& prop) override;
  DataContainer getProperties() const override;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;

  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());
};

class Lambertian : public Material {
 public:
  Lambertian(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Lambertian() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                      const Ray& scatteredRay) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());
};

class Isotropic : public Material {
 public:
  Isotropic(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Isotropic() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                      const Ray& scatteredRay) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());
};

class Metal : public Material {
 public:
  Metal(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Metal() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());

 private:
  float m_fuzz;
};

class Dielectric : public Material {
 public:
  Dielectric(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Dielectric() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());

 private:
  float ref_idx;
};

class EmissiveMaterial : public Material {
 public:
  EmissiveMaterial(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~EmissiveMaterial() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D()) override;
  bool isEmissive() const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());
};
