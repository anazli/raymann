#include "materials/material.h"

#include "composite/scene_element.h"
#include "material.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"
#include "tools/orthonormal.h"

namespace {

float schlick(float cosine, float ref_idx) {
  auto r0 = (1.f - ref_idx) / (1.f + ref_idx);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * pow((1.f - cosine), 5);
}

Vec3D reflect(const Vec3D& v, const Vec3D& n) {
  return v - 2.f * dot(v, n) * n;
}

bool refract(const Vec3D& v, const Vec3D& n, float ni_over_nt,
             Vec3D& refracted) {
  auto uv = getUnitVectorOf(v);
  auto dt = dot(uv, n);
  auto discriminant = 1.f - ni_over_nt * ni_over_nt * (1.f - dt * dt);
  if (discriminant > 0) {
    refracted =
        ni_over_nt * (uv - n * dt) - n * static_cast<float>(sqrt(discriminant));
    return true;
  } else
    return false;
}
}  // namespace

Material::Material(TexturePtr tex, const MaterialProperties& prop)
    : m_tex(std::move(tex)), m_prop(prop) {}

void Material::setTexture(TexturePtr tex) {}

TextureRawPtr Material::getTexture() const { return nullptr; }

void Material::setProperties(const MaterialProperties& prop) {}

MaterialProperties Material::getProperties() const {
  return MaterialProperties();
}

Vec3D Material::emmit(float u, float v, const Vec3D& p) { return Vec3D(); }

bool Material::isEmissive() const { return false; }

float Material::scatteringPDF(const Ray& r, const IntersectionRecord& record,
                              const Ray& scatteredRay) const {
  return 1.f;
}

std::shared_ptr<StochasticPdf> Material::pdf() const { return m_pdf; }

MaterialType Material::getType() const { return m_type; }

StandardMaterial::StandardMaterial(TexturePtr tex,
                                   const MaterialProperties& prop)
    : Material(std::move(tex), prop) {}

void StandardMaterial::setTexture(TexturePtr tex) { m_tex = std::move(tex); }

TextureRawPtr StandardMaterial::getTexture() const { return m_tex.get(); }

void StandardMaterial::setProperties(const MaterialProperties& prop) {
  m_prop = prop;
}

MaterialProperties StandardMaterial::getProperties() const { return m_prop; }

