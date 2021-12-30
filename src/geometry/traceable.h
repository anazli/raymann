#pragma once

#include <string>
#include <vector>

class Traceable {
 public:
  virtual ~Traceable();
  virtual void hit();
  virtual void add(Traceable *);
  virtual void remove(Traceable *);
  std::string name() const { return m_name; }

 protected:
  Traceable(const std::string &);

 private:
  std::string m_name;
};

class World : public Traceable {
 public:
  virtual ~World();
  virtual void hit();
  virtual void add(Traceable *);
  virtual void remove(Traceable *);

 protected:
  World(const std::string &);

 private:
  std::vector<Traceable *> m_traceable_list;  // TODO: or maybe list
};
