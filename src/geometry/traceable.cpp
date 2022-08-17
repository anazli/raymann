#include "traceable.h"

#include <algorithm>

using std::list;
using std::sort;
using std::vector;

//------------------------------------------------------------------------------
//---------------------------Abstract Traceable---------------------------------
//------------------------------------------------------------------------------

Traceable::~Traceable() {}

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

Record Traceable::record() const { return rec; }

void Traceable::setParent(Traceable* t) { m_parent = t; }

Traceable* Traceable::getParent() const { return m_parent; }

//------------------------------------------------------------------------------
//---------------------------Decorator------------------------------------------
//------------------------------------------------------------------------------

TraceableDeco::TraceableDeco(Traceable* tr) : m_traceable(tr) {}

TraceableDeco::~TraceableDeco() { delete m_traceable; }

bool TraceableDeco::intersect(const Ray& r) {
  return m_traceable->intersect(r);
}

Vec3f TraceableDeco::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  return m_traceable->lighting(w, ray);
}

std::string TraceableDeco::name() const { return m_traceable->name(); }

Record TraceableDeco::record() const { return m_traceable->record(); }

Vec3f TraceableDeco::normal(const Point3f& p) const {
  return m_traceable->normal(p);
}

void TraceableDeco::checkInside(const Ray& r) {
  return m_traceable->checkInside(r);
}

bool TraceableDeco::isShadowed(std::shared_ptr<Traceable> w, const Point3f& p) {
  return m_traceable->isShadowed(w, p);
}

//------------------------------------------------------------------------------
//---------------------------Transformer----------------------------------------
//------------------------------------------------------------------------------

Transformer::Transformer(Traceable* tr) : TraceableDeco(tr) {
  m_transformer.identity();
}

Transformer::Transformer(Traceable* tr, const Mat4f& m)
    : TraceableDeco(tr), m_transformer(m) {}

Transformer::~Transformer() {}

bool Transformer::intersect(const Ray& r) {
  Ray r_transformed = r.transform(m_transformer.inverse());
  return TraceableDeco::intersect(r_transformed);
}

Vec3f Transformer::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  return TraceableDeco::lighting(w, ray);
}

std::string Transformer::name() const { return TraceableDeco::name(); }

Record Transformer::record() const { return TraceableDeco::record(); }

Vec3f Transformer::normal(const Point3f& p) const {
  Vec4f v4 = p;
  Point3f object_point = m_transformer.inverse() * v4;
  Vec3f object_normal = TraceableDeco::normal(object_point);
  Vec3f world_normal =
      m_transformer.inverse().transpose() * Vec4f(object_normal);
  return world_normal.normalize();
}

void Transformer::checkInside(const Ray& r) {
  return TraceableDeco::checkInside(r);
}

bool Transformer::isShadowed(std::shared_ptr<Traceable> w, const Point3f& p) {
  return TraceableDeco::isShadowed(w, p);
}

//------------------------------------------------------------------------------
//---------------------------Material-------------------------------------------
//------------------------------------------------------------------------------

Material::Material(Traceable* tr, Vec3f& color)
    : TraceableDeco(tr), m_color(color) {}

Material::Material(Traceable* tr, const Vec3f& c, float am, float diff,
                   float spec, float shi)
    : TraceableDeco(tr),
      m_color(c),
      m_ambient(am),
      m_diffuse(diff),
      m_specular(spec),
      m_shininess(shi) {}

Material::~Material() {}

bool Material::intersect(const Ray& r) { return TraceableDeco::intersect(r); }

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

std::string Material::name() const { return TraceableDeco::name(); }

Record Material::record() const { return TraceableDeco::record(); }

Vec3f Material::normal(const Point3f& p) const {
  return TraceableDeco::normal(p);
}

void Material::checkInside(const Ray& r) {
  return TraceableDeco::checkInside(r);
}

bool Material::isShadowed(std::shared_ptr<Traceable> w, const Point3f& p) {
  return TraceableDeco::isShadowed(w, p);
}

//------------------------------------------------------------------------------
//---------------------------Stripe Pattern-------------------------------------
//------------------------------------------------------------------------------

StripePattern::StripePattern(Traceable* tr, const Vec3f& a, const Vec3f& b,
                             const Mat4f& ptrans, const Mat4f& otrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f StripePattern::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(w, ray);
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
                                 const Mat4f& ptrans, const Mat4f& otrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f GradientPattern::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(w, ray);
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
                         const Mat4f& ptrans, const Mat4f& otrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f RingPattern::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(w, ray);
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
                               const Mat4f& ptrans, const Mat4f& otrans)
    : Material(tr),
      m_color_a(a),
      m_color_b(b),
      m_pattern_trans(ptrans),
      m_object_trans(otrans) {}

Vec3f CheckerPattern::lighting(std::shared_ptr<Traceable> w, const Ray& ray) {
  Point3f p =
      record().point(ray) + (record().inside ? normal(record().point(ray))
                                             : normal(record().point(ray))) *
                                0.02f;
  m_color = pattern_at(p);
  return Material::lighting(w, ray);
}

Vec3f CheckerPattern::pattern_at(const Point3f& p) const {
  Point3f object_p = m_object_trans.inverse() * Vec4f(p);
  Point3f res_p = m_pattern_trans.inverse() * Vec4f(object_p);

  if (fmod(floor(res_p.x()) + floor(res_p.y()) + floor(res_p.z()), 2.) == 0.)
    return m_color_a;
  return m_color_b;
}