bool StandardMaterial::scatter(const Ray& r_in, const IntersectionRecord& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

MaterialPtr StandardMaterial::create(TexturePtr tex,
                                     const MaterialProperties& prop) {
  return std::make_shared<StandardMaterial>(std::move(tex), prop);
}

Lambertian::Lambertian(TexturePtr tex, const MaterialProperties& prop)
    : Material(std::move(tex), prop) {
  m_pdf = std::make_shared<CosPdf>();
  m_type = MaterialType::LAMBERTIAN;
}

bool Lambertian::scatter(const Ray& r_in, const IntersectionRecord& rec,
                         Vec3D& attenuation, Ray& scattered) const {
  // auto point = rec.point(r_in);
  // OrthoNormalBasis orthnb;
  // orthnb.buildFromW(rec.object->normal(point));
  // auto scatterDir = orthnb.local(Random::randomCosineDirection());
  auto point = rec.point(r_in);
  auto target =
      point + Random::randomVectorOnUnitSphere() + rec.object->normal(point);
  scattered = Ray(point, target - point);
  m_pdf->setFromW(rec.object->normal(point));
  attenuation = m_tex->value(0, 0, Vec3D());
  // m_pdf->setFromW(rec.object->normal(point));
  return true;
}

float Lambertian::scatteringPDF(const Ray& r, const IntersectionRecord& record,
                                const Ray& scatteredRay) const {
  auto cTheta = dot(record.object->normal(record.point(scatteredRay)),
                    getUnitVectorOf(scatteredRay.direction()));
  return cTheta < 0 ? 0 : cTheta / PI;
}

MaterialPtr Lambertian::create(TexturePtr tex, const MaterialProperties& prop) {
  return std::make_shared<Lambertian>(std::move(tex), prop);
}

Isotropic::Isotropic(TexturePtr tex, const MaterialProperties& prop)
    : Material(std::move(tex), prop) {
  m_pdf = std::make_shared<SpherePdf>();
  m_type = MaterialType::ISOTROPIC;
}

bool Isotropic::scatter(const Ray& r_in, const IntersectionRecord& rec,
                        Vec3D& attenuation, Ray& scattered) const {
  scattered =
      Ray(rec.point(r_in), getUnitVectorOf(Random::randomVectorOnUnitSphere()));
  attenuation = m_tex->value(0.f, 0.f, Vec3D());
  return true;
}

float Isotropic::scatteringPDF(const Ray& r, const IntersectionRecord& record,
                               const Ray& scatteredRay) const {
  return 1.f / (4.f * PI);
}

MaterialPtr Isotropic::create(TexturePtr tex, const MaterialProperties& prop) {
  return std::make_shared<Isotropic>(std::move(tex), prop);
}

Metal::Metal(TexturePtr tex, const MaterialProperties& prop)
    : Material(std::move(tex), prop) {
  auto f = prop.getPropertyAs<float>(MaterialProperties::FUZZ).value_or(0.);
  if (f < 1.f)
    m_fuzz = f;
  else
    m_fuzz = 1.f;
  m_type = MaterialType::METAL;
}

bool Metal::scatter(const Ray& r_in, const IntersectionRecord& rec,
                    Vec3D& attenuation, Ray& scattered) const {
  auto point = rec.point(r_in);
  auto normal = rec.object->normal(point);
  Vec3D reflected = reflect(getUnitVectorOf(r_in.direction()), normal);
  scattered =
      Ray(point, reflected + m_fuzz * Random::randomVectorOnUnitSphere());
  attenuation = m_tex->value(0, 0, Vec3D());
  return (dot(scattered.direction(), normal) > 0);
}

MaterialPtr Metal::create(TexturePtr tex, const MaterialProperties& prop) {
  return std::make_shared<Metal>(std::move(tex), prop);
}

Dielectric::Dielectric(TexturePtr tex, const MaterialProperties& prop)
    : Material(std::move(tex), prop) {
  ref_idx = prop.getPropertyAs<float>(MaterialProperties::REFRACTIVE_INDEX)
                .value_or(1.);
  m_type = MaterialType::DIELECTRIC;
}

bool Dielectric::scatter(const Ray& r_in, const IntersectionRecord& rec,
                         Vec3D& attenuation, Ray& scattered) const {
  auto point = rec.point(r_in);
  Vec3D normal = rec.object->normal(point);
  Vec3D outward_normal;
  Vec3D reflected = reflect(r_in.direction(), normal);
  float ni_over_nt;
  attenuation = Vec3D(1.f, 1.f, 1.f);
  Vec3D refracted;
  float reflect_prob;
  float cosine;
  if (dot(r_in.direction(), normal) > 0) {
    outward_normal = -normal;
    ni_over_nt = ref_idx;
    cosine = dot(r_in.direction(), normal) / r_in.direction().length();
    cosine = sqrt(1.f - ref_idx * ref_idx * (1.f - cosine * cosine));
  } else {
    outward_normal = normal;
    ni_over_nt = 1.f / ref_idx;
    cosine = -dot(r_in.direction(), normal) / r_in.direction().length();
  }

  if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
    reflect_prob = schlick(cosine, ref_idx);
  else
    reflect_prob = 1.f;

  if (drand48() < reflect_prob)
    scattered = Ray(point, reflected);
  else
    scattered = Ray(point, refracted);

  return true;
}

MaterialPtr Dielectric::create(TexturePtr tex, const MaterialProperties& prop) {
  return std::make_shared<Dielectric>(std::move(tex), prop);
}

EmissiveMaterial::EmissiveMaterial(TexturePtr tex,
                                   const MaterialProperties& prop)
    : Material(std::move(tex), prop) {
  m_type = MaterialType::DIFFUSE_LIGHT;
}

bool EmissiveMaterial::scatter(const Ray& r_in, const IntersectionRecord& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

Vec3D EmissiveMaterial::emmit(float u, float v, const Vec3D& p) {
  return m_tex->value(u, v, p);
}

bool EmissiveMaterial::isEmissive() const { return true; }

MaterialPtr EmissiveMaterial::create(TexturePtr tex,
                                     const MaterialProperties& prop) {
  return std::make_shared<EmissiveMaterial>(std::move(tex), prop);
}
