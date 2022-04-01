#include <fstream>
#include <iostream>

#include "container/canvas.h"
#include "geometry/sphere.h"
#include "tools/tools.h"
#include "world/scene.h"
#include "world/world.h"

using namespace std;

int main() {
  Scene scene;
  PointLight light(Point3f(-15.0f, 15.0f, -15.0f), Vec3f(0.7f, 0.7f, 0.7f));

  shared_ptr<SphereBuilder> builder(new StandardSphere);

  shared_ptr<Traceable> sphere = scene.createTransformedSphere(
      builder, scale(0.8f, 1.0f, 1.0f), Vec3f(0.09f, 0.172f, 0.909f));

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 10.0f));

  Canvas canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");

  unique_ptr<Traceable> w(new World());
  w->add(sphere);

  canvas.render(move(w), r, light);
  canvas.save();

  return 0;
}
