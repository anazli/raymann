#pragma once
#include "composite/element.h"

class ElementDeco : public Element {
 public:
  ElementDeco(Element *tr);
  virtual ~ElementDeco();
  bool intersect(const Ray &r) override;
  Vec3f lighting(const Ray &ray) override;
  Vec3f colorAt(const Ray &ray, int rec = 5) override;
  Vec3f reflectedColor(const Ray &r, int rec = 5) override;
  std::string name() const override;
  Record &record() override;
  Vec3f normal(const Point3f &p) const override;
  void checkInside(const Ray &r) override;
  bool isShadowed(const Point3f &p) override;
  void setParent(ElementPtr t) override;
  ElementPtr getParent() const override;
  void setLight(const PointLight &l) override;
  PointLight getLight() const override;
  void setReflection(float ref) override;
  float getReflection() const override;

 protected:
  Element *m_element;
};
