#include <chrono>
// #include <fstream>
#include <iostream>

#include "application/wavefront_reader.h"
#include "camera/camera.h"
#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/quad.h"
#include "renderers/path_tracer.h"
#include "stochastic/stochastic_method.h"
#include "textures/texture.h"

using namespace std;
using app = AppParameters;

int main() {
  auto light = PointLight(Point3D(0.f, 0.f, 2.f), Vec3D(1.0f, 1.0f, 1.0f));
  auto eps = 0.05f;
  auto white = Vec3D(0.73f, 0.73f, 0.73f);
  auto green = Vec3D(0.12f, 0.45f, 0.15f);
  auto red = Vec3D(0.65f, 0.05f, 0.05f);
  auto diffuse_light = Vec3D(15.f, 15.f, 15.f);

  SceneDirector scene_director;
  PrimitiveBuilder primitive_builder;
  WorldBuilder world_builder;

  /*---------------------------------------------------------------------------
   *				Floor
   * -------------------------------------------------------------------------*/

  DataContainer floor;
  floor.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(278.f, 0.f, -245.f) * scale(279.f, eps, 556.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, floor);

  /*---------------------------------------------------------------------------
   *				Ceil
   * -------------------------------------------------------------------------*/
  DataContainer ceil;
  ceil.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(278.f, 555.f, -245.f) * scale(279.f, eps, 556.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, ceil);

  /*---------------------------------------------------------------------------
   *				Left Wall
   * -------------------------------------------------------------------------*/
  DataContainer left_wall;
  left_wall.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(0.f, 278.f, -245.f) * scale(eps, 279.f, 556.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, green)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, left_wall);

  /*---------------------------------------------------------------------------
   *				Right Wall
   * -------------------------------------------------------------------------*/
  DataContainer right_wall;
  right_wall.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(555.f, 278.f, -245.f) * scale(eps, 279.f, 556.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, red)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, right_wall);

  /*---------------------------------------------------------------------------
   *				Center Wall
   * -------------------------------------------------------------------------*/
  DataContainer center_wall;
  center_wall.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(0.f, 278.f, -245.f) * scale(556.f, 279.f, eps))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, center_wall);

  /*---------------------------------------------------------------------------
   *				Light Wall
   * -------------------------------------------------------------------------*/
  DataContainer light_wall;
  light_wall
      .setProperty(app::PRIMITIVE_TYPE, app::QUAD)
      //.setProperty(app::TRANSFORMATION_MATRIX,
      //             translation(277.f, 554.5f, -455.f) * scale(139.f,
      //             eps, 50.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, diffuse_light)
      .setProperty(app::MATERIAL_TYPE, app::DIFFUSE_LIGHT)
      .setProperty(app::QUAD_ORIGIN, Point3D(213.f, 554.f, -485.f))
      .setProperty(app::QUAD_UAXIS, Vec3D(130.f, 0.f, 0.f))
      .setProperty(app::QUAD_VAXIS, Vec3D(0.f, 0.f, 105.f));
  scene_director.createSceneElement(primitive_builder, light_wall);
  auto diffuse_light_element = scene_director.getCurrentElement();

  /*---------------------------------------------------------------------------
   *				Left Box
   * -------------------------------------------------------------------------*/
  DataContainer left_box;
  left_box.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(195.f, 1.f, -390.f) *
                                                   scale(70.f, 290.f, 60.f) *
                                                   rotationOverY(-0.4f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, left_box);

  /*---------------------------------------------------------------------------
   *				Right Box
   * -------------------------------------------------------------------------*/
  DataContainer right_box;
  right_box.setProperty(app::PRIMITIVE_TYPE, app::CUBE)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(367.f, 1.f, -440.f) *
                                                   scale(60.f, 110.f, 60.f) *
                                                   rotationOverY(0.4f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, right_box);

  /*---------------------------------------------------------------------------
   *				Left Sphere
   * -------------------------------------------------------------------------*/
  DataContainer left_sphere;
  left_sphere.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(200.f, 300.f, -385.f) * scale(60.f, 60.f, 60.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::DIELECTRIC)
      .setProperty(app::REFRACTIVE_INDEX, 1.5f);
  scene_director.createSceneElement(primitive_builder, left_sphere);

  /*---------------------------------------------------------------------------
   *				Right Sphere
   * -------------------------------------------------------------------------*/
  DataContainer right_sphere;
  right_sphere.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::TRANSFORMATION_MATRIX,
                   translation(200.f, 30.f, -500.f) * scale(60.f, 60.f, 60.f))
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::COLOR, white)
      .setProperty(app::MATERIAL_TYPE, app::METAL)
      .setProperty(app::FUZZ, 0.2f);
  scene_director.createSceneElement(primitive_builder, right_sphere);

  //----------------------------------------------------------------------------
  scene_director.createWorld(world_builder, light);
  auto world = scene_director.getSceneProduct();

  auto canvas = Canvas(100, 100);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = make_shared<Camera>(canvas.width(), canvas.height(), 1.54f);
  auto from = Point3D(278.f, 260.f, -830.f);
  auto to = Point3D(278.f, 278.f, 0.f);
  auto up = Vec3D(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  auto samples_per_pixel = 10;
  auto material_depth = 5;
  BaseRendererPtr renderer =
      make_unique<PathTracer>(std::make_unique<BruteForceSampler>(
          camera, samples_per_pixel, material_depth));
  renderer->setBackgroundColor(Vec3D(0.3f, 0.3f, 0.3f));
  renderer->addDiffuseLight(diffuse_light_element.get());
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
