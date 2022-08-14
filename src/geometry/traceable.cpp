#include "traceable.h"

#include <algorithm>

using std::list;
using std::sort;
using std::vector;

//------------------------------------------------------------------------------
//---------------------------Abstract Traceable---------------------------------
//------------------------------------------------------------------------------

Vec3f Traceable::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  return Vec3f();
}

bool Traceable::isWorld() const { return false; }

Vec3f Traceable::normal(const Point3f& p) const { return Vec3f(); }

std::string Traceable::name() const { return m_name; }

Traceable& Traceable::closestHit(const Ray& r) { return *this; }

void Traceable::checkInside(const Ray& r) {
  if (dot(record().eye(r), normal(record().point(r))) < 0.0f) rec.inside = true;
}

bool Traceable::isShadowed(std::shared_ptr<Traceable> w, const Point3f& p) {
  return false;
}

PointLight Traceable::getLight() const { return PointLight(); }

void Traceable::setParent(Traceable* t) { m_parent = t; }

Traceable* Traceable::getParent() const { return m_parent; }

Record Traceable::record() const { return rec; }

//------------------------------------------------------------------------------
//---------------------------Transformer----------------------------------------
//------------------------------------------------------------------------------

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformer.inverse() * v4;
  Vec3f object_normal = TraceableDeco::normal(object_point);
  Vec3f world_normal =
      m_transformer.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}

//------------------------------------------------------------------------------
//---------------------------Material-------------------------------------------
//------------------------------------------------------------------------------

Material::Material(Traceable* tr, const Vec3f& c, float am, float diff,
                   float spec, float shi)
    : TraceableDeco(tr),
      m_color(c),
      m_ambient(am),
      m_diffuse(diff),
      m_specular(spec),
      m_shininess(shi) {}

Vec3f Material::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Vec3f effective_color = m_color * w->getLight().intensity();
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  Vec3f normal_vec = record().inside ? -normal(p) : normal(p);
  Vec3f lightv = (w->getLight().position() - p).normalize();

  Vec3f ret_ambient = effective_color * m_ambient;
  Vec3f ret_diffuse;
  Vec3f ret_specular;

  if (isShadowed(w, p)) return ret_ambient;

  float light_normal = dot(lightv, normal_vec);
  if (light_normal > 0.0f) {
    ret_diffuse = effective_color * m_diffuse * light_normal;
    Vec3f reflectv = reflect(-lightv, normal_vec);
    float reflect_dot_eye = dot(reflectv, record().eye(ray));
    if (reflect_dot_eye > 0.0f) {
      float factor = pow(reflect_dot_eye, m_shininess);
      ret_specular = w->getLight().intensity() * m_specular * factor;
    }
  }

  return ret_ambient + ret_diffuse + ret_specular;
}

//------------------------------------------------------------------------------
//---------------------------Stripe Pattern-------------------------------------
//------------------------------------------------------------------------------

Vec3f StripePattern::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;

  m_color = stripe_at(p);
  return Material::lighting(w, ray);
}
