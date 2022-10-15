#include <fstream>
#include <iostream>

#include "builder/shape_builder.h"
#include "composite/world.h"
#include "container/canvas.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  Vec3f pattern_colorA(1.0f, 1.0f, 1.0f);
  Vec3f pattern_colorB(0.1f, 0.1f, 0.1f);
  Mat4f object_transform =
      translation(0.0f, 0.0f, 5.0f) * rotationX(-PI / 2.0f);
  Mat4f pattern_transform;

  //----------------------Floor---------------------
  shared_ptr<TraceableBuilder> plane_builder = make_shared<Shape>();
  plane_builder->addTraceable(new Plane);
  plane_builder->addCheckerPattern(pattern_colorA, pattern_colorB,
                                   object_transform, pattern_transform);
  shared_ptr<Traceable> floor = plane_builder->getTraceable();

  //----------------------Left wall---------------------
  pattern_colorA = Vec3f(0.1f, 0.1f, 0.1f);
  pattern_colorB = Vec3f(0.1f, 1.0f, 0.1f);
  pattern_transform = translation(0.0f, 0.0f, 5.0f);
  object_transform = translation(0.0f, 0.0f, 5.0f) * rotationX(-PI / 2.0f);
  plane_builder->addTraceable(new Plane);
  plane_builder->transformTraceable(object_transform);
  plane_builder->addCheckerPattern(pattern_colorA, pattern_colorB,
                                   object_transform, pattern_transform);
  shared_ptr<Traceable> left_wall = plane_builder->getTraceable();

  //----------------------Right wall---------------------
  object_transform = translation(0.0f, 0.0f, 5.0f) * rotationY(PI / 4.0f) *
                     rotationX(-PI / 2.0f);
  pattern_transform = translation(0.0f, 2.0f, -0.5f) * scale(0.9f, 0.9f, 0.9f);
  plane_builder->addTraceable(new Plane);
  plane_builder->transformTraceable(object_transform);
  plane_builder->addRingPattern(pattern_colorA, pattern_colorB,
                                object_transform, pattern_transform);
  shared_ptr<Traceable> right_wall = plane_builder->getTraceable();
  //----------------------------------------------------------------------------

  pattern_colorA = Vec3f(0.1f, 0.1f, 1.0f);
  pattern_colorB = Vec3f(1.0f, 0.1f, 0.1f);
  shared_ptr<TraceableBuilder> sphere_builder = make_shared<Shape>();
  //----------------------Middle sphere---------------------
  object_transform = translation(-0.5f, 1.0f, 0.5f);
  pattern_transform = translation(-0.5f, 1.0f, 0.5f) * scale(0.5f, 0.5f, 0.5f);
  sphere_builder->addTraceable(new Sphere);
  sphere_builder->transformTraceable(object_transform);
  sphere_builder->addCheckerPattern(pattern_colorA, pattern_colorB,
                                    object_transform, pattern_transform);
  shared_ptr<Traceable> middle = sphere_builder->getTraceable();

  //----------------------Right sphere---------------------
  object_transform = translation(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) *
                     rotationY(PI / 2.0f);
  pattern_transform = translation(1.5f, 0.5f, -0.5f) * scale(0.1f, 0.1f, 0.1f);
  sphere_builder->addTraceable(new Sphere);
  sphere_builder->transformTraceable(object_transform);
  sphere_builder->addRingPattern(pattern_colorA, pattern_colorB,
                                 pattern_transform, object_transform);
  shared_ptr<Traceable> right = sphere_builder->getTraceable();

  //----------------------Left sphere---------------------
  object_transform =
      translation(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f);
  pattern_transform =
      translation(-1.5f, 0.33f, -0.75f) * scale(4.0f, 4.0f, 4.0f);
  sphere_builder->addTraceable(new Sphere);
  sphere_builder->transformTraceable(object_transform);
  sphere_builder->addGradientPattern(pattern_colorA, pattern_colorB,
                                     pattern_transform, object_transform);
  shared_ptr<Traceable> left = sphere_builder->getTraceable();

  Canvas canvas(800, 800);
  canvas.setFileName("scenes/scene.ppm");
  Camera c(canvas.width(), canvas.height(), PI / 3.0f);
  c.computePixelSize();
  Point3f from(0.0f, 2.0f, -7.0f);
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
