#pragma once

#include "tools/mat4.h"
#include "tools/ray.h"

class Camera {
 public:
  Camera() = default;
  Camera(const int &hs, const int &vs, const float &fv)
      : m_hsize(hs), m_vsize(vs), m_field_of_view(fv) {}

  void setHSize(const int &hs) { m_hsize = hs; }
  void setVSize(const int &vs) { m_vsize = vs; }
  void setFieldOfView(const float &fv) { m_field_of_view = fv; }
  void setTransform(const Mat4f &m) { m_transform = m; }

  int hSize() const { return m_hsize; }
  int vSize() const { return m_vsize; }
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

  void computePixelSize() {
    float half_view = tan(m_field_of_view / 2.0f);
    float aspect = static_cast<float>(m_hsize) / m_vsize;
    if (aspect >= 1.0f) {
      m_half_width = half_view;
      m_half_height = half_view / aspect;
    } else {
      m_half_width = half_view * aspect;
      m_half_height = half_view;
    }
    m_pixel_size = (m_half_width * 2.0f) / m_hsize;
  }

  Ray getRay(float pixel_x, float pixel_y) const {
    Ray ray;
    float xoffset = (pixel_x + 0.5f) * pixelSize();
    float yoffset = (pixel_y + 0.5f) * pixelSize();
    float world_x = halfWidth() - xoffset;
    float world_y = halfHeight() - yoffset;
    Mat4f inversed = transform().inverse();
    Point3f pixel = inversed * Vec4f(world_x, world_y, -1.0f, 1.0f);
    ray.setOrigin(inversed * Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    ray.setDirection(
        (Point3f(pixel.x(), pixel.y(), pixel.z()) - ray.origin()).normalize());
    return ray;
  }

 private:
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
