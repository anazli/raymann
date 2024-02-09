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

  //----------------------Perlin Spheres----------------
  int number_per_axis = 10;
  float distance = 0.9f;
  int c = 0;
  for (int i = 0; i < number_per_axis; ++i) {
    for (int j = 0; j < number_per_axis; ++j) {
      for (int k = 0; k < number_per_axis; ++k) {
        builder->processSceneElement(new Sphere);
        builder->applyTransformation(translation(-6.5f + i * distance,
                                                 -2.f + j * distance,
                                                 -2.f + k * distance) *
                                     scale(0.35f, 0.35f, 0.35f));
        builder->applyMaterial(make_unique<PerlinTexture>(0.002f * c++),
                               MaterialProperties{});
        builder->addElement();
      }
    }
  }

  SceneElementPtr world = builder->getProductBVHierarchy();

  //----------------------------------------------------------------------------
  auto canvas = Canvas(800, 600);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera = make_unique<RayTracingChalengeCamera>(
      canvas.width(), canvas.height(), 1.112f);
  camera->computePixelSize();
  auto from = Point3f(-2.6f, 4.5f, -11.f);
  auto to = Point3f(-2.6f, 1.0f, -0.5f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<PhongModel>();
  renderer->setBackgroundColor(Vec3f(0.1f, 0.1f, 0.1f));
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
