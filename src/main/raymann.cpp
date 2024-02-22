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
#include "stochastic/stochastic_method.h"
#include "textures/texture.h"

using namespace std;

int main() {
  auto light = PointLight(Point3f(-4.9f, 4.9f, -1.f), Vec3f(1.0f, 1.0f, 1.0f));
  auto builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();

  //----------------------1st Sphere----------------
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.6f, -1.f, 0.6f) *
                               scale(6.f, 1.f, 6.f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.4f, 0.4f, 0.4f)),
      MaterialProperties{});
  builder->addElement();

  //----------------------2nd Sphere----------------
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.6f, 1.f, 0.6f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.4f, 0.4f, 0.4f)),
      MaterialProperties{});
  builder->addElement();

  auto world = builder->getProductBVHierarchy();

  //----------------------------------------------------------------------------
  auto canvas = Canvas(300, 300);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = make_shared<Camera>(canvas.width(), canvas.height(), 1.152f);
  auto from = Point3f(-2.6f, 1.5f, -4.f);
  auto to = Point3f(-0.6f, 1.0f, -0.8f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer =
      make_unique<PathTracer>(std::make_unique<BruteForceMC>(camera));
  renderer->setBackgroundColor(Vec3f(0.8f, 0.8f, 0.8f));
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
