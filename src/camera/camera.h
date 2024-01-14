#pragma once

#include <memory>

#include "tools/tools.h"

class BaseCamera {
 public:
  virtual Ray getRay(const int &pixel_x, const int &pixel_y) const {
    return Ray();
  };
  virtual int hSize() const { return 0; }
  virtual int vSize() const { return 0; }
  virtual void computePixelSize() {}
  virtual void setTransform(const Mat4f &m) {}
};

using BaseCameraPtr = std::unique_ptr<BaseCamera>;

class RayTracingChalengeCamera : public BaseCamera {
 public:
  RayTracingChalengeCamera(const int &hs, const int &vs, const float &fv)
      : m_hsize(hs), m_vsize(vs), m_field_of_view(fv) {}

  void setHSize(const int &hs) { m_hsize = hs; }
  void setVSize(const int &vs) { m_vsize = vs; }
  void setFieldOfView(const float &fv) { m_field_of_view = fv; }
  void setTransform(const Mat4f &m) override { m_transform = m; }

  int hSize() const override { return m_hsize; }
  int vSize() const override { return m_vsize; }
  float fieldOfView() const { return m_field_of_view; }
  Mat4f transform() const { return m_transform; }
  float halfWidth() const { return m_half_width; }
  float halfHeight() const { return m_half_height; }
  float pixelSize() const { return m_pixel_size; }
  void setParamVectors(const Point3f &from, const Point3f &to,
                       const Vec3f &up) {
    m_from = from;
    m_to = to;
    m_up = up;
  }
  Point3f getFromPoint() const { return m_from; }
  Point3f getToPoint() const { return m_to; }
  Vec3f getUpVector() const { return m_up; }

  void computePixelSize() override {
    auto half_view = tan(m_field_of_view / 2.0f);
    auto aspect = static_cast<float>(m_hsize) / m_vsize;
    if (aspect >= 1.0f) {
      m_half_width = half_view;
      m_half_height = half_view / aspect;
    } else {
      m_half_width = half_view * aspect;
      m_half_height = half_view;
    }
    m_pixel_size = (m_half_width * 2.0f) / m_hsize;
  }

  Ray getRay(const int &pixel_x, const int &pixel_y) const override {
    auto fpixel_x = static_cast<float>(pixel_x);
    auto fpixel_y = static_cast<float>(pixel_y);
    Ray ray;
    auto xoffset = (fpixel_x + 0.5f) * pixelSize();
    auto yoffset = (fpixel_y + 0.5f) * pixelSize();
    auto world_x = halfWidth() - xoffset;
    auto world_y = halfHeight() - yoffset;
    Mat4f inversed = transform().inverse();
    Point3f pixel = inversed * Vec4f(world_x, world_y, -1.0f, 1.0f);
    ray.setOrigin(inversed * Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    ray.setDirection(
        (Point3f(pixel.x(), pixel.y(), pixel.z()) - ray.origin()).normalize());
    return ray;
  }

 protected:
  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Mat4f m_transform;
  float m_half_width;
  float m_half_height;
  float m_pixel_size;
  Point3f m_from;
  Point3f m_to;
  Vec3f m_up;
};
