#pragma once

#include <vector>

#include "camera/camera.h"
#include "composite/scene_element.h"
#include "renderers/renderer.h"

class BaseCamera;

class Canvas {
 public:
  Canvas(Camera &&camera, std::unique_ptr<BaseRenderer> renderer);
  int width() const;
  int height() const;
  std::string fileName() const;
  void setFileName(const std::string &fn);
  void render(const SceneElementPtr &world);
  void renderParallel(const SceneElementPtr &world);
  void writePixel(int x, int y, const Vec3f &color);
  Vec3f pixel(int x, int y) const;
  void save();

 private:
  Camera m_camera;
  std::unique_ptr<BaseRenderer> m_renderer;
  std::string m_fileName;
  std::vector<std::vector<Vec3f>> m_pixels;
};
