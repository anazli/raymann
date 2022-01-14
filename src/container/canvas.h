#pragma once

#include <string>
#include <vector>

#include "geometry/traceable.h"
#include "tools/tools.h"

class Canvas {
 public:
  Canvas(const int &w = 0, const int &h = 0) : m_width(w), m_height(h) {
    m_pixels = std::vector(w, std::vector<Vec3f>(h));
  }
  int width() const { return m_pixels.size(); }
  int height() const { return m_pixels[0].size(); }
  std::string fileName() const { return m_fileName; }
  // void setWidth(const int &w) { m_width = w; }
  // void setHeight(const int &h) { m_height = h; }
  void setFileName(const std::string &fn) { m_fileName = fn; }

  void render(World &w, const Ray &r, const PointLight &light);
  void writePixel(const int &x, const int &y, const Vec3f &color) {
    m_pixels[x][y] = color;
  }

  Vec3f pixel(const int &x, const int &y) const { return m_pixels[x][y]; }

  void save();

 private:
  int m_width;
  int m_height;
  std::string m_fileName;
  std::vector<std::vector<Vec3f>> m_pixels;
};
