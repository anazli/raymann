#include <fstream>
#include <iostream>

#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "tools/tools.h"

using namespace std;

int main() {
  PointLight light(Point3f(-10.0f, 10.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  BuilderPtr world = make_shared<WorldBuilder>();
  world->createWorld(light);
  Properties prop;
  //----------------------Floor---------------------
  prop.setProperty(Props::PATTERN_COLORA, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(Props::PATTERN_COLORB, Vec3f(0.1f, 0.1f, 0.1f))
      .setProperty(Props::REFLECTION, 0.5f);
  world->createPlane();
  world->applyCheckerPattern(prop);
  world->addElement();

  //----------------------Left wall---------------------
  prop.setProperty(Props::COLOR, Vec3f(0.4f, 0.4f, 0.4f))
      .setProperty(Props::REFLECTION, 0.3f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX,
                   transl(0.0f, 0.0f, 5.0f) * rotX(-PI / 2.0f));
  world->createPlane();
  world->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  world->applyMaterial(prop);
  world->addElement();

  //----------------------Right wall---------------------
  prop.setProperty(
          Props::OBJECT_TRANSFROM_MATRIX,
          transl(0.0f, 0.0f, 5.0f) * rotY(PI / 4.0f) * rotX(-PI / 2.0f))
      .setProperty(Props::COLOR, Vec3f(0.8f, 0.8f, 0.8f))
      .setProperty(Props::REFLECTION, 0.7f);
  world->createPlane();
  world->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  world->applyMaterial(prop);
  world->addElement();

  //----------------------Middle sphere---------------------
  prop.setProperty(Props::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(Props::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, transl(-0.5f, 1.0f, 0.5f))
      .setProperty(Props::PATTERN_TRANSFORM_MATRIX,
                   transl(-0.5f, 1.0f, 0.5f) * scale(0.08f, 0.08f, 0.08f))
      .setProperty(Props::REFLECTION, 0.f);
  world->createSphere();
  world->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  world->applyCheckerPattern(prop);
  world->addElement();

  //----------------------Right sphere---------------------
  prop.setProperty(Props::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(Props::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(
          Props::OBJECT_TRANSFROM_MATRIX,
          transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f) * rotY(PI / 2.0f))
      .setProperty(Props::PATTERN_TRANSFORM_MATRIX,
                   transl(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f));
  world->createSphere();
  world->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  world->applyGradientPattern(prop);
  world->addElement();

  //----------------------Left sphere---------------------
  prop.setProperty(Props::PATTERN_COLORA, Vec3f(0.1f, 0.1f, 1.0f))
      .setProperty(Props::PATTERN_COLORB, Vec3f(1.0f, 0.1f, 0.1f))
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX,
                   transl(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f))
      .setProperty(Props::PATTERN_TRANSFORM_MATRIX,
                   transl(-1.5f, 0.33f, -0.75f) * scale(0.3f, 0.3f, 0.3f));
  world->createSphere();
  world->applyTransformation(
      prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
  world->applyRingPattern(prop);
  world->addElement();

  //----------------------------------------------------------------------------
  Canvas canvas(500, 500);
  canvas.setFileName("scenes/scene.ppm");
  Camera camera(canvas.width(), canvas.height(), PI / 4.0f);
  camera.computePixelSize();
  Point3f from(0.0f, 2.0f, -7.0f);
  Point3f to(0.0f, 1.0f, 0.0f);
  Vec3f up(0.0f, 1.0f, 0.0f);
  camera.setTransform(view_transform(from, to, up));
  canvas.render(world->getProduct(), camera);
  canvas.save();

  return 0;
}
