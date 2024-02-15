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
  auto light = PointLight(Point3f(0.f, 7.f, -8.f), Vec3f(1.f, 1.f, 1.f));

  //----------------------Teapot----------------

  WavefrontReader reader("dragon.obj");
  reader.addLightForModel(light);
  auto prop = MaterialProperties{};
  prop.setProperty(Props::SPECULAR, 0.4f).setProperty(Props::SHININESS, 5.f);
  reader.addMaterial(make_unique<ConstantTexture>(Vec3f(0.f, 0.95f, 0.f)),
                     prop);
  reader.parseInput();
  SceneElementPtr world = reader.getStructureBVHierarchy();
  cout << "Done reading file..." << endl;
  cout << "Number of children:" << world->getChildren().size() << endl;

  //----------------------------------------------------------------------------
  auto canvas = Canvas(100, 100);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera =
      make_unique<Camera>(canvas.width(), canvas.height(), 1.112f);
  camera->computePixelSize();
  auto from = Point3f(0.f, 0.f, -1.f);
  auto to = Point3f(0.f, 0.0f, 0.f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<PhongModel>();
  renderer->setBackgroundColor(Vec3f(0.1f, 0.1f, 0.1f));
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
