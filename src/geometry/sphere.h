#pragma once

#include "geometry/primitive.h"
#include "stochastic/random.h"

class Sphere : public Primitive {
 public:
  ~Sphere() override = default;
  Sphere(const Transformation &tr = Transformation(),
         const Point3D &c = Point3D(0.0f, 0.0f, 0.0f), const float &r = 1.0f)
      : m_center(c), m_radius(r), Primitive(tr) {
    m_object_box.minPoint() = Point3D(-1.f, -1.f, -1.f) + Vec3D(c);
    m_object_box.maxPoint() = Point3D(1.f, 1.f, 1.f) + Vec3D(c);
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    Point3D origin = transf_ray.origin();
    Vec3D direction = transf_ray.direction();
    auto co = origin - m_center;
    auto a = dot(direction, direction);
    auto b = 2.0f * dot(direction, co);
    auto c = dot(co, co) - m_radius * m_radius;
    auto discr = b * b - 4.0f * a * c;
    if (discr >= 0.0f) {
      auto t1 = static_cast<float>(-b - sqrt(discr)) / (2.f * a);
      auto t2 = static_cast<float>(-b + sqrt(discr)) / (2.f * a);
      record.min_hit = Intersection::getMinHitParam(transf_ray, {t1, t2});
      record.hit_point = record.getHitPoint(transf_ray);
      record.normal = normal(record.hit_point);
      return true;
    }
    return false;
  }

  Normal3D normal(const Point3D &p) const override {
    auto object_point = m_transformation.worldToObjectSpace(p);
    auto object_normal = Normal3D(getUnitVectorOf(object_point - m_center));
    return getUnitVectorOf(m_transformation.objectToWorldSpace(object_normal));
  }

  void setCenter(const Point3D &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3D center() const { return m_center; }
  float radius() const { return m_radius; }

  static PrimitivePtr create(const Transformation &tr = Transformation(),
                             const Point3D &c = Point3D(0.0f, 0.0f, 0.0f),
                             const float &r = 1.0f) {
    return std::make_shared<Sphere>(tr, c, r);
  }

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
