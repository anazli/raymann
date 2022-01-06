#include <fstream>
#include <iostream>

#include "canvas.h"
#include "sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  Traceable *t = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  World w;
  w.add(t);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));

  Canvas canvas(100, 100);
  canvas.setFileName("scene.ppm");
  canvas.save();

  return 0;
}
