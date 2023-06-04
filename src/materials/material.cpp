#include "materials/material.h"

#include <random>

#include "composite/scene_element.h"

using std::uniform_real_distribution;
std::mt19937 gen(13759327u);

Vec3f randomVectorOnUnitSphere() {
  uniform_real_distribution<float> rand(0.f, 1.f);
  float xi1, xi2, dsq = 2.f;
  while (dsq >= 1.f) {
    xi1 = 1.f - 2. * rand(gen);
    xi2 = 1.f - 2. * rand(gen);
    dsq = xi1 * xi1 + xi2 * xi2;
  }

  float ranh = 2.f * sqrt(1.f - dsq);
  float dmx = xi1 * ranh;
  float dmy = xi2 * ranh;
  float dmz = 1.f - 2.f * dsq;

  Vec3f ret(dmx, dmy, dmz);
  return ret;
}

float schlick(float cosine, float ref_idx) {
  float r0 = (1.f - ref_idx) / (1.f + ref_idx);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * pow((1.f - cosine), 5);
}

Vec3f reflect(const Vec3f& v, const Vec3f& n) {
  return v - 2.f * dot(v, n) * n;
}

bool refract(const Vec3f& v, const Vec3f& n, float ni_over_nt,
             Vec3f& refracted) {
  Vec3f uv = getUnitVectorOf(v);
  float dt = dot(uv, n);
  float discriminant = 1.f - ni_over_nt * ni_over_nt * (1.f - dt * dt);
  if (discriminant > 0) {
    refracted =
        ni_over_nt * (uv - n * dt) - n * static_cast<float>(sqrt(discriminant));
    return true;
  } else
    return false;
}

Material::Material(TexturePtr tex, const MaterialProperties& prop)
    : BaseMaterial(tex, prop) {}

void Material::setTexture(TexturePtr tex) { m_tex = tex; }

TexturePtr Material::getTexture() const { return m_tex; }

void Material::setProperties(const MaterialProperties& prop) { m_prop = prop; }

MaterialProperties Material::getProperties() const { return m_prop; }

bool Material::scatter(const Ray& r_in, const IntersectionRecord& rec,
                       Vec3f& attenuation, Ray& scattered) const {
  return false;
}

Lambertian::Lambertian(TexturePtr tex, const MaterialProperties& prop)
    : BaseMaterial(tex, prop) {}

bool Lambertian::scatter(const Ray& r_in, const IntersectionRecord& rec,
                         Vec3f& attenuation, Ray& scattered) const {
  Vec3f target =
      Vec3f(rec.point(r_in)) + randomVectorOnUnitSphere() + rec.normal;
  scattered = Ray(rec.point(r_in), target - rec.point(r_in));
  attenuation = m_tex->value(0, 0, Vec3f());
  return true;
}

Metal::Metal(float f, TexturePtr tex, const MaterialProperties& prop)
    : m_fuzz(f), BaseMaterial(tex, prop) {
  if (f < 1.f)
    m_fuzz = f;
  else
    m_fuzz = 1.f;
}

bool Metal::scatter(const Ray& r_in, const IntersectionRecord& rec,
                    Vec3f& attenuation, Ray& scattered) const {
  Vec3 reflected = reflect(getUnitVectorOf(r_in.direction()), rec.normal);
  scattered =
      Ray(rec.point(r_in), reflected + m_fuzz * randomVectorOnUnitSphere());
  attenuation = m_tex->value(0, 0, Vec3f());
  return (dot(scattered.direction(), rec.normal) > 0);
}

Dielectric::Dielectric(float ri, TexturePtr tex, const MaterialProperties& prop)
    : ref_idx(ri), BaseMaterial(tex, prop) {}

bool Dielectric::scatter(const Ray& r_in, const IntersectionRecord& rec,
                         Vec3f& attenuation, Ray& scattered) const {
  Vec3f outward_normal;
  Vec3f reflected = reflect(r_in.direction(), rec.normal);
  float ni_over_nt;
  attenuation = Vec3f(1.f, 1.f, 1.f);
  Vec3f refracted;
  float reflect_prob;
  float cosine;
  if (dot(r_in.direction(), rec.normal) > 0) {
    outward_normal = -rec.normal;
    ni_over_nt = ref_idx;
    cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
    cosine = sqrt(1.f - ref_idx * ref_idx * (1.f - cosine * cosine));
  } else {
    outward_normal = rec.normal;
    ni_over_nt = 1.f / ref_idx;
    cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
  }

  if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
    reflect_prob = schlick(cosine, ref_idx);
  else
    reflect_prob = 1.f;

  if (drand48() < reflect_prob)
    scattered = Ray(rec.point(r_in), reflected);
  else
    scattered = Ray(rec.point(r_in), refracted);

  return true;
}
