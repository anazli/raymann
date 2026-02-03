#include <chrono>
// #include <fstream>
#include <iostream>

#include "application/wavefront_reader.h"
#include "camera/camera.h"
#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/quad.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/standard.h"
#include "renderers/path_tracer.h"
#include "renderers/phong_model.h"
#include "stochastic/stochastic_method.h"
#include "textures/texture.h"

using namespace std;
using app = App;

int main() {
  auto light = PointLight(Point3f(80.f, 70.f, 200.f), Vec3f(1.f, 1.f, 1.f));

  auto file = std::filesystem::path("dragon.obj");
  WavefrontReader reader(file);
  DataContainer dragon_properties;
  dragon_properties.setProperty(App::AMBIENT, 0.1f)
      .setProperty(App::DIFFUSE, 0.9f)
      .setProperty(App::REFLECTION, 0.f)
      .setProperty(App::TRANSPARENCY, 0.f)
      .setProperty(App::SPECULAR, 0.5f)
      .setProperty(App::SHININESS, 50.f);

  auto dragon_texture =
      make_unique<PerlinTexture>(0.17f, Vec3f(0.3f, 0.55f, 0.3f));
  auto dragon_material =
      StandardMaterial::create(std::move(dragon_texture), dragon_properties);

  reader.addMaterial(dragon_material);
  reader.addLightForModel(light);
  reader.parseInput();
  SceneElementPtr world = reader.getStructureBVHierarchy();

  DataContainer data;
  data.setProperty(App::PRIMITIVE_TYPE, App::PLANE)
      .setProperty(App::TRANSFORMATION_MATRIX,
                   translation(0.f, -40.f, 0.f) * rotationOverY(PI))
      .setProperty(App::TEXTURE_TYPE, App::CONSTANT_TEXTURE)
      .setProperty(App::COLOR, Vec3f(0.25f, 0.25f, 0.25f))
      .setProperty(App::MATERIAL_TYPE, App::STANDARD);
  EntityFactory builder;
  SceneDirector director;
  director.createSceneElement(builder, data);
  world->add(director.getCurrentElement());

  auto canvas = Canvas(800, 600);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = Camera(canvas.width(), canvas.height(), 1.02f);
  auto from = Point3f(240.f, 45.f, 120.f);
  auto to = Point3f(-90.f, -5.f, -30.f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera.setTransform(view_transform(from, to, up));

  int samples_per_pixel = 5;
  int material_depth = 2;
  auto renderer = std::make_unique<PhongModel>();
  renderer->setBackgroundColor(Vec3f(0.3f, 0.5f, 0.3f));
  auto start = chrono::steady_clock::now();
  canvas.renderParallel(world, camera, std::move(renderer));
  canvas.save();

  auto end = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::minutes>(end - start);
  cout << "..............................." << endl;
  cout << "Total elapsed time:" << elapsed.count() << " min." << endl;
  cout << "..............................." << endl;

  return 0;
}
