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
  auto builder = make_unique<WorldBuilder>();
  builder->createWorld(light);
  //----------------------Floor---------------------
  auto prop_floor = MaterialProperties{};
  prop_floor.setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.4f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(rotationOverY(0.31415f));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.65f, 0.65f, 0.65f)), prop_floor);
  builder->addElement();

  //----------------------Ceiling---------------------
  auto prop_ceil = MaterialProperties{};
  prop_ceil.setProperty(Props::AMBIENT, 0.3f).setProperty(Props::SPECULAR, 0.f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, 5.f, 0.f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.8f, 0.8f, 0.8f)),
                         prop_ceil);
  builder->addElement();

  //----------------------West Wall---------------------
  auto prop_wall = MaterialProperties{};
  prop_wall.setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.3f);
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(-5.f, 0.f, 0.f) *
                               rotationOverZ(1.5708f));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)), prop_wall);
  builder->addElement();

  //----------------------East Wall---------------------
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(5.f, 0.f, 0.f) *
                               rotationOverZ(1.5708f));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)), prop_wall);
  builder->addElement();

  //----------------------North Wall---------------------
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, 0.f, 5.f) *
                               rotationOverX(-1.5708f));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)), prop_wall);
  builder->addElement();

  //----------------------South Wall---------------------
  builder->processSceneElement(new Plane);
  builder->applyTransformation(translation(0.f, 0.f, -5.f) *
                               rotationOverX(1.5708f));
  builder->applyMaterial(
      make_unique<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)), prop_wall);
  builder->addElement();

  auto world0 = builder->getProduct();

  //----------------------Background Spheres----------------
  builder->createWorld(light);

  auto prop_back = MaterialProperties{};
  prop_back.setProperty(Props::SHININESS, 50.f);
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(4.6f, 0.4f, 1.f) *
                               scale(0.4f, 0.4f, 0.4f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.8f, 0.5f, 0.3f)),
                         prop_back);
  builder->addElement();

  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(4.7f, 0.3f, 0.4f) *
                               scale(0.3f, 0.3f, 0.3f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.9f, 0.4f, 0.5f)),
                         prop_back);
  builder->addElement();

  auto world1 = builder->getProduct();

  builder->createWorld(light);

  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-1.f, 0.5f, 4.5f) *
                               scale(0.5f, 0.5f, 0.5f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.4f, 0.9f, 0.6f)),
                         prop_back);
  builder->addElement();

  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-1.7f, 0.3f, 4.7f) *
                               scale(0.3f, 0.3f, 0.3f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(0.4f, 0.6f, 0.9f)),
                         prop_back);
  builder->addElement();

  auto world2 = builder->getProduct();

  //----------------------Red Sphere----------------
  builder->createWorld(light);
  auto prop_red = MaterialProperties{};
  prop_red.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::SHININESS, 5.f);
  prop_red.setProperty(Props::SHININESS, 50.f);
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.6f, 1.f, 0.6f));
  builder->applyMaterial(make_unique<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)),
                         prop_red);
  builder->addElement();

  //----------------------Perlin Sphere1----------------
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(0.6f, 0.7f, -0.6f) *
                               scale(0.7f, 0.7f, 0.7f));
  builder->applyMaterial(make_unique<PerlinTexture>(1.2f),
                         MaterialProperties{});
  builder->addElement();

  //----------------------Perlin Sphere2----------------
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(-0.7f, 0.5f, -0.8f) *
                               scale(0.5f, 0.5f, 0.5f));
  builder->applyMaterial(make_unique<PerlinTexture>(0.6f),
                         MaterialProperties{});
  builder->addElement();

  //----------------------Perlin Box----------------
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(-2.5f, 0.5f, 0.8f) *
                               scale(0.5f, 0.5f, 0.5f));
  builder->applyMaterial(make_unique<PerlinTexture>(1.1f),
                         MaterialProperties{});
  builder->addElement();

  //----------------------Perlin Cylinder----------------
  builder->processSceneElement(new Cylinder(0, 2, true));
  builder->applyTransformation(translation(1.4f, 0.f, 1.6f) *
                               scale(0.7f, 1.f, 1.f));
  builder->applyMaterial(make_unique<PerlinTexture>(1.2f),
                         MaterialProperties{});
  builder->addElement();
  auto world3 = builder->getProduct();

  builder->createWorld(light);
  auto finalWorld = builder->getProduct();
  finalWorld->add(world0);
  finalWorld->add(world1);
  finalWorld->add(world2);
  finalWorld->add(world3);

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
  canvas.render(finalWorld, move(camera), move(renderer));
  canvas.save();
  /*chrono::time_point<chrono::steady_clock> start =
  chrono::steady_clock::now(); chrono::time_point<chrono::steady_clock> end =
  chrono::steady_clock::now(); chrono::duration<double> elapsed = (end - start)
  / (60.); cout << "...................." << endl; cout << "Elapsed time:" <<
  elapsed.count() << " min." << endl; cout << "...................." << endl;*/

  return 0;
}
