#pragma once

#include <list>
#include <string>

#include "tools/ray.h"

class Traceable {
 public:
  virtual ~Traceable() {}
  virtual bool intersect(const Ray &r) = 0;
  virtual void add(Traceable *item) {}
  virtual void remove(Traceable *item, bool del = true) {}
  virtual bool isWorld() const { return false; }
  virtual Vec3f normal(const Point3f &p) const { return Vec3f(); }
  virtual std::string name() const { return m_name; }
  void setParent(Traceable *t) { m_parent = t; }
  Traceable *getParent() const { return m_parent; }

  struct Record {
    int count = 0;
    float t1 = 0.0f;
    float t2 = 0.0f;
    float t_min() { return std::min(t1, t2); }
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
  Vec3f normal(const Point3f &p) const override {
    Vec4f v4(p);
    v4 = m_transformer.inverse() * v4;
    Point3f object_point = v4;
    Vec3f object_normal = object_point - Point3f(0.0f, 0.0f, 0.0f);
    v4 = object_normal;
    v4 = m_transformer.inverse().transpose() * v4;
    Vec3f world_normal = v4;
    return world_normal.normalize();
    // return TraceableDeco::normal(p);
  }

 private:
  Mat4f m_transformer;
};

class World : public Traceable {
 public:
  World() {}
  World(const std::string &n);
  virtual ~World() {
    std::list<Traceable *>::iterator it;
    for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
      delete (*it);
  }
  bool intersect(const Ray &r) override {
    bool has_intersection = false;
    std::list<Traceable *>::iterator it;
    for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
      if ((*it)->intersect(r)) has_intersection = true;
    return has_intersection;
  }
  void add(Traceable *item) override {
    m_traceable_list.push_back(item);
    item->setParent(this);
  }
  void remove(Traceable *item, bool del = true) override {
    m_traceable_list.remove(item);
    item->setParent(nullptr);
    if (del) delete item;
  }
  bool isWorld() const override { return true; }
  Traceable &closestHit() const {  // TODO: maybe a better solution
    std::list<Traceable *>::const_iterator it;
    Traceable *temp = nullptr;
    float min_hit = MAXFLOAT;
    for (it = m_traceable_list.begin(); it != m_traceable_list.end(); ++it)
      if ((*it)->record().t_min() > 0.0f && (*it)->record().t_min() < min_hit) {
        temp = (*it);
        min_hit = temp->record().t_min();
      }
    assert(temp != nullptr);  // All intersections are negative
    return *temp;
  }

 private:
  std::list<Traceable *> m_traceable_list;
};
