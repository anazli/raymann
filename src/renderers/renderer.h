#pragma once

#include <memory>

#include "composite/scene_element.h"

class BaseRenderer {
 public:
  virtual ~BaseRenderer() = default;
  virtual void visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                 const Ray &ray) = 0;
  virtual void visitSceneElementComposite(const SceneElementRawPtr elementComp,
                                          const Ray &ray) = 0;
  virtual Vec3f computeColor(const SceneElementRawPtr world, const Ray &ray,
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

using BaseRendererPtr = std::unique_ptr<BaseRenderer>;
