#include "canvas.h"

#include <algorithm>
#include <fstream>
#include <thread>
#include <vector>

using std::endl;
using std::list;
using std::ofstream;

Canvas::Canvas(Camera &&camera, std::unique_ptr<BaseRenderer> renderer,
               int num_threads)
    : m_camera(camera),
      m_renderer(std::move(renderer)),
      m_num_threads(num_threads) {
  m_pixels =
      std::vector(m_camera.hSize(), std::vector<Vec3f>(m_camera.vSize()));
}

int Canvas::width() const { return m_pixels.size(); }

int Canvas::height() const { return m_pixels[0].size(); }

std::string Canvas::fileName() const { return m_fileName; }

void Canvas::setFileName(const std::string &fn) { m_fileName = fn; }

void Canvas::setNumThreads(int num_threads) { m_num_threads = num_threads; }

void Canvas::render(SceneElementPtr &&world) {
  for (int j = 0; j < m_camera.vSize(); ++j) {
    for (int i = 0; i < m_camera.hSize(); ++i) {
      auto color = Vec3f{};
      m_renderer->setPixelInfo(i, j);
      world->accept(*m_renderer, m_camera.getRay(i, j));
      color = m_renderer->getColor();
      writePixel(i, j, color);
    }
  }
}

void Canvas::renderRegion(int y_start, int y_end,
                          const SceneElementPtr &world) {
  for (int j = y_start; j < y_end; ++j) {
    for (int i = 0; i < m_camera.hSize(); ++i) {
      auto color = Vec3f{};
      m_renderer->setPixelInfo(i, j);
      world->accept(*m_renderer, m_camera.getRay(i, j));
      color = m_renderer->getColor();
      writePixel(i, j, color);
    }
  }
}

void Canvas::renderParallel(SceneElementPtr &&world) {
  int height = m_camera.vSize();
  int rows_per_thread = height / m_num_threads;
  std::vector<std::thread> threads;

  for (int t = 0; t < m_num_threads; ++t) {
    int y_start = t * rows_per_thread;
    int y_end = (t == m_num_threads - 1) ? height : (t + 1) * rows_per_thread;
    threads.emplace_back(&Canvas::renderRegion, this, y_start, y_end,
                         std::ref(world));
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void Canvas::writePixel(int x, int y, const Vec3f &color) {
  m_pixels[x][y] = color;
}

Vec3f Canvas::pixel(int x, int y) const { return m_pixels[x][y]; }

void Canvas::save() {
  APP_ASSERT(!fileName().empty(), "Cannot save scene, filename is empty!");

  ofstream out;
  out.open(fileName());

  out << "P3" << endl;
  out << width() << " " << height() << endl;
  out << "255" << endl;

  for (int j = 0; j < height(); ++j) {
    for (int i = 0; i < width(); ++i) {
      m_pixels[i][j] = m_pixels[i][j] * 255.f;
      if (m_pixels[i][j].x > 255.f) m_pixels[i][j].x = 255.f;
      if (m_pixels[i][j].x < 0.f) m_pixels[i][j].x = 0.f;
      if (m_pixels[i][j].y > 255.f) m_pixels[i][j].y = 255.f;
      if (m_pixels[i][j].y < 0.f) m_pixels[i][j].y = 0.f;
      if (m_pixels[i][j].z > 255.f) m_pixels[i][j].z = 255.f;
      if (m_pixels[i][j].z < 0.f) m_pixels[i][j].z = 0.f;

      out << (int)m_pixels[i][j].x << " " << (int)m_pixels[i][j].y << " "
          << (int)m_pixels[i][j].z << " ";
    }
    out << '\n';
  }

  out.close();
}
