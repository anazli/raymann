#include "canvas.h"

#include <algorithm>
#include <execution>
#include <fstream>

using std::endl;
using std::list;
using std::ofstream;
using std::execution::par;

Canvas::Canvas(int w, int h) : m_width(w), m_height(h) {
  m_pixels = std::vector(w, std::vector<Vec3f>(h));
}

int Canvas::width() const { return m_pixels.size(); }

int Canvas::height() const { return m_pixels[0].size(); }

std::string Canvas::fileName() const { return m_fileName; }

void Canvas::setFileName(const std::string &fn) { m_fileName = fn; }

void Canvas::render(const SceneElementPtr &world, const BaseCameraPtr &camera,
                    BaseRendererPtr renderer) {
  for (int j = 0; j < camera->vSize(); ++j) {
    for (int i = 0; i < camera->hSize(); ++i) {
      auto color = Vec3f{};
      renderer->setPixelInfo(i, j);
      world->accept(*renderer, camera->getRay(i, j));
      color = renderer->getColor();
      writePixel(i, j, color);
    }
  }
}

void Canvas::renderParallel(const SceneElementPtr &world,
                            const BaseCameraPtr &camera,
                            BaseRendererPtr renderer) {
  fillImageResolutionIterators(camera->hSize(), camera->vSize());
  std::for_each(par, m_vContainer.begin(), m_vContainer.end(), [&](int j) {
    std::for_each(par, m_hContainer.begin(), m_hContainer.end(), [&](int i) {
      auto color = Vec3f{};
      renderer->setPixelInfo(i, j);
      world->accept(*renderer, camera->getRay(i, j));
      color = renderer->getColor();
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
      m_pixels[i][j] *= 255.f;
      if (m_pixels[i][j].x() > 255.f) m_pixels[i][j].setX(255.f);
      if (m_pixels[i][j].x() < 0.f) m_pixels[i][j].setX(0.f);
      if (m_pixels[i][j].y() > 255.f) m_pixels[i][j].setY(255.f);
      if (m_pixels[i][j].y() < 0.f) m_pixels[i][j].setY(0.f);
      if (m_pixels[i][j].z() > 255.f) m_pixels[i][j].setZ(255.f);
      if (m_pixels[i][j].z() < 0.f) m_pixels[i][j].setZ(0.f);

      out << (int)m_pixels[i][j].x() << " " << (int)m_pixels[i][j].y() << " "
          << (int)m_pixels[i][j].z() << " ";
    }
    out << '\n';
  }

  out.close();
}

void Canvas::fillImageResolutionIterators(int hSize, int vSize) {
  for (int i = 0; i < hSize; ++i) m_hContainer.push_back(i);
  for (int j = 0; j < vSize; ++j) m_vContainer.push_back(j);
}
