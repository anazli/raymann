#include <chrono>
// #include <fstream>
#include <iostream>

#include "application/config_parser.h"
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
  auto light =
      PointLight(Point3f(80.f, 50.f, 200.f), Vec3f(1.12f, 1.12f, 1.12f));

  auto file = std::filesystem::path("dragon.obj");
  WavefrontReader reader(file);
  DataContainer dragon_properties;
  dragon_properties.setProperty(App::AMBIENT, 0.1f)
      .setProperty(App::DIFFUSE, 0.9f)
      .setProperty(App::REFLECTION, 0.2f)
      .setProperty(App::TRANSPARENCY, 0.f)
      .setProperty(App::SPECULAR, 0.5f)
      .setProperty(App::SHININESS, 1.2f);

  auto dragon_texture =
      make_unique<PerlinTexture>(0.33f, Vec3f(0.3f, 0.6f, 0.3f));
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
      .setProperty(App::COLOR, Vec3f(0.1f, 0.1f, 0.1f))
      .setProperty(App::REFLECTION, 0.8f)
      .setProperty(App::DIFFUSE, 0.f)
      .setProperty(App::MATERIAL_TYPE, App::STANDARD);
  EntityFactory builder;
  SceneDirector director;
  director.createSceneElement(builder, data);
  world->add(director.getCurrentElement());

  auto start = chrono::steady_clock::now();
  auto canvas = ConfigParser::createFromJSON("config.json");
  canvas.renderParallel(std::move(world));
  canvas.save();

  auto end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "..............................." << endl;
  cout << "Total elapsed time: " << elapsed.count() << " s" << endl;
  cout << "..............................." << endl;

  return 0;
}
