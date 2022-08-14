#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "tools/tools.h"

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

struct Record;

class Traceable {
 public:
  virtual ~Traceable() {}
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

class TraceableDeco : public Traceable {
 public:
  TraceableDeco(Traceable *tr) : m_traceable(tr) {}
  virtual ~TraceableDeco() { delete m_traceable; }
  bool intersect(const Ray &r) override { return m_traceable->intersect(r); }
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override {
    return m_traceable->lighting(w, ray);
  }
  std::string name() const override { return m_traceable->name(); }
  Record record() const override { return m_traceable->record(); }
  Vec3f normal(const Point3f &p) const override {
    return m_traceable->normal(p);
  }
  void checkInside(const Ray &r) override {
    return m_traceable->checkInside(r);
  }
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override {
    return m_traceable->isShadowed(w, p);
  }

 protected:
  Traceable *m_traceable;
};

class Transformer : public TraceableDeco {
 public:
  Transformer(Traceable *tr) : TraceableDeco(tr) { m_transformer.identity(); }
  Transformer(Traceable *tr, const Mat4f &m)
      : TraceableDeco(tr), m_transformer(m) {}
  virtual ~Transformer() {}
  bool intersect(const Ray &r) override {
    Ray r_transformed = r.transform(m_transformer.inverse());
    return TraceableDeco::intersect(r_transformed);
  }
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override {
    return TraceableDeco::lighting(w, ray);
  }
  std::string name() const override { return TraceableDeco::name(); }
  Record record() const override { return TraceableDeco::record(); }
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override {
    return TraceableDeco::checkInside(r);
  }
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override {
    return TraceableDeco::isShadowed(w, p);
  }

 private:
  Mat4f m_transformer;
};

class Material : public TraceableDeco {
 public:
  Material(Traceable *tr, Vec3f &color) : TraceableDeco(tr), m_color(color) {}
  Material(Traceable *tr, const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
           float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
           float shi = 200.0f);
  virtual ~Material() {}

  bool intersect(const Ray &r) override { return TraceableDeco::intersect(r); }
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override { return TraceableDeco::name(); }
  Record record() const override { return TraceableDeco::record(); }
  Vec3f normal(const Point3f &p) const override {
    return TraceableDeco::normal(p);
  }
  void checkInside(const Ray &r) override {
    return TraceableDeco::checkInside(r);
  }
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override {
    return TraceableDeco::isShadowed(w, p);
  }

 protected:
  Vec3f m_color = Vec3f(1.0f, 1.0f, 1.0f);
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.0f;
};

class StripePattern : public Material {
 public:
  StripePattern(Traceable *tr, const Vec3f &a = Vec3f(1.0f, 1.0f, 1.0f),
                const Vec3f &b = Vec3f())
      : Material(tr), m_color_a(a), m_color_b(b) {}

  bool intersect(const Ray &r) override { return Material::intersect(r); }
  Vec3f lighting(std::shared_ptr<Traceable> w, const Ray &ray) override;
  std::string name() const override { return Material::name(); }
  Record record() const override { return Material::record(); }
  Vec3f normal(const Point3f &p) const override { return Material::normal(p); }
  void checkInside(const Ray &r) override { return Material::checkInside(r); }
  bool isShadowed(std::shared_ptr<Traceable> w, const Point3f &p) override {
    return Material::isShadowed(w, p);
  }

 private:
  Vec3f m_color_a;
  Vec3f m_color_b;

  Vec3f stripe_at(const Point3f &p) {
    if (fmod(floor(p.x()), 2.0f) == 0.0f) return m_color_a;
    return m_color_b;
  }
};
