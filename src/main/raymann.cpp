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
  auto prop_floor = MaterialProperties{};
  prop_floor.setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.4f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(rotationOverY(0.31415f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.65f, 0.65f, 0.65f)), prop_floor);
  builder->addElement();

  //----------------------North Wall---------------------
  auto prop_wall = MaterialProperties{};
  prop_wall.setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.3f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, 0.f, 5.f) *
                               rotationOverX(-1.5708f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)), prop_wall);
  builder->addElement();

  //----------------------Red Sphere----------------
  auto prop_red = MaterialProperties{};
  prop_red.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::SHININESS, 5.f);
  prop_red.setProperty(Props::SHININESS, 50.f);
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.6f, 1.f, 0.6f));
  builder->applyLambertianMaterial(
      make_unique<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)), prop_red);
  builder->addElement();

  SceneElementPtr world = builder->getProductBVHierarchy();

  //----------------------------------------------------------------------------
  auto canvas = Canvas(200, 200);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera =
      make_shared<Camera>(canvas.width(), canvas.height(), 1.152f);
  camera->computePixelSize();
  auto from = Point3f(-2.6f, 3.5f, -4.9f);
  auto to = Point3f(-0.6f, 1.0f, -0.8f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<BruteForceMC>(camera);
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
