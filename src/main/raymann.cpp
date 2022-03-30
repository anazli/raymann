#include <fstream>
#include <iostream>

#include "container/canvas.h"
#include "geometry/sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  PointLight light(Point3f(-15.0f, 15.0f, -15.0f), Vec3f(0.7f, 0.7f, 0.7f));

  Traceable *sphere = new Sphere();
  sphere = new Material(new Transformer(sphere, scale(0.8f, 1.0f, 1.0f)),
                        Vec3f(0.09f, 0.172f, 0.909f));

  /*sphere = new Transformer(sphere, translation(-0.6f, 0.0f, 0.0f));
  Traceable *sphere2 = new Sphere();
  sphere2 = new Material(new Transformer(sphere2, scale(0.5f, 1.0f, 1.0f)),
                         Vec3f(0.2f, 1.0f, 0.2f));
  sphere2 = new Transformer(sphere2, translation(0.6f, 0.0f, 0.0f));
  */

  unique_ptr<Traceable> w;
  w.reset(new World());

  shared_ptr<Traceable> shared_sphere;
  shared_sphere.reset(sphere);
  w->add(shared_sphere);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 10.0f));

  Canvas canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");

  canvas.render(move(w), r, light);
  canvas.save();

  return 0;
}
