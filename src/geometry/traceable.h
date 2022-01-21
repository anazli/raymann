#pragma once

#include <list>
#include <string>
#include <vector>

#include "tools/tools.h"

class Traceable {
 public:
  virtual ~Traceable() {}
  virtual bool intersect(const Ray &r) = 0;
  virtual Vec3f lighting(const PointLight &light, const Ray &ray) {
    return Vec3f();
  }
  virtual void add(Traceable *item) {}
  virtual void remove(Traceable *item, bool del = true) {}
  virtual bool isWorld() const { return false; }
  virtual Vec3f normal(const Point3f &p) const { return Vec3f(); }
  virtual std::string name() const { return m_name; }
  virtual Traceable &closestHit() { return *this; }
  void setParent(Traceable *t) { m_parent = t; }
  Traceable *getParent() const { return m_parent; }

  struct Record {
    int count = 0;
    float t1 = 0.0f;
    float t2 = 0.0f;
    float t_min() const { return std::min(t1, t2); }
    Point3f point(const Ray &r) const { return r.position(t_min()); }
    Vec3f eye(const Ray &r) const { return -r.direction(); }
    bool inside = false;
  };
  virtual Record record() const { return rec; }

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
  Vec3f lighting(const PointLight &light, const Ray &ray) override {
    return m_traceable->lighting(light, ray);
  }
  std::string name() const override { return m_traceable->name(); }
  Record record() const override { return m_traceable->record(); }
  Vec3f normal(const Point3f &p) const override {
    return m_traceable->normal(p);
  }

 protected:
  Traceable *m_traceable;
};

class Transformer : public TraceableDeco {
 public:
  Transformer(Traceable *tr) : TraceableDeco(tr) { m_transformer.identity(); }
  Transformer(Traceable *tr, const Mat4f &m)
      : TraceableDeco(tr), m_transformer(m) {}
  bool intersect(const Ray &r) override {
    Ray r_transformed = r.transform(m_transformer.inverse());
    return TraceableDeco::intersect(r_transformed);
  }
  std::string name() const override { return TraceableDeco::name(); }
  Record record() const override { return TraceableDeco::record(); }
  Vec3f normal(const Point3f &p) const override;

 private:
  Mat4f m_transformer;
};

class Material : public TraceableDeco {
 public:
  Material(Traceable *tr, const Vec3f &c = Vec3f(1.0f, 1.0f, 1.0f),
           float am = 0.1f, float diff = 0.9f, float spec = 0.9f,
           float shi = 10.0f);

  bool intersect(const Ray &r) override { return TraceableDeco::intersect(r); }
  Vec3f lighting(const PointLight &light, const Ray &ray) override;
  std::string name() const override { return TraceableDeco::name(); }
  Record record() const override { return TraceableDeco::record(); }
  Vec3f normal(const Point3f &p) const override {
    return TraceableDeco::normal(p);
  }

 protected:
  Vec3f m_color;
  float m_ambient;
  float m_diffuse;
  float m_specular;
  float m_shininess;
};

class World : public Traceable {
 public:
  World() {}
  World(const std::string &n);
  virtual ~World();
  bool intersect(const Ray &r) override;
  void add(Traceable *item) override;
  void remove(Traceable *item, bool del = true) override;
  bool isWorld() const override { return true; }
  Traceable &closestHit() override;
  std::vector<float> intersectionsSorted() const;
  std::list<Traceable *>::iterator createIterator() {
    return m_traceable_list.begin();
  }

  std::list<Traceable *>::iterator isDone() { return m_traceable_list.end(); }

 private:
  std::list<Traceable *> m_traceable_list;
};
