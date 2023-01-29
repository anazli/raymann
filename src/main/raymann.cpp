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
  TraceableBuilderPtr builder = make_shared<ShapeBuilder>();
  Properties prop;
  prop.setProperty(PropertyNames::AMBIENT, 0.1f)
      .setProperty(PropertyNames::DIFFUSE, 0.9f)
      .setProperty(PropertyNames::SPECULAR, 0.9f)
      .setProperty(PropertyNames::SHININESS, 200.f);
  //----------------------Floor---------------------
  prop.setProperty(PropertyNames::PATTERN_COLORA, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(PropertyNames::PATTERN_COLORB, Vec3f(0.1f, 0.1f, 0.1f))
      .setProperty(PropertyNames::REFLECTION, 0.5f);
  SceneDirectorPtr fl_dir = make_shared<CheckerPlane>();
  TraceablePtr floor = fl_dir->create(builder, prop);

  //----------------------Left wall---------------------
  prop.setProperty(PropertyNames::COLOR, Vec3f(0.4f, 0.4f, 0.4f))
      .setProperty(PropertyNames::REFLECTION, 0.3f)
      .setProperty(PropertyNames::OBJECT_TRANSFROM_MATRIX,
                   transl(0.0f, 0.0f, 5.0f) * rotX(-PI / 2.0f));
  SceneDirectorPtr lw_dir = make_shared<StandardPlane>();
  TraceablePtr left_wall = lw_dir->create(builder, prop);

  //----------------------Right wall---------------------
  prop.setProperty(
          PropertyNames::OBJECT_TRANSFROM_MATRIX,
          transl(0.0f, 0.0f, 5.0f) * rotY(PI / 4.0f) * rotX(-PI / 2.0f))
      .setProperty(PropertyNames::COLOR, Vec3f(0.8f, 0.8f, 0.8f))
      .setProperty(PropertyNames::REFLECTION, 0.7f);
  SceneDirectorPtr rw_dir = make_shared<StandardPlane>();
  TraceablePtr right_wall = rw_dir->create(builder, prop);

  //----------------------Middle sphere---------------------
  prop.setProperty(PropertyNames::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(PropertyNames::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(PropertyNames::OBJECT_TRANSFROM_MATRIX,
                   transl(-0.5f, 1.0f, 0.5f))
      .setProperty(PropertyNames::PATTERN_TRANSFORM_MATRIX,
                   transl(-0.5f, 1.0f, 0.5f) * scale(0.08f, 0.08f, 0.08f));
  SceneDirectorPtr ms_dir = make_shared<CheckerSphere>();
  TraceablePtr middle = ms_dir->create(builder, prop);
  middle->addPerlinNoise();

  //----------------------Right sphere---------------------
  prop.setProperty(PropertyNames::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(PropertyNames::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(
          PropertyNames::OBJECT_TRANSFROM_MATRIX,
          transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) * rotY(PI / 2.0f))
      .setProperty(PropertyNames::PATTERN_TRANSFORM_MATRIX,
                   transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f));
  SceneDirectorPtr rs_dir = make_shared<GradientSphere>();
  TraceablePtr right = rs_dir->create(builder, prop);
  right->addPerlinNoise();

  //----------------------Left sphere---------------------
  prop.setProperty(PropertyNames::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(PropertyNames::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(PropertyNames::OBJECT_TRANSFROM_MATRIX,
                   transl(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f))
      .setProperty(PropertyNames::PATTERN_TRANSFORM_MATRIX,
                   transl(-1.5f, 0.33f, -0.75f) * scale(0.3f, 0.3f, 0.3f));
  SceneDirectorPtr ls_dir = make_shared<RingSphere>();
  TraceablePtr left = ls_dir->create(builder, prop);
  left->addPerlinNoise();

  //----------------------------------------------------------------------------
  Canvas canvas(500, 500);
  canvas.setFileName("scenes/scene.ppm");
  Camera c(canvas.width(), canvas.height(), PI / 4.0f);
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
