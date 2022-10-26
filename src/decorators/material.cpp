#include "decorators/material.h"

//------------------------------------------------------------------------------
//---------------------------Material-------------------------------------------
//------------------------------------------------------------------------------

Material::Material(Traceable* tr, Vec3f& color)
    : TraceableDeco(tr), m_color(color) {}

Material::Material(Traceable* tr, Vec3f& color, float reflection)
    : TraceableDeco(tr), m_color(color), m_reflective(reflection) {}

Material::Material(Traceable* tr, const Vec3f& c, float reflection, float am,
                   float diff, float spec, float shi)
    : TraceableDeco(tr),
      m_color(c),
      m_reflective(reflection),
      m_ambient(am),
      m_diffuse(diff),
      m_specular(spec),
      m_shininess(shi) {}

Material::~Material() {}

bool Material::intersect(const Ray& r) { return TraceableDeco::intersect(r); }

Vec3f Material::lighting(const Ray& ray) {
  Vec3f effective_color = m_color * getLight().intensity();
  Point3f over_point =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  Vec3f normal_vec = record().inside ? -normal(over_point) : normal(over_point);
  Vec3f lightv = (getLight().position() - over_point).normalize();

  Vec3f ret_ambient = effective_color * m_ambient;
  Vec3f ret_diffuse;
  Vec3f ret_specular;

  if (isShadowed(over_point)) return ret_ambient;

  float light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse = effective_color * m_diffuse * light_normal;
    Vec3f reflectv = reflect(-lightv, normal_vec);
    float reflect_dot_eye = dot(reflectv, record().eye(ray));
    if (reflect_dot_eye > 0.0f) {
      float factor = pow(reflect_dot_eye, m_shininess);
      ret_specular = getLight().intensity() * m_specular * factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

Vec3f Material::colorAt(const Ray& ray, int rec) {
  Vec3f surf_col = lighting(ray);
  Vec3f refl_col = reflectedColor(ray, rec);
  return surf_col + refl_col;
}

Vec3f Material::reflectedColor(const Ray& r, int rec) {
  if (rec <= 0) {
    return Vec3f(0.f, 0.f, 0.f);
  }
  if (m_reflective <= 0.f) {
    return Vec3f(0.f, 0.f, 0.f);
  }
  Vec3f reflectv =
      reflect(r.direction(), (record().inside ? normal(record().point(r))
                                              : normal(record().point(r))));
  Point3f over_point =
      record().point(r) + (record().inside ? normal(record().point(r))
                                           : normal(record().point(r))) *
                              0.02f;
  record().over_point_from_refl_surf = over_point;
  Ray reflect_ray(over_point, reflectv);

  Vec3f color = getParent()->colorAt(reflect_ray, rec - 1);
  return color * m_reflective;
}

Record& Material::record() { return TraceableDeco::record(); }

Vec3f Material::normal(const Point3f& p) const {
  return TraceableDeco::normal(p);
}

void Material::checkInside(const Ray& r) {
  return TraceableDeco::checkInside(r);
}

bool Material::isShadowed(const Point3f& p) {
  return TraceableDeco::isShadowed(p);
}

//------------------------------------------------------------------------------
//---------------------------Stripe Pattern-------------------------------------
//------------------------------------------------------------------------------

StripePattern::StripePattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                             const Mat4f& otrans, const Mat4f& ptrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f StripePattern::lighting(const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(ray);
}

Vec3f StripePattern::pattern_at(const Point3f& p) const {
  Point3f object_p = m_object_trans.inverse() * Vec4f(p);
  Point3f res_p = m_pattern_trans.inverse() * Vec4f(object_p);
  if (fmod(floor(res_p.x()), 2.0f) == 0.0f) return m_color_a;
  return m_color_b;
}

//------------------------------------------------------------------------------
//---------------------------Gradient Pattern-----------------------------------
//------------------------------------------------------------------------------

GradientPattern::GradientPattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                                 const Mat4f& otrans, const Mat4f& ptrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f GradientPattern::lighting(const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(ray);
}

Vec3f GradientPattern::pattern_at(const Point3f& p) const {
  Point3f object_p = m_object_trans.inverse() * Vec4f(p);
  Point3f res_p = m_pattern_trans.inverse() * Vec4f(object_p);

  Vec3f distance = m_color_b - m_color_a;
  float fraction = res_p.x() - floor(res_p.x());
  return m_color_a + distance * fraction;
}

//------------------------------------------------------------------------------
//---------------------------Ring Pattern---------------------------------------
//------------------------------------------------------------------------------

RingPattern::RingPattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                         const Mat4f& otrans, const Mat4f& ptrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f RingPattern::lighting(const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(ray);
}

Vec3f RingPattern::pattern_at(const Point3f& p) const {
  Point3f object_p = m_object_trans.inverse() * Vec4f(p);
  Point3f res_p = m_pattern_trans.inverse() * Vec4f(object_p);

  if (fmod(floor(sqrt(res_p.x() * res_p.x() + res_p.z() * res_p.z())), 2.) ==
      0.)
    return m_color_a;
  return m_color_b;
}

//------------------------------------------------------------------------------
//---------------------------Checker Pattern------------------------------------
//------------------------------------------------------------------------------

CheckerPattern::CheckerPattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                               const Mat4f& otrans, const Mat4f& ptrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

CheckerPattern::CheckerPattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                               const Mat4f& otrans, const Mat4f& ptrans,
                               const Vec3f& c, float reflection, float am,
                               float diff, float spec, float shi)
    : Material(tr, c, reflection, am, diff, spec, shi),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f CheckerPattern::lighting(const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(ray);
}

Vec3f CheckerPattern::pattern_at(const Point3f& p) const {
  Point3f object_p = m_object_trans.inverse() * Vec4f(p);
  Point3f res_p = m_pattern_trans.inverse() * Vec4f(object_p);

  if (fmod(floor(res_p.x()) + floor(res_p.y()) + floor(res_p.z()), 2.) == 0.)
    return m_color_a;
  return m_color_b;
}
