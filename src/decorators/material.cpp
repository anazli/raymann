#include "decorators/material.h"

Material::Material(Element* tr, TexturePtr tex, const Properties& prop)
    : ElementDeco(tr), m_tex(tex), m_prop(prop) {
  setReflection(m_prop.getPropertyAsFloat(Props::REFLECTION));
  m_ambient = m_prop.getPropertyAsFloat(Props::AMBIENT);
  m_diffuse = m_prop.getPropertyAsFloat(Props::DIFFUSE);
  m_specular = m_prop.getPropertyAsFloat(Props::SPECULAR);
  m_shininess = m_prop.getPropertyAsFloat(Props::SHININESS);
}

bool Material::intersect(const Ray& r) { return ElementDeco::intersect(r); }

Vec3f Material::lighting(const Ray& ray) {
  Point3f over_point =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  Vec3f normal_vec = record().inside ? -normal(over_point) : normal(over_point);
  Vec3f lightv = (getLight().position() - over_point).normalize();

  Vec3f effective_color =
      m_tex->value(0, 0, Vec3f(over_point)) * getLight().intensity();

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
  ElementPtr w = getParent();
  if (w) {
    ElementPtr t = w->closestHit(ray);
    if (t) {
      t->setLight(w->getLight());
      Vec3f surf_col = t->lighting(ray);
      Vec3f refl_col = t->reflectedColor(ray, rec);
      return surf_col + refl_col;
    }
  }
  return Vec3f(0.f, 0.f, 0.f);
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
                              EPS1;
  record().over_point_from_refl_surf = over_point;
  Ray reflect_ray(over_point, reflectv);

  Vec3f color = colorAt(reflect_ray, rec - 1);
  return color * m_reflective;
}

Record& Material::record() { return ElementDeco::record(); }

Vec3f Material::normal(const Point3f& p) const {
  return ElementDeco::normal(p);
}

void Material::checkInside(const Ray& r) {
  return ElementDeco::checkInside(r);
}

bool Material::isShadowed(const Point3f& p) {
  return ElementDeco::isShadowed(p);
}

void Material::setReflection(float ref) { ElementDeco::setReflection(ref); }

float Material::getReflection() const { return ElementDeco::getReflection(); }
