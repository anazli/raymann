#include "materials/metal.h"

#include "composite/scene_element.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"

Metal::Metal(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex), prop) {
  auto f = prop.getPropertyAs<float>(DataContainer::FUZZ).value_or(0.);
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

MaterialPtr Metal::create(TexturePtr tex, const DataContainer& prop) {
  return std::make_shared<Metal>(std::move(tex), prop);
}