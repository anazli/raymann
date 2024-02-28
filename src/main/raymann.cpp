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
  auto light = PointLight(Point3f(80.f, 70.f, 200.f), Vec3f(1.f, 1.f, 1.f));

  WavefrontReader reader("dragon.obj");
  reader.addLightForModel(light);
  reader.addMaterial(make_shared<Lambertian>(
      make_unique<ConstantTexture>(Vec3f(0.08f, 0.683f, 0.382f))));
  reader.parseInput();
  SceneElementPtr world = reader.getStructureBVHierarchy();

  BuilderPtr builder = make_unique<WorldBuilder>();
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, -40.f, 0.f) *
                               rotationOverY(PI));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.25f, 0.25f, 0.75f)));
  SceneElementPtr floor(builder->getCurrentElement());
  world->add(floor);

  auto canvas = Canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = make_shared<Camera>(canvas.width(), canvas.height(), 1.5f);
  auto from = Point3f(0.f, -15.f, 130.f);
  auto to = Point3f(30.f, 30.0f, -40.f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  int samplesPerPixel = 100;
  int materialDepth = 30;
  BaseRendererPtr renderer =
      make_unique<PathTracer>(std::make_unique<StratifiedSampler>(
          camera, samplesPerPixel, materialDepth));
  renderer->setBackgroundColor(Vec3f(0.55f, 0.4f, 0.4f));
  chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
  canvas.renderParallel(world, camera, std::move(renderer));
  canvas.save();

  chrono::time_point<chrono::steady_clock> end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
