//#include <chrono>
//#include <fstream>
//#include <iostream>

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
  auto light = PointLight(Point3f(-4.9f, 4.9f, -1.f), Vec3f(1.0f, 1.0f, 1.0f));
  auto world = make_unique<WorldBuilder>();
  world->createWorld(light);
  //----------------------Floor---------------------
  auto prop_floor = MaterialProperties{};
  prop_floor.setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.4f);
  world->processSceneElement(new Plane);
  world->applyTransformation(rotationOverY(0.31415f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.65f, 0.65f, 0.65f)),
                       prop_floor);
  world->addElement();

  //----------------------Ceiling---------------------
  auto prop_ceil = MaterialProperties{};
  prop_ceil.setProperty(Props::AMBIENT, 0.3f).setProperty(Props::SPECULAR, 0.f);
  world->processSceneElement(new Plane);
  world->applyTransformation(translation(0.f, 5.f, 0.f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.8f, 0.8f, 0.8f)),
                       prop_ceil);
  world->addElement();

  //----------------------West Wall---------------------
  auto prop_wall = MaterialProperties{};
  prop_wall.setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.3f);
  world->processSceneElement(new Plane);
  world->applyTransformation(translation(-5.f, 0.f, 0.f) *
                             rotationOverZ(1.5708f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_wall);
  world->addElement();

  //----------------------East Wall---------------------
  world->processSceneElement(new Plane);
  world->applyTransformation(translation(5.f, 0.f, 0.f) *
                             rotationOverZ(1.5708f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_wall);
  world->addElement();

  //----------------------North Wall---------------------
  world->processSceneElement(new Plane);
  world->applyTransformation(translation(0.f, 0.f, 5.f) *
                             rotationOverX(-1.5708f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_wall);
  world->addElement();

  //----------------------South Wall---------------------
  world->processSceneElement(new Plane);
  world->applyTransformation(translation(0.f, 0.f, -5.f) *
                             rotationOverX(1.5708f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_wall);
  world->addElement();

  //----------------------Background Spheres----------------
  auto prop_back = MaterialProperties{};
  prop_back.setProperty(Props::SHININESS, 50.f);
  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(4.6f, 0.4f, 1.f) *
                             scale(0.4f, 0.4f, 0.4f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.8f, 0.5f, 0.3f)),
                       prop_back);
  world->addElement();

  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(4.7f, 0.3f, 0.4f) *
                             scale(0.3f, 0.3f, 0.3f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.9f, 0.4f, 0.5f)),
                       prop_back);
  world->addElement();

  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(-1.f, 0.5f, 4.5f) *
                             scale(0.5f, 0.5f, 0.5f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.4f, 0.9f, 0.6f)),
                       prop_back);
  world->addElement();

  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(-1.7f, 0.3f, 4.7f) *
                             scale(0.3f, 0.3f, 0.3f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.4f, 0.6f, 0.9f)),
                       prop_back);
  world->addElement();

  //----------------------Red Sphere----------------
  auto prop_red = MaterialProperties{};
  prop_red.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::SHININESS, 5.f);
  prop_red.setProperty(Props::SHININESS, 50.f);
  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(-0.6f, 1.f, 0.6f));
  world->applyMaterial(make_unique<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)),
                       prop_red);
  world->addElement();

  //----------------------Perlin Sphere1----------------
  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(0.6f, 0.7f, -0.6f) *
                             scale(0.7f, 0.7f, 0.7f));
  world->applyMaterial(make_unique<PerlinTexture>(1.2f), MaterialProperties{});
  world->addElement();

  //----------------------Perlin Sphere2----------------
  world->processSceneElement(new Sphere);
  world->applyTransformation(translation(-0.7f, 0.5f, -0.8f) *
                             scale(0.5f, 0.5f, 0.5f));
  world->applyMaterial(make_unique<PerlinTexture>(0.6f), MaterialProperties{});
  world->addElement();

  //----------------------Perlin Box----------------
  world->processSceneElement(new Cube);
  world->applyTransformation(translation(-2.5f, 0.5f, 0.8f) *
                             scale(0.5f, 0.5f, 0.5f));
  world->applyMaterial(make_unique<PerlinTexture>(1.1f), MaterialProperties{});
  world->addElement();

  //----------------------Perlin Cylinder----------------
  world->processSceneElement(new Cylinder(0, 2, true));
  world->applyTransformation(translation(1.4f, 0.f, 1.6f) *
                             scale(0.7f, 1.f, 1.f));
  world->applyMaterial(make_unique<PerlinTexture>(1.2f), MaterialProperties{});
  world->addElement();

  //----------------------------------------------------------------------------
  auto canvas = Canvas(800, 600);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera = make_unique<RayTracingChalengeCamera>(
      canvas.width(), canvas.height(), 1.152f);
  camera->computePixelSize();
  auto from = Point3f(-2.6f, 3.5f, -4.9f);
  auto to = Point3f(-0.6f, 1.0f, -0.8f);
  auto up = Vec3f(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_unique<PhongModel>();
  canvas.render(world->getProduct(), move(camera), move(renderer));
  canvas.save();
  /*chrono::time_point<chrono::steady_clock> start =
  chrono::steady_clock::now(); chrono::time_point<chrono::steady_clock> end =
  chrono::steady_clock::now(); chrono::duration<double> elapsed = (end - start)
  / (60.); cout << "...................." << endl; cout << "Elapsed time:" <<
  elapsed.count() << " min." << endl; cout << "...................." << endl;*/

  return 0;
}
