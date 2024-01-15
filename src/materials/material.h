#pragma once

#include "materials/material_properties.h"
#include "textures/texture.h"

class IntersectionRecord;

class BaseMaterial {
 public:
  virtual void setTexture(TexturePtr tex) {}
  virtual TextureRawPtr getTexture() const { return nullptr; }
  virtual void setProperties(const MaterialProperties& prop) {}
  virtual MaterialProperties getProperties() const;
  virtual bool scatter(const Ray& r_in, const IntersectionRecord& rec,
                       Vec3f& attenuation, Ray& scattered) const = 0;

 protected:
  BaseMaterial(TexturePtr tex,
               const MaterialProperties& prop = MaterialProperties())
      : m_tex(std::move(tex)), m_prop(prop) {}
  TexturePtr m_tex;
  MaterialProperties m_prop;
};

using BaseMaterialPtr = std::shared_ptr<BaseMaterial>;

class Material : public BaseMaterial {
 public:
  Material(TexturePtr tex,
           const MaterialProperties& prop = MaterialProperties());
  void setTexture(TexturePtr tex) override;
  TextureRawPtr getTexture() const override;
  void setProperties(const MaterialProperties& prop) override;
  MaterialProperties getProperties() const override;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3f& attenuation, Ray& scattered) const override;
};

class Lambertian : public BaseMaterial {
 public:
  Lambertian(TexturePtr tex,
             const MaterialProperties& prop = MaterialProperties());
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3f& attenuation, Ray& scattered) const override;
};

class Metal : public BaseMaterial {
 public:
  Metal(float f, TexturePtr tex, const MaterialProperties& prop);
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3f& attenuation, Ray& scattered) const override;

 private:
  float m_fuzz;
};

class Dielectric : public BaseMaterial {
 public:
  Dielectric(float ri, TexturePtr tex, const MaterialProperties& prop);

  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3f& attenuation, Ray& scattered) const override;

 private:
  float ref_idx;
};
