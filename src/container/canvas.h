#pragma once

#include <thread>
#include <vector>

#include "camera/camera.h"
#include "composite/scene_element.h"
#include "renderers/renderer.h"

class BaseCamera;

class Canvas {
 public:
  Canvas(Camera &&camera, std::unique_ptr<BaseRenderer> renderer,
         int num_threads = 4);
  Canvas(Canvas &&) = default;
  Canvas &operator=(Canvas &&) = default;
  Canvas(const Canvas &) = delete;
  Canvas &operator=(const Canvas &) = delete;

  int width() const;
  int height() const;
  std::string fileName() const;
  void setFileName(const std::string &fn);
  void setNumThreads(int num_threads);
  void render(SceneElementPtr &&world);
  void renderParallel(SceneElementPtr &&world);
  void writePixel(int x, int y, const Vec3f &color);
  Vec3f pixel(int x, int y) const;
  void save();

 private:
  void renderRegion(int y_start, int y_end, const SceneElementPtr &world);

  Camera m_camera;
  std::unique_ptr<BaseRenderer> m_renderer;
  std::string m_fileName;
  std::vector<std::vector<Vec3f>> m_pixels;
  int m_num_threads;
};
