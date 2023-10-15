#include "canvas.h"

#include <cassert>
#include <fstream>
#include <list>

#include "camera/camera.h"
#include "renderers/renderer.h"

using std::endl;
using std::list;
using std::ofstream;

void Canvas::render(const SceneElementPtr &world, const BaseCamera &c,
                    BaseRenderer &renderer) {
  for (int j = 0; j < c.vSize(); ++j) {
    for (int i = 0; i < c.hSize(); ++i) {
      auto color = Vec3f{};
      renderer.setPixelInfo(i, j);
      world->accept(renderer, c.getRay(i, j));
      color = renderer.getColor();
      writePixel(i, j, color);
    }
  }
}

void Canvas::save() {
  assert(fileName() != "");

  ofstream out;
  out.open(fileName());

  out << "P3" << endl;
  out << width() << " " << height() << endl;
  out << "255" << endl;

  for (int j = 0; j < height(); ++j) {
    for (int i = 0; i < width(); ++i) {
      m_pixels[i][j] *= 255.;
      if (m_pixels[i][j].x() > 255.) m_pixels[i][j].setX(255.);
      if (m_pixels[i][j].x() < 0.) m_pixels[i][j].setX(0.);
      if (m_pixels[i][j].y() > 255.) m_pixels[i][j].setY(255.);
      if (m_pixels[i][j].y() < 0.) m_pixels[i][j].setY(0.);
      if (m_pixels[i][j].z() > 255.) m_pixels[i][j].setZ(255.);
      if (m_pixels[i][j].z() < 0.) m_pixels[i][j].setZ(0.);

      out << (int)m_pixels[i][j].x() << " " << (int)m_pixels[i][j].y() << " "
          << (int)m_pixels[i][j].z() << " ";
    }
    out << '\n';
  }

  out.close();
}
