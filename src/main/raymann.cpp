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
  //----------------------Floor---------------------
  prop.setPatColA(Vec3f(1.0f, 1.0f, 1.0f)).setPatColB(Vec3f(0.1f, 0.1f, 0.1f));
  SceneDirectorPtr fl_dir = make_shared<CheckerPlane>();
  TraceablePtr floor = fl_dir->create(builder, prop);

  //----------------------Left wall---------------------
  prop.reset()
      .setPatColA(Vec3f(0.1f, 0.1f, 0.1f))
      .setPatColB(Vec3f(0.1f, 1.0f, 0.1f))
      .setPatTrans(transl(0.0f, 0.0f, 5.0f))
      .setObjTrans(transl(0.0f, 0.0f, 5.0f) * rotX(-PI / 2.0f));
  SceneDirectorPtr lw_dir = make_shared<CheckerPlane>();
  TraceablePtr left_wall = lw_dir->create(builder, prop);

  //----------------------Right wall---------------------
  prop.reset()
      .setPatColA(Vec3f(0.1f, 0.1f, 0.1f))
      .setPatColB(Vec3f(0.1f, 1.0f, 0.1f))
      .setObjTrans(transl(0.0f, 0.0f, 5.0f) * rotY(PI / 4.0f) *
                   rotX(-PI / 2.0f))
      .setPatTrans(transl(0.0f, 2.0f, -0.5f) * scale(0.9f, 0.9f, 0.9f));
  SceneDirectorPtr rw_dir = make_shared<RingPlane>();
  TraceablePtr right_wall = rw_dir->create(builder, prop);

  //----------------------Middle sphere---------------------
  prop.reset()
      .setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(-0.5f, 1.0f, 0.5f))
      .setPatTrans(transl(-0.5f, 1.0f, 0.5f) * scale(0.6f, 0.6f, 0.6f));
  SceneDirectorPtr ms_dir = make_shared<CheckerSphere>();
  TraceablePtr middle = ms_dir->create(builder, prop);

  //----------------------Right sphere---------------------
  prop.reset()
      .setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) *
                   rotY(PI / 2.0f))
      .setPatTrans(transl(1.5f, 0.5f, -0.5f) * scale(0.1f, 0.1f, 0.1f));
  SceneDirectorPtr rs_dir = make_shared<RingSphere>();
  TraceablePtr right = rs_dir->create(builder, prop);

  //----------------------Left sphere---------------------
  prop.reset()
      .setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f))
      .setPatTrans(transl(-1.5f, 0.33f, -0.75f) * scale(4.0f, 4.0f, 4.0f));
  SceneDirectorPtr ls_dir = make_shared<GradientSphere>();
  TraceablePtr left = ls_dir->create(builder, prop);

  //----------------------------------------------------------------------------
  Canvas canvas(500, 500);
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
