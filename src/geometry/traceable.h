#pragma once

#include <list>
#include <string>

#include "tools/ray.h"

class Traceable {
 public:
  virtual ~Traceable() {}
  virtual bool intersect(const Ray &r) = 0;
  virtual void add(Traceable *item) {}
  virtual void remove(Traceable *item) {}
  virtual bool isWorld() const { return false; }
  std::string name() const { return m_name; }
  void setParent(Traceable *t) { m_parent = t; }
  Traceable *getParent() const { return m_parent; }

  struct record {
    int count = 0;
    float t1 = 0.0f;
    float t2 = 0.0f;
  };
  record rec;

 protected:
  Traceable() {}
  Traceable(const std::string &n);
  Traceable *m_parent;

 private:
  std::string m_name;
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
  void remove(Traceable *item) override {
    m_traceable_list.remove(item);
    item->setParent(nullptr);
  }
  bool isWorld() const override { return true; }

 private:
  std::list<Traceable *> m_traceable_list;  // TODO: or maybe list
};
