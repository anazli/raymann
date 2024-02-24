#pragma once

#include <vector>

#include "camera/camera.h"
#include "composite/scene_element.h"
#include "renderers/renderer.h"

class BaseCamera;

class Canvas {
 public:
  Canvas(int w = 0, int h = 0);
  int width() const;
  int height() const;
  std::string fileName() const;
  void setFileName(const std::string &fn);
  void render(const SceneElementPtr &world, const BaseCameraPtr &camera,
              BaseRendererPtr renderer);
  void writePixel(int x, int y, const Vec3f &color);
  Vec3f pixel(int x, int y) const;
  void save();

 private:
  void fillImageResolutionIterators(int hSize, int vSize);
  int m_width;
  int m_height;
  std::string m_fileName;
  std::vector<std::vector<Vec3f>> m_pixels;
  std::vector<int> m_hContainer;
  std::vector<int> m_vContainer;
};
