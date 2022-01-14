#include <fstream>
#include <iostream>

#include "container/canvas.h"
#include "geometry/sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  PointLight light(Point3f(-15.0f, 15.0f, -15.0f), Vec3f(0.7f, 0.7f, 0.7f));
  Traceable *m =
      new Material(new Transformer(new Sphere(), scale(0.8f, 1.0f, 1.0f)),
                   Vec3f(0.2f, 1.0f, 0.2f));
  World w;
  w.add(m);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 10.0f));

  Canvas canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");

  canvas.render(w, r, light);
  canvas.save();

  return 0;
}
