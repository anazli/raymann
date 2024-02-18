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
#include "renderers/phong_model.h"
#include "textures/texture.h"

using namespace std;

int main() {
  auto light = PointLight(Point3f(80.f, 70.f, 200.f), Vec3f(1.f, 1.f, 1.f));

  WavefrontReader reader("dragon.obj");
  reader.addLightForModel(light);
  auto dragon_props = MaterialProperties{};
  dragon_props.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::SHININESS, 5.f);
  reader.addMaterial(make_unique<ConstantTexture>(Vec3f(0.08f, 0.683f, 0.382f)),
                     dragon_props);
  reader.parseInput();
  SceneElementPtr world = reader.getStructureBVHierarchy();

  BuilderPtr builder = make_unique<WorldBuilder>();
  auto floor_props = MaterialProperties{};
  floor_props.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::REFLECTION, 0.f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, -40.f, 0.f) *
                               rotationOverY(PI));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.35f, 0.35f, 0.65f)), floor_props);
  SceneElementPtr floor(builder->getCurrentElement());
  world->add(floor);

  //----------------------------------------------------------------------------
  auto canvas = Canvas(1200, 800);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera =
      make_unique<Camera>(canvas.width(), canvas.height(), 1.5f);
  camera->computePixelSize();
  auto from = Point3f(0.f, -15.f, 130.f);
  auto to = Point3f(30.f, 30.0f, -40.f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<PhongModel>();
  renderer->setBackgroundColor(Vec3f(0.5f, 0.3f, 0.3f));
  chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
  canvas.render(world, std::move(camera), std::move(renderer));
  canvas.save();

  chrono::time_point<chrono::steady_clock> end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
