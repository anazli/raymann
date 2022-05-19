#include "canvas.h"

#include <cassert>
#include <fstream>

using std::endl;
using std::list;
using std::ofstream;

void Canvas::render(std::unique_ptr<Traceable> world, const Camera &c,
                    const PointLight &light) {
  int num_samples = 1;
  for (int j = 0; j < c.vSize(); ++j) {
    for (int i = 0; i < c.hSize(); ++i) {
      Vec3f color;
      for (int s = 0; s < num_samples; ++s) {
        Ray ray(c, i, j);
        if (world->intersect(ray)) {
          Traceable &closest = world->closestHit();
          color += closest.lighting(light, ray, false);
        } else {
          color += Vec3f(0.0, 0.0, 0.0);
        }
      }
      color = color / (float)num_samples;
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
