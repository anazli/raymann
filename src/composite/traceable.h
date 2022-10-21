#pragma once

#include <memory>
#include <string>

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
};

class Traceable {
 public:
  virtual ~Traceable();
  virtual bool intersect(const Ray &r) = 0;
  virtual Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray);
  virtual void add(std::shared_ptr<Traceable> item) {}
  virtual void remove(std::shared_ptr<Traceable> item, bool del = true) {}
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual std::string name() const;
  virtual Traceable &closestHit(const Ray &r);
  virtual void checkInside(const Ray &r);
  virtual bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p);
  virtual void setLight(const PointLight &l) {}
  virtual PointLight getLight() const;
  virtual Record &record();

  void setParent(Traceable *t);
  Traceable *getParent() const;

 protected:
  Traceable() {}
  Traceable(const std::string &n);
  Traceable *m_parent;
  std::string m_name;
  Record rec;
};

typedef std::shared_ptr<Traceable> TraceablePtr;