#include "materials/metal.h"

#include "composite/scene_element.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"

Metal::Metal(TexturePtr tex, float fuzz) : Material(std::move(tex)) {
  if (fuzz < 1.f)
    m_fuzz = fuzz;
  else
    m_fuzz = 1.f;
  m_type = AppParameters::METAL;
}

bool Metal::scatter(const Ray& r_in, const Intersection& rec,
                    Vec3f& attenuation, Ray& scattered) const {
  auto point = rec.getHitPoint(r_in);
  auto normal = rec.closest_scene_element->getPrimitive()->normal(point);
  Vec3f reflected = reflect(normalized(r_in.direction()), Vec3f(normal));
  scattered =
      Ray(point, reflected + m_fuzz * Random::randomVectorOnUnitSphere());
  attenuation = m_tex->value(0, 0, Vec3f());
  return (dot(scattered.direction(), normal) > 0);
}

MaterialPtr Metal::create(TexturePtr tex, float fuzz) {
  return std::make_shared<Metal>(std::move(tex), fuzz);
}