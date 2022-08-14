#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

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

//------------------------------------------------------------------------------
//---------------------------Abstract Traceable---------------------------------
//------------------------------------------------------------------------------

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
  virtual Record record() const;

  void setParent(Traceable *t);
  Traceable *getParent() const;

 protected:
  Traceable() {}
  Traceable(const std::string &n);
  Traceable *m_parent;
  std::string m_name;
  Record rec;
};

//------------------------------------------------------------------------------
//---------------------------Traceable Decorator--------------------------------
//------------------------------------------------------------------------------

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr);
  virtual ~TraceableDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override;
  Record record() const override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

 protected:
  Traceable *m_traceable;
};

//------------------------------------------------------------------------------
//---------------------------Transformation Decorator---------------------------
//------------------------------------------------------------------------------

class Transformer : public TraceableDeco {
 public:
  Transformer(Traceable *tr);
  Transformer(Traceable *tr, const Mat4f &m);
  virtual ~Transformer();
  bool intersect(const Ray &r) override;
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override;
  Record record() const override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

 private:
  Mat4f m_transformer;
};

//------------------------------------------------------------------------------
//---------------------------Material Decorator---------------------------------
//------------------------------------------------------------------------------

class Material : public TraceableDeco {
 public:
  Material(Traceable *tr, Vec3f &color);
  Material(Traceable *tr, const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
           float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
           float shi = 200.0f);
  virtual ~Material();

  bool intersect(const Ray &r) override;
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override;
  Record record() const override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override;

 protected:
  Vec3f m_color = Vec3f(1.0f, 1.0f, 1.0f);
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.0f;
};

//------------------------------------------------------------------------------
//---------------------------Stripe Pattern Decorator---------------------------
//------------------------------------------------------------------------------

class StripePattern : public Material {
 public:
  StripePattern(Traceable *tr, const Vec3f &a = Vec3f(1.0f, 1.0f, 1.0f),
                const Vec3f &b = Vec3f());
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;

  Vec3f stripe_at(const Point3f &p);
};

//------------------------------------------------------------------------------
//---------------------------Gradient Pattern Decorator-------------------------
//------------------------------------------------------------------------------

class GradientPattern : public Material {
 public:
  GradientPattern(Traceable *tr, const Vec3f &a = Vec3f(1.0f, 1.0f, 1.0f),
                  const Vec3f &b = Vec3f());
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;

  Vec3f gradient_at(const Point3f &p);
};

//------------------------------------------------------------------------------
//---------------------------Ring Pattern Decorator-----------------------------
//------------------------------------------------------------------------------

class RingPattern : public Material {
 public:
  RingPattern(Traceable *tr, const Vec3f &a = Vec3f(1.0f, 1.0f, 1.0f),
              const Vec3f &b = Vec3f());
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;

  Vec3f ring_at(const Point3f &p);
};
