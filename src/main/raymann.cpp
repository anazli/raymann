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
  Properties fl_prop;
  fl_prop.setPatColA(Vec3f(1.0f, 1.0f, 1.0f))
      .setPatColB(Vec3f(0.1f, 0.1f, 0.1f));
  TraceableBuilderPtr fl_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr fl_dir = make_shared<CheckerPlane>(fl_builder, fl_prop);
  TraceablePtr floor = fl_dir->create();

  //----------------------Left wall---------------------
  Properties lw_prop;
  lw_prop.setPatColA(Vec3f(0.1f, 0.1f, 0.1f))
      .setPatColB(Vec3f(0.1f, 1.0f, 0.1f))
      .setPatTrans(transl(0.0f, 0.0f, 5.0f))
      .setObjTrans(transl(0.0f, 0.0f, 5.0f) * rotX(-PI / 2.0f));
  TraceableBuilderPtr lw_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr lw_dir = make_shared<CheckerPlane>(lw_builder, lw_prop);
  TraceablePtr left_wall = lw_dir->create();

  //----------------------Right wall---------------------
  Properties rw_prop;
  rw_prop.setPatColA(Vec3f(0.1f, 0.1f, 0.1f))
      .setPatColB(Vec3f(0.1f, 1.0f, 0.1f))
      .setObjTrans(transl(0.0f, 0.0f, 5.0f) * rotY(PI / 4.0f) *
                   rotX(-PI / 2.0f))
      .setPatTrans(transl(0.0f, 2.0f, -0.5f) * scale(0.9f, 0.9f, 0.9f));
  TraceableBuilderPtr rw_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr rw_dir = make_shared<RingPlane>(rw_builder, rw_prop);
  TraceablePtr right_wall = rw_dir->create();

  //----------------------Middle sphere---------------------
  Properties ms_prop;
  ms_prop.setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(-0.5f, 1.0f, 0.5f))
      .setPatTrans(transl(-0.5f, 1.0f, 0.5f) * scale(0.6f, 0.6f, 0.6f));
  TraceableBuilderPtr ms_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr ms_dir = make_shared<CheckerSphere>(ms_builder, ms_prop);
  TraceablePtr middle = ms_dir->create();

  //----------------------Right sphere---------------------
  Properties rs_prop;
  rs_prop.setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) *
                   rotY(PI / 2.0f))
      .setPatTrans(transl(1.5f, 0.5f, -0.5f) * scale(0.1f, 0.1f, 0.1f));
  TraceableBuilderPtr rs_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr rs_dir = make_shared<RingSphere>(rs_builder, rs_prop);
  TraceablePtr right = rs_dir->create();

  //----------------------Left sphere---------------------
  Properties ls_prop;
  ls_prop.setPatColA(Vec3f(0.1f, 0.1f, 1.0f))
      .setPatColB(Vec3f(1.0f, 0.1f, 0.1f))
      .setObjTrans(transl(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f))
      .setPatTrans(transl(-1.5f, 0.33f, -0.75f) * scale(4.0f, 4.0f, 4.0f));
  TraceableBuilderPtr ls_builder = make_shared<ShapeBuilder>();
  SceneDirectorPtr ls_dir = make_shared<GradientSphere>(ls_builder, ls_prop);
  TraceablePtr left = ls_dir->create();

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
