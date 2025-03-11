#pragma once

#include <memory>

#include "application/error.h"
#include "tools/tools.h"

class Camera {
 public:
  Camera(int hs, int vs, float fv);
  Ray getRay(int pixel_x, int pixel_y) const;
  int hSize() const;
  int vSize() const;
  void setTransform(const Mat4D &m);
  float pixelSize() const;

 private:
  void computePixelSize();

  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Mat4D m_transform;
  Mat4D m_transformInv;
  float m_pixel_size;
  float m_half_width;
  float m_half_height;
};

using CameraPtr = std::shared_ptr<Camera>;
