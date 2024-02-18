#pragma once

#include <memory>

#include "camera/camera.h"
#include "composite/scene_element.h"

class BaseRenderer {
 public:
  BaseRenderer() = default;
  BaseRenderer(const BaseCameraPtr &cam) : m_camera(cam) {}
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
  void setBackgroundColor(const Vec3f &color) { m_background_color = color; }
  Vec3f backgroundColor() const { return m_background_color; }
  void attachCamera(const BaseCameraPtr &cam) { m_camera = cam; }

 protected:
  IntersectionRecord m_closestHit;
  Vec3f m_out_color;
  Vec3f m_background_color;
  float m_tmin = MAXFLOAT;
  float m_x;
  float m_y;
  BaseCameraPtr m_camera;
};

using BaseRendererPtr = std::unique_ptr<BaseRenderer>;
