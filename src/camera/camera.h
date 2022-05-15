#pragma once

#include "tools/mat4.h"

class Camera {
 public:
  Camera() = default;
  Camera(const int &hs, const int &vs, const float &fv)
      : m_hsize(hs), m_vsize(vs), m_field_of_view(fv) {}

  void setHSize(const int &hs) { m_hsize = hs; }
  void setVSize(const int &vs) { m_vsize = vs; }
  void setFieldOfView(const float &fv) { m_field_of_view = fv; }

  int hSize() const { return m_hsize; }
  int vSize() const { return m_vsize; }
  float fieldOfView() const { return m_field_of_view; }
  Mat4f transform() const { return m_transform; }

 private:
  int m_hsize;
  int m_vsize;
  float m_field_of_view;
  Mat4f m_transform;
};
