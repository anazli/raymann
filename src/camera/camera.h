#pragma once

#include <mat4.h>
#include <ray.h>

#include <memory>

#include "application/error.h"

class Camera {
 public:
  Camera() = default;
  Camera(int hs, int vs, float fv);
  Ray getRay(int pixel_x, int pixel_y) const;
  void setHSize(int h_size);
  void setVSize(int v_size);
  void setFieldOfView(float fov);
  int hSize() const;
  int vSize() const;
  void setTransform(const Mat4f &m);
  Mat4f getTransform() const;
  float pixelSize() const;
  void computePixelSize();

 private:
  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Mat4f m_transform;
  Mat4f m_transformInv;
  float m_pixel_size;
  float m_half_width;
  float m_half_height;
};
