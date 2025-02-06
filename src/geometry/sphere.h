#pragma once

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Sphere : public SceneElement {
 public:
  ~Sphere() override = default;
  Sphere(const Point3D &c = Point3D(0.0f, 0.0f, 0.0f), const float &r = 1.0f)
      : m_center(c), m_radius(r) {
    m_elementType = SceneElementType::SPHERE;
    m_bBox.minPoint() = Point3D(-1.f, -1.f, -1.f) + c;
    m_bBox.maxPoint() = Point3D(1.f, 1.f, 1.f) + c;
  }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto transformed_ray = r.transform(m_transformation.getInverseMatrix());
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    auto co = origin - m_center;
    auto a = dot(direction, direction);
    auto b = 2.0f * dot(direction, co);
    auto c = dot(co, co) - m_radius * m_radius;
    auto discr = b * b - 4.0f * a * c;
    if (discr >= 0.0f) {
      record.t1 = (-b - sqrt(discr)) / (2. * a);
      record.t2 = (-b + sqrt(discr)) / (2. * a);
      record.count = 2;
      record.saved_point = record.point(transformed_ray);
      return true;
    }
    return false;
  }

  Vec3D normal(const Point3D &p) const override {
    Vec4D v4 = p;
    Point3D object_point = m_transformation.getInverseMatrix() * v4;
    auto object_normal = getUnitVectorOf(object_point - m_center);
    auto world_normal =
        m_transformation.getInverseMatrix() * Vec4D(object_normal);
    return getUnitVectorOf(world_normal);
  }

  float pdf(const Point3D &origin, const Vec3D &direction) override {
    IntersectionRecord rec;
    if (!intersect(Ray(origin, direction), rec)) return 0;
    float radius = 30.f;
    Point3D center(277.f, 540.f, -455.f);

    auto cosThetaMax = sqrt(
        1.f - radius * radius /
                  ((center - origin).length() * (center - origin).length()));
    auto solidAngle = 2 * PI * (1.f - cosThetaMax);

    return 1.f / solidAngle;
  }
  Vec3D random(const Point3D &origin) override {
    auto direction = Vec3D(277.f, 540.f, -455.f) - origin;
    auto distSquared = direction.length() * direction.length();
    OrthoNormalBasis onb;
    onb.buildFromW(direction);
    return onb.local(randomToSphere(30.f, distSquared));
  }

  void setCenter(const Point3D &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3D center() const { return m_center; }
  float radius() const { return m_radius; }

 private:
  Vec3D randomToSphere(float radius, float distSquared) {
    auto r1 = Random::randomNumber();
    auto r2 = Random::randomNumber();
    auto z = 1.f + r2 * (sqrt(1.f - radius * radius / distSquared) - 1.f);

    auto phi = 2.f * PI * r1;
    auto x = cos(phi) * sqrt(1.f - z * z);
    auto y = sin(phi) * sqrt(1.f - z * z);

    return Vec3D(x, y, z);
  }

  Point3D m_center;
  float m_radius;
};
