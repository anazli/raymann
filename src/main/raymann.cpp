#include <chrono>
//#include <fstream>
#include <iostream>

#include "application/wavefront_reader.h"
#include "camera/camera.h"
#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "renderers/path_tracer.h"
#include "renderers/phong_model.h"
#include "textures/texture.h"

using namespace std;

int main() {
  auto light = PointLight(Point3f(-4.9f, 4.9f, -1.f), Vec3f(1.0f, 1.0f, 1.0f));
  auto builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();
  //----------------------Floor---------------------
  builder->processSceneElement(new Plane);
  builder->applyTransformation(rotationOverY(0.31415f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.65f, 0.65f, 0.65f)),
      MaterialProperties{});
  builder->addElement();

  //----------------------North Wall---------------------
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, 0.f, 5.f) *
                               rotationOverX(-1.5708f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
      MaterialProperties{});
  builder->addElement();

  //----------------------Red Sphere----------------
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.6f, 1.f, 0.6f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)),
      MaterialProperties{});
  builder->addElement();

  SceneElementPtr world = builder->getProductBVHierarchy();

  //----------------------------------------------------------------------------
  auto canvas = Canvas(200, 200);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera =
      make_shared<Camera>(canvas.width(), canvas.height(), 1.152f);
  auto from = Point3f(-2.6f, 3.5f, -4.f);
  auto to = Point3f(-0.6f, 1.0f, -0.8f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<BruteForceMC>(camera, 50);
  renderer->setBackgroundColor(Vec3f(0.5f, 0.3f, 0.3f));
  chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
  canvas.render(world, camera, std::move(renderer));
  canvas.save();

  chrono::time_point<chrono::steady_clock> end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
