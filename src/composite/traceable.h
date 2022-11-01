#pragma once

#include <memory>
#include <string>

#include "textures/perlin.h"
#include "tools/tools.h"

//------------------------------------------------------------------------------
//---------------------------Helper Struct--------------------------------------
//------------------------------------------------------------------------------

struct Record {
  int count = 0;
  float t1 = -1.0f;
  float t2 = -1.0f;
  float t_min() const {
    if (t1 <= 0.0f && t2 > 0.0f)
      return t2;
    else if (t2 <= 0.0f && t1 > 0.0f)
      return t1;
    else if (t1 > 0.0f && t2 > 0.0f)
      return std::min(t1, t2);
    return -1.0f;  // TODO: to be fixed for negative intersections
  }
  Point3f point(const Ray &r) const { return r.position(t_min()); }
  Vec3f eye(const Ray &r) const { return -r.direction(); }
  bool inside = false;
  Point3f over_point_from_refl_surf;
};

class Traceable {
 public:
  virtual ~Traceable();
  virtual bool intersect(const Ray &r) = 0;
  virtual Vec3f lighting(const Ray &ray);
  virtual Vec3f colorAt(const Ray &ray, int rec = 5);
  virtual Vec3f reflectedColor(const Ray &r, int rec = 5);
  virtual void add(std::shared_ptr<Traceable> item) {}
  virtual void remove(std::shared_ptr<Traceable> item, bool del = true) {}
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual std::string name() const;
  virtual std::shared_ptr<Traceable> closestHit(const Ray &r);
  virtual void checkInside(const Ray &r);
  virtual bool isShadowed(const Point3f &p);
  virtual void setLight(const PointLight &l);
  virtual PointLight getLight() const;
  virtual Record &record();
  virtual void setParent(std::shared_ptr<Traceable> t);
  virtual std::shared_ptr<Traceable> getParent() const;
  virtual void setReflection(float ref);
  virtual float getReflection() const;
  virtual void addPerlinNoise();

 protected:
  Traceable() {}
  Traceable(const std::string &n);
  std::shared_ptr<Traceable> m_parent;
  std::string m_name;
  Record rec;
  PointLight m_light;
  float m_reflection = 0.f;  // TODO: move it from material here
  std::shared_ptr<PerlinNoise> m_perlin = nullptr;
};

typedef std::shared_ptr<Traceable> TraceablePtr;
