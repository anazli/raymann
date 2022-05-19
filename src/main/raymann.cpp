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

  shared_ptr<SphereBuilder> builder = make_shared<StandardSphere>();

  shared_ptr<Traceable> sphere = scene.createTransformedSphere(
      builder, scale(0.8f, 1.0f, 1.0f), Vec3f(0.09f, 0.172f, 0.909f));

  shared_ptr<Traceable> sphere1 = scene.createTransformedSphere(
      builder, scale(0.8f, 1.0f, 1.0f), Vec3f(1.0f, 0.0f, 0.0f), 0.1f, 0.9f,
      0.9f, 10.0f, Point3f(2.0f, 0.0f, 0.0f));

  shared_ptr<Traceable> sphere2 = scene.createTransformedSphere(
      builder, scale(0.8f, 1.0f, 1.0f), Vec3f(0.0f, 1.0f, 0.0f), 0.1f, 0.9f,
      0.9f, 10.0f, Point3f(-2.0f, 0.0f, 0.0f));

  Canvas canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");

  Camera c(canvas.width(), canvas.height(), PI / 2.0f);
  c.computePixelSize();
  Point3f from(0.0f, 0.0f, -5.0f);
  Point3f to(0.0f, 0.0f, 0.0f);
  Vec3f up(0.0f, 1.0f, 0.0f);
  c.setTransform(view_transform(from, to, up));

  unique_ptr<Traceable> w = make_unique<World>();
  w->add(sphere);
  // w->add(sphere1);
  w->add(sphere2);

  canvas.render(move(w), c, light);
  canvas.save();

  return 0;
}
