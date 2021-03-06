#pragma once

#include "geometry/traceable.h"
#include "tools/point3.h"
#include "tools/ray.h"
#include "tools/vec3.h"

class Sphere : public Traceable {
 public:
  Sphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f), const float &r = 1.0f)
      : m_center(c), m_radius(r) {
    setParent(nullptr);
    std::stringstream ss;
    ss << center();
    m_name = "Sphere:" + ss.str();  // TODO:Translation can change the name
  }

  virtual ~Sphere() {}
  bool intersect(const Ray &r) override;
  bool isWorld() const override { return false; }
  std::string name() const override { return m_name; }
  Record record() const override { return rec; }
  Vec3f normal(const Point3f &p) const override {
    return (p - m_center).normalize();
  }
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;
  void setCenter(const Point3f &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3f center() const { return m_center; }
  float radius() const { return m_radius; }

 private:
  Point3f m_center;
  float m_radius;
};

inline bool Sphere::intersect(const Ray &r) {
  Vec3f co = r.origin() - m_center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(r.direction(), co);
  float c = dot(co, co) - m_radius * m_radius;
  float discr = b * b - 4.0f * a * c;
  if (discr >= 0.0f) {
    float t1 = (-b - sqrt(discr)) / (2. * a);
    float t2 = (-b + sqrt(discr)) / (2. * a);
    rec.t1 = t1;
    rec.t2 = t2;
    rec.count = 2;
    return true;
  }
  return false;
}

inline bool Sphere::isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) {
  Vec3f v = p - w->getLight().position();
  float distance = v.length();
  Ray r(w->getLight().position(), v.normalize());
  if (w->intersect(r)) {
    Traceable &t = w->closestHit(r);
    if (t.record().t_min() >= 0.0f && t.record().t_min() < distance)
      return true;
  }
  return false;
}

class SphereBuilder {
 public:
  virtual void addSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                         const float &r = 1.0f) {}
  virtual void transformSphere(const Mat4f &m) {}
  virtual void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
                        float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
                        float shi = 200.0f) {}
  virtual void addStripePattern(const Vec3f &colorA = Vec3f(1.0f, 1.0f, 1.0f),
                                const Vec3f &colorB = Vec3f()) {}
  virtual std::shared_ptr<Traceable> getSphere() { return nullptr; }

 protected:
  SphereBuilder() {}
};

class StandardSphere : public SphereBuilder {
 public:
  StandardSphere() : m_currentSphere(nullptr) {}
  void addSphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f),
                 const float &r = 1.0f) override {
    m_currentSphere = new Sphere(c, r);
  }
  void transformSphere(const Mat4f &m) override {
    m_currentSphere = new Transformer(m_currentSphere, m);
  }
  void addColor(const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f), float am = 0.1f,
                float diff = 0.9f, float spec = 0.9f,
                float shi = 200.0f) override {
    m_currentSphere = new Material(m_currentSphere, c, am, diff, spec, shi);
  }
  void addStripePattern(const Vec3f &colorA = Vec3f(1.0f, 1.0f, 1.0f),
                        const Vec3f &colorB = Vec3f()) override {
    m_currentSphere = new StripePattern(m_currentSphere, colorA, colorB);
  }
  std::shared_ptr<Traceable> getSphere() override {
    std::shared_ptr<Traceable> ret(m_currentSphere);
    return ret;
  }

 private:
  Traceable *m_currentSphere;
};
