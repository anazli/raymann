#include "canvas.h"

#include <algorithm>
#include <execution>
#include <fstream>

using std::endl;
using std::list;
using std::ofstream;
using std::execution::par;

Canvas::Canvas(Camera &&camera, std::unique_ptr<BaseRenderer> renderer)
    : m_camera(camera), m_renderer(std::move(renderer)) {
  m_pixels =
      std::vector(m_camera.hSize(), std::vector<Vec3f>(m_camera.vSize()));
}

int Canvas::width() const { return m_pixels.size(); }

int Canvas::height() const { return m_pixels[0].size(); }

std::string Canvas::fileName() const { return m_fileName; }

void Canvas::setFileName(const std::string &fn) { m_fileName = fn; }

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

void Canvas::renderParallel(SceneElementPtr &&world) {
  std::vector<int> hContainer;
  std::vector<int> vContainer;
  hContainer.resize(m_camera.hSize());
  vContainer.resize(m_camera.vSize());
  std::iota(hContainer.begin(), hContainer.end(), 0);
  std::iota(vContainer.begin(), vContainer.end(), 0);

  std::for_each(par, vContainer.begin(), vContainer.end(), [&](int j) {
    std::for_each(par, hContainer.begin(), hContainer.end(), [&](int i) {
      auto color = Vec3f{};
      m_renderer->setPixelInfo(i, j);
      world->accept(*m_renderer, m_camera.getRay(i, j));
      color = m_renderer->getColor();
      writePixel(i, j, color);
    });
  });
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
