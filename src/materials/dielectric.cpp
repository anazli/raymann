#include "materials/dielectric.h"

#include "composite/scene_element.h"

Dielectric::Dielectric(TexturePtr tex, float refractive_index)
    : m_refractive_index(refractive_index), Material(std::move(tex)) {
  m_type = AppParameters::DIELECTRIC;
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
    ni_over_nt = m_refractive_index;
    cosine = dot(r_in.direction(), normal) / r_in.direction().length();
    cosine = sqrt(1.f - m_refractive_index * m_refractive_index *
                            (1.f - cosine * cosine));
  } else {
    outward_normal = normal;
    ni_over_nt = 1.f / m_refractive_index;
    cosine = -dot(r_in.direction(), normal) / r_in.direction().length();
  }

  if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
    reflect_prob = schlick(cosine, m_refractive_index);
  else
    reflect_prob = 1.f;

  if (drand48() < reflect_prob)
    scattered = Ray(point, reflected);
  else
    scattered = Ray(point, refracted);

  return true;
}

MaterialPtr Dielectric::create(TexturePtr tex, float refractive_index) {
  return std::make_shared<Dielectric>(std::move(tex), refractive_index);
}