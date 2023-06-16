#pragma once

#include <memory>

#include "camera/camera.h"
#include "composite/scene_element.h"
#include "tools/tools.h"

class IntersectionRecord {
 public:
  int count = 0;
  float t1 = -MAXFLOAT;
  float t2 = -MAXFLOAT;
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
  Point3f under_point_from_refrac_surf;
  Vec3f normal;
  float n1, n2;
  std::shared_ptr<SceneElement> object = nullptr;
};

class BaseRenderer {
 public:
  virtual void visitSceneElement(SceneElement &elementLeaf, const Ray &ray) = 0;
  virtual void visitSceneElementComposite(const SceneElementPtr &elementComp,
                                          const Ray &ray) = 0;
  virtual Vec3f computeColor(const SceneElementPtr &world, const Ray &ray,
                             int rec = 5) = 0;

  Vec3f getColor() const { return m_out_color; }
  void setPixelInfo(const int &x, const int y) {
    m_x = static_cast<float>(x);
    m_y = static_cast<float>(y);
  }

 protected:
  IntersectionRecord m_closestHit;
  Vec3f m_out_color;
  float m_tmin = MAXFLOAT;
  float m_x;
  float m_y;
};

typedef std::shared_ptr<BaseRenderer> BaseRendererPtr;
