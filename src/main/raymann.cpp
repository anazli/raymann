#include <chrono>
//#include <fstream>
#include <iostream>

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
  auto light = PointLight(Point3f(-2.f, 4.9f, -10.f), Vec3f(1.0f, 1.0f, 1.0f));
  auto builder = make_unique<WorldBuilder>();
  builder->createWorld(light);
  int number_per_axis = 10;

  //----------------------Perlin Spheres----------------
  auto prop = MaterialProperties{};
  prop.setProperty(Props::SPECULAR, 0.4f).setProperty(Props::SHININESS, 5.f);
  for (int i = 0; i < number_per_axis; ++i) {
    for (int j = 0; j < number_per_axis; ++j) {
      for (int k = 0; k < number_per_axis; ++k) {
        builder->processSceneElement(new Sphere);
        builder->applyTransformation(
            translation(-5.5f + i * 0.7f, -2.f + j * 0.7f, -2.5f + k * 0.7f) *
            scale(0.2f, 0.2f, 0.2f));
        // builder->applyMaterial(make_unique<PerlinTexture>(1.2f),
        //                        MaterialProperties{});
        builder->applyMaterial(
            make_unique<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)), prop);
        builder->addElement();
      }
    }
  }

  builder->enableBVHierarchy(true);
  SceneElementPtr world = builder->getProduct();
  std::cout << "World constructed!" << std::endl;
  std::cout << "Number of childer:" << world->getChildren().size() << std::endl;

  //----------------------------------------------------------------------------
  auto canvas = Canvas(800, 600);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera = make_unique<RayTracingChalengeCamera>(
      canvas.width(), canvas.height(), 1.112f);
  camera->computePixelSize();
  auto from = Point3f(-2.6f, 3.5f, -11.f);
  auto to = Point3f(-2.6f, 1.0f, -0.5f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<PhongModel>();
  chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
  canvas.render(world, move(camera), move(renderer));
  canvas.save();
  chrono::time_point<chrono::steady_clock> end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
