#include <fstream>
#include <iostream>

#include "container/canvas.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "tools/tools.h"
#include "world/world.h"

using namespace std;

int main() {
  shared_ptr<SphereBuilder> builder = make_shared<StandardSphere>();

  shared_ptr<Traceable> floor(
      new Material(new Plane(), Vec3f(1.0f, 0.9f, 0.9f), 0.1f, 0.9f, 0.0f));

  builder->addSphere();
  builder->transformSphere(translation(0.0f, 0.0f, 5.0f) *
                           rotationY(-PI / 4.0f) * rotationX(PI / 2.0f) *
                           scale(10.0f, 0.01f, 10.0f));
  builder->addColor(Vec3f(1.0f, 0.9f, 0.9f), 0.1f, 0.9f, 0.0f);
  shared_ptr<Traceable> left_wall = builder->getSphere();

  builder->addSphere();
  builder->transformSphere(translation(0.0f, 0.0f, 5.0f) *
                           rotationY(PI / 4.0f) * rotationX(PI / 2.0f) *
                           scale(10.0f, 0.01f, 10.0f));
  builder->addColor(Vec3f(1.0f, 0.9f, 0.9f), 0.1f, 0.9f, 0.0f);
  shared_ptr<Traceable> right_wall = builder->getSphere();

  builder->addSphere();
  builder->transformSphere(translation(-0.5f, 1.0f, 0.5f));
  // builder->addColor(Vec3f(0.1f, 0.0f, 1.0f), 0.1f, 0.7f, 0.3f);
  builder->addStripePattern(Vec3f(1.0f, 0.1f, 0.1f), Vec3f(0.1f, 0.1f, 1.0f));
  shared_ptr<Traceable> middle = builder->getSphere();

  builder->addSphere();
  builder->transformSphere(translation(1.5f, 0.5f, -0.5f) *
                           scale(0.5f, 0.5f, 0.5f));
  builder->addColor(Vec3f(1.0f, 0.0f, 0.1f), 0.1f, 0.7f, 0.3f);
  shared_ptr<Traceable> right = builder->getSphere();

  builder->addSphere();
  builder->transformSphere(translation(-1.5f, 0.33f, -0.75f) *
                           scale(0.33f, 0.33f, 0.33f));
  builder->addColor(Vec3f(0.8f, 0.8f, 0.8f), 0.1f, 0.7f, 0.3f);
  shared_ptr<Traceable> left = builder->getSphere();

  Canvas canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");
  Camera c(canvas.width(), canvas.height(), PI / 3.0f);
  c.computePixelSize();
  Point3f from(0.0f, 1.5f, -5.0f);
  Point3f to(0.0f, 1.0f, 0.0f);
  Vec3f up(0.0f, 1.0f, 0.0f);
  c.setTransform(view_transform(from, to, up));

  PointLight light(Point3f(-10.0f, 10.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  shared_ptr<Traceable> w = make_shared<World>();
  w->setLight(light);
  w->add(floor);
  w->add(left_wall);
  w->add(right_wall);
  w->add(middle);
  w->add(right);
  w->add(left);

  canvas.render(w, c);
  canvas.save();

  return 0;
}
