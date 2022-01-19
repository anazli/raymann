#include "canvas.h"

#include <cassert>
#include <fstream>

using std::endl;
using std::list;
using std::ofstream;

void Canvas::render(Traceable *world, const Ray &r, const PointLight &light) {
  int num_samples = 50;
  for (int j = 0; j < height(); ++j) {
    for (int i = 0; i < width(); ++i) {
      float wall_z = r.direction().z();
      float wall_size = 7.0f;
      float pixel_size = wall_size / (float)width();
      float half = wall_size / 2.0f;

      Vec3f color;
      for (int s = 0; s < num_samples; ++s) {
        float world_y = half - pixel_size * (j + drand48());
        float world_x = -half + pixel_size * (i + drand48());
        Point3f position(world_x, world_y, wall_z);
        Ray ray(r.origin(), (position - r.origin()).normalize());

        if (world->intersect(ray)) {
          Traceable &closest = world->closestHit();
          Point3f point = ray.position(closest.record().t_min());
          Vec3f eye = -r.direction();
          color += closest.lighting(light, point, eye);
        } else {
          color += Vec3f(0.1, 0.1, 0.2);
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
