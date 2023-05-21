#pragma once

#include <list>
#include <memory>
#include <string>

#include "tools/tools.h"

// TODO: implement base SceneElement class and GeometricalElement base class.
// We have the composite which has geometrical elements asigned to each of Scene
// elements which accepts also the rendering algorithm and
// materials/transformations as visitor. The rendering algos could be strategy.

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

class Element {
 public:
  virtual ~Element();
  virtual bool intersect(const Ray &r) = 0;
  virtual Vec3f lighting(const Ray &ray);
  virtual Vec3f reflectedColor(const Ray &r, int rec = 5);
  virtual bool isWorld() const;
  virtual Vec3f normal(const Point3f &p) const;
  virtual std::shared_ptr<Element> closestHit(const Ray &r);
  virtual void checkInside(const Ray &r);
  virtual bool isShadowed(const Point3f &p);
  virtual void setLight(const PointLight &l);
  virtual PointLight getLight() const;
  virtual void setReflection(float ref);
  virtual float getReflection() const;

 protected:
  Element() {}
  Element(const std::string &n);
  std::shared_ptr<Element> m_parent;
  std::string m_name;
  Record rec;
  PointLight m_light;
  float m_reflection = 0.f;  // TODO: move it from material here
};

typedef std::shared_ptr<Element> ElementPtr;

class SceneElement;

class BaseGeomElement {
 public:
  virtual bool intersect(const Ray &r) = 0;
  virtual Vec3f normal(const Point3f &p) const = 0;
  virtual void visitSceneElement(
      std::shared_ptr<SceneElement> sceneElem) const {}
};

class SceneElement {
 public:
  virtual void add(std::shared_ptr<SceneElement> item) = 0;
  virtual void remove(std::shared_ptr<SceneElement> item, bool del = true) = 0;
  virtual Vec3f colorAt(const Ray &ray, int rec = 5) const = 0;
  virtual Record &record() = 0;
  virtual void setParent(std::shared_ptr<SceneElement> t);
  virtual std::shared_ptr<SceneElement> getParent() const = 0;
};

typedef std::shared_ptr<SceneElement> SceneElementPtr;

class SceneElementLeaf : public SceneElement {
 public:
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(std::shared_ptr<SceneElement> item, bool del = true) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) const override;
  Record &record() override;
  void setParent(std::shared_ptr<SceneElement> t) override;
  std::shared_ptr<SceneElement> getParent() const override;

 private:
  std::shared_ptr<BaseGeomElement> m_geom_elem;
};

typedef std::shared_ptr<SceneElement> SceneElementLeafPtr;

class World : public SceneElement {
 public:
  void add(std::shared_ptr<SceneElement> item) override;
  void remove(std::shared_ptr<SceneElement> item, bool del = true) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) const override;
  Record &record() override;
  void setParent(std::shared_ptr<SceneElement> t) override;
  std::shared_ptr<SceneElement> getParent() const override;

 private:
  std::list<SceneElement> m_scene_elem_list;
};
