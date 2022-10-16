#include <fstream>
#include <iostream>

#include "builder/shape_builder.h"
#include "composite/scene_director.h"
#include "composite/world.h"
#include "container/canvas.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  //----------------------Floor---------------------
  Properties floor_properties;
  floor_properties.setPatternColorA(Vec3f(1.0f, 1.0f, 1.0f));
  floor_properties.setPatternColorB(Vec3f(0.1f, 0.1f, 0.1f));
  shared_ptr<TraceableBuilder> floor_builder = make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> floor_director =
      make_shared<PlaneWithCheckerPattern>(floor_builder, floor_properties);
  shared_ptr<Traceable> floor = floor_director->create();

  //----------------------Left wall---------------------
  Properties left_wall_properties;
  left_wall_properties.setPatternColorA(Vec3f(0.1f, 0.1f, 0.1f));
  left_wall_properties.setPatternColorB(Vec3f(0.1f, 1.0f, 0.1f));
  left_wall_properties.setPatternTransformation(translation(0.0f, 0.0f, 5.0f));
  left_wall_properties.setObjectTransformation(translation(0.0f, 0.0f, 5.0f) *
                                               rotationX(-PI / 2.0f));
  shared_ptr<TraceableBuilder> left_wall_builder =
      make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> left_wall_director =
      make_shared<PlaneWithCheckerPattern>(left_wall_builder,
                                           left_wall_properties);
  shared_ptr<Traceable> left_wall = left_wall_director->create();

  //----------------------Right wall---------------------
  Properties right_wall_properties;
  right_wall_properties.setPatternColorA(Vec3f(0.1f, 0.1f, 0.1f));
  right_wall_properties.setPatternColorB(Vec3f(0.1f, 1.0f, 0.1f));
  right_wall_properties.setObjectTransformation(translation(0.0f, 0.0f, 5.0f) *
                                                rotationY(PI / 4.0f) *
                                                rotationX(-PI / 2.0f));
  right_wall_properties.setPatternTransformation(
      translation(0.0f, 2.0f, -0.5f) * scale(0.9f, 0.9f, 0.9f));
  shared_ptr<TraceableBuilder> right_wall_builder =
      make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> right_wall_director =
      make_shared<PlaneWithRingPattern>(right_wall_builder,
                                        right_wall_properties);
  shared_ptr<Traceable> right_wall = right_wall_director->create();

  //----------------------Middle sphere---------------------
  Properties middle_sphere_properties;
  middle_sphere_properties.setPatternColorA(Vec3f(0.1f, 0.1f, 1.0f));
  middle_sphere_properties.setPatternColorB(Vec3f(1.0f, 0.1f, 0.1f));
  middle_sphere_properties.setObjectTransformation(
      translation(-0.5f, 1.0f, 0.5f));
  middle_sphere_properties.setPatternTransformation(
      translation(-0.5f, 1.0f, 0.5f) * scale(0.6f, 0.6f, 0.6f));
  shared_ptr<TraceableBuilder> middle_sphere_builder =
      make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> middle_sphere_director =
      make_shared<SphereWithCheckerPattern>(middle_sphere_builder,
                                            middle_sphere_properties);
  shared_ptr<Traceable> middle = middle_sphere_director->create();

  //----------------------Right sphere---------------------
  Properties right_sphere_properties;
  right_sphere_properties.setPatternColorA(Vec3f(0.1f, 0.1f, 1.0f));
  right_sphere_properties.setPatternColorB(Vec3f(1.0f, 0.1f, 0.1f));
  right_sphere_properties.setObjectTransformation(
      translation(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) *
      rotationY(PI / 2.0f));
  right_sphere_properties.setPatternTransformation(
      translation(1.5f, 0.5f, -0.5f) * scale(0.1f, 0.1f, 0.1f));
  shared_ptr<TraceableBuilder> right_sphere_builder =
      make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> right_sphere_director =
      make_shared<SphereWithRingPattern>(right_sphere_builder,
                                         right_sphere_properties);
  shared_ptr<Traceable> right = right_sphere_director->create();

  //----------------------Left sphere---------------------
  Properties left_sphere_properties;
  left_sphere_properties.setPatternColorA(Vec3f(0.1f, 0.1f, 1.0f));
  left_sphere_properties.setPatternColorB(Vec3f(1.0f, 0.1f, 0.1f));
  left_sphere_properties.setObjectTransformation(
      translation(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f));
  left_sphere_properties.setPatternTransformation(
      translation(-1.5f, 0.33f, -0.75f) * scale(4.0f, 4.0f, 4.0f));
  shared_ptr<TraceableBuilder> left_sphere_builder =
      make_shared<StandardBuilder>();
  shared_ptr<SceneDirector> left_sphere_director =
      make_shared<SphereWithGradientPattern>(left_sphere_builder,
                                             left_sphere_properties);
  shared_ptr<Traceable> left = left_sphere_director->create();

  //----------------------------------------------------------------------------
  Canvas canvas(600, 600);
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
