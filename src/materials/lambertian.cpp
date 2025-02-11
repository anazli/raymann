#include "materials/lambertian.h"

#include "composite/scene_element.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"

Lambertian::Lambertian(TexturePtr tex) : Material(std::move(tex)) {
  m_pdf = std::make_shared<CosPdf>();
  m_type = AppParameters::LAMBERTIAN;
}

bool Lambertian::scatter(const Ray& r_in, const Intersection& rec,
                         Vec3D& attenuation, Ray& scattered) const {
  // auto point = rec.point(r_in);
  // OrthoNormalBasis orthnb;
  // orthnb.buildFromW(rec.object->normal(point));
  // auto scatterDir = orthnb.local(Random::randomCosineDirection());
  auto point = rec.point(r_in);
  auto target =
      point + Random::randomVectorOnUnitSphere() + rec.primitive->normal(point);
  scattered = Ray(point, target - point);
  m_pdf->setFromW(rec.primitive->normal(point));
  attenuation = m_tex->value(0, 0, Vec3D());
  // m_pdf->setFromW(rec.object->normal(point));
  return true;
}

float Lambertian::scatteringPDF(const Ray& r, const Intersection& record,
                                const Ray& scatteredRay) const {
  auto cTheta = dot(record.primitive->normal(record.point(scatteredRay)),
                    getUnitVectorOf(scatteredRay.direction()));
  return cTheta < 0 ? 0 : cTheta / PI;
}

MaterialPtr Lambertian::create(TexturePtr tex) {
  return std::make_shared<Lambertian>(std::move(tex));
}