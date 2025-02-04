#pragma once

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Sphere : public SceneElement {
 public:
  ~Sphere() override = default;
  Sphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f), const float &r = 1.0f)
      : m_center(c), m_radius(r) {
    m_elementType = SceneElementType::SPHERE;
    m_bBox.minPoint() = Point3f(-1.f, -1.f, -1.f) + c;
    m_bBox.maxPoint() = Point3f(1.f, 1.f, 1.f) + c;
  }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto co = r.origin() - m_center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0f * dot(r.direction(), co);
    auto c = dot(co, co) - m_radius * m_radius;
    auto discr = b * b - 4.0f * a * c;
    if (discr >= 0.0f) {
      record.t1 = (-b - sqrt(discr)) / (2. * a);
      record.t2 = (-b + sqrt(discr)) / (2. * a);
      record.count = 2;
      record.saved_point = record.point(r);
      return true;
    }
    return false;
  }

  Vec3f normal(const Point3f &p) const override {
    return getUnitVectorOf(p - m_center);
  }

  float pdf(const Point3f &origin, const Vec3f &direction) override {
    IntersectionRecord rec;
    if (!intersect(Ray(origin, direction), rec)) return 0;
    float radius = 30.f;
    Point3f center(277.f, 540.f, -455.f);

    auto cosThetaMax = sqrt(
        1.f - radius * radius /
                  ((center - origin).length() * (center - origin).length()));
    auto solidAngle = 2 * PI * (1.f - cosThetaMax);

    return 1.f / solidAngle;
  }
  Vec3f random(const Point3f &origin) override {
    auto direction = Vec3f(277.f, 540.f, -455.f) - origin;
    auto distSquared = direction.length() * direction.length();
    OrthoNormalBasis onb;
    onb.buildFromW(direction);
    return onb.local(randomToSphere(30.f, distSquared));
  }

  void setCenter(const Point3f &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3f center() const { return m_center; }
  float radius() const { return m_radius; }

 private:
  Vec3f randomToSphere(float radius, float distSquared) {
    auto r1 = Random::randomNumber();
    auto r2 = Random::randomNumber();
    auto z = 1.f + r2 * (sqrt(1.f - radius * radius / distSquared) - 1.f);

    auto phi = 2.f * PI * r1;
    auto x = cos(phi) * sqrt(1.f - z * z);
    auto y = sin(phi) * sqrt(1.f - z * z);

    return Vec3f(x, y, z);
  }

  Point3f m_center;
  float m_radius;
};
