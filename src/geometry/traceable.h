#pragma once

#include <string>
#include <vector>

#include "tools/ray.h"

struct record {
  int count;
  float t1;
  float t2;
};

class Traceable {
 public:
  virtual ~Traceable() {}
  virtual bool intersect(const Ray &r) = 0;
  virtual void add(Traceable *) {}
  virtual void remove(Traceable *) {}
  std::string name() const { return m_name; }

  record rec;

 protected:
  Traceable() {}
  Traceable(const std::string &);

 private:
  std::string m_name;
};

class World : public Traceable {
 public:
  virtual ~World();
  bool intersect(const Ray &r) override;
  void add(Traceable *) override;
  void remove(Traceable *) override;

 protected:
  World(const std::string &);

 private:
  std::vector<Traceable *> m_traceable_list;  // TODO: or maybe list
};
