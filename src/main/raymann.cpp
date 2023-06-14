#include <chrono>
#include <fstream>
#include <iostream>

#include "camera/camera.h"
#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "renderers/renderer.h"
#include "textures/texture.h"
#include "tools/tools.h"

using namespace std;

int main() {
  PointLight light(Point3f(-4.9f, 4.9f, -1.f), Vec3f(1.0f, 1.0f, 1.0f));
  BuilderPtr world = make_shared<WorldBuilder>();
  world->createWorld(light);
  //----------------------Floor---------------------
  MaterialProperties prop_floor;
  prop_floor.setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.4f);
  world->createPlane();
  world->applyTransformation(rotY(0.31415f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.65f, 0.65f, 0.65f)),
                       prop_floor);
  world->addElement();

  //----------------------Ceiling---------------------
  MaterialProperties prop_ceil;
  prop_ceil.setProperty(Props::AMBIENT, 0.3f).setProperty(Props::SPECULAR, 0.f);
  world->createPlane();
  world->applyTransformation(transl(0.f, 5.f, 0.f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.8f, 0.8f, 0.8f)),
                       prop_ceil);
  world->addElement();

  //----------------------West Wall---------------------
  MaterialProperties prop_west;
  prop_west.setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::REFLECTION, 0.3f);
  world->createPlane();
  world->applyTransformation(transl(-5.f, 0.f, 0.f) * rotZ(1.5708f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_west);
  world->addElement();

  //----------------------East Wall---------------------
  world->createPlane();
  world->applyTransformation(transl(5.f, 0.f, 0.f) * rotZ(1.5708f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_west);
  world->addElement();

  //----------------------North Wall---------------------
  world->createPlane();
  world->applyTransformation(transl(0.f, 0.f, 5.f) * rotX(-1.5708f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_west);
  world->addElement();

  //----------------------South Wall---------------------
  world->createPlane();
  world->applyTransformation(transl(0.f, 0.f, -5.f) * rotX(1.5708f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.55f, 0.55f, 0.55f)),
                       prop_west);
  world->addElement();

  //----------------------Background Spheres----------------
  MaterialProperties prop_back;
  prop_back.setProperty(Props::SHININESS, 50.f);
  world->createSphere();
  world->applyTransformation(transl(4.6f, 0.4f, 1.f) * scale(0.4f, 0.4f, 0.4f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.8f, 0.5f, 0.3f)),
                       prop_back);
  world->addElement();

  world->createSphere();
  world->applyTransformation(transl(4.7f, 0.3f, 0.4f) *
                             scale(0.3f, 0.3f, 0.3f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.9f, 0.4f, 0.5f)),
                       prop_back);
  world->addElement();

  world->createSphere();
  world->applyTransformation(transl(-1.f, 0.5f, 4.5f) *
                             scale(0.5f, 0.5f, 0.5f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.4f, 0.9f, 0.6f)),
                       prop_back);
  world->addElement();

  world->createSphere();
  world->applyTransformation(transl(-1.7f, 0.3f, 4.7f) *
                             scale(0.3f, 0.3f, 0.3f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.4f, 0.6f, 0.9f)),
                       prop_back);
  world->addElement();

  //----------------------Background Spheres----------------
  //----------------------Red Sphere----------------
  MaterialProperties prop_red;
  prop_red.setProperty(Props::SPECULAR, 0.4f)
      .setProperty(Props::SHININESS, 5.f);
  prop_red.setProperty(Props::SHININESS, 50.f);
  world->createSphere();
  world->applyTransformation(transl(-0.6f, 1.f, 0.6f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(1.0f, 0.3f, 0.2f)),
                       prop_red);
  world->addElement();

  //----------------------Blue glass Sphere----------------
  MaterialProperties prop_blue;
  prop_blue.setProperty(Props::SHININESS, 300.f)
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::REFLECTION, 0.9f)
      .setProperty(Props::TRANSPARENCY, 0.9f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.5f);
  world->createSphere();
  world->applyTransformation(transl(0.6f, 0.7f, -0.6f) *
                             scale(0.7f, 0.7f, 0.7f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.f, 0.f, 0.2f)),
                       prop_blue);
  world->addElement();

  //----------------------Green glass Sphere----------------
  MaterialProperties prop_green;
  prop_green.setProperty(Props::SHININESS, 300.f)
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.4f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::REFLECTION, 0.9f)
      .setProperty(Props::TRANSPARENCY, 0.9f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.5f);
  world->createSphere();
  world->applyTransformation(transl(-0.7f, 0.5f, -0.8f) *
                             scale(0.5f, 0.5f, 0.5f));
  world->applyMaterial(make_shared<ConstantTexture>(Vec3f(0.f, 0.2f, 0.f)),
                       prop_green);
  world->addElement();

  //----------------------------------------------------------------------------
  Canvas canvas(300, 300);
  canvas.setFileName("scenes/scene.ppm");
  BaseCameraPtr camera = make_shared<RayTracingChalengeCamera>(
      canvas.width(), canvas.height(), 1.152f);
  camera->computePixelSize();
  Point3f from(-2.6f, 1.5f, -3.9f);
  Point3f to(-0.6f, 1.0f, -0.8f);
  Vec3f up(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  BaseRendererPtr renderer = make_shared<PhongModel>();
  canvas.render(world->getProduct(), *camera, *renderer);
  canvas.save();
  /*chrono::time_point<chrono::steady_clock> start =
  chrono::steady_clock::now(); chrono::time_point<chrono::steady_clock> end =
  chrono::steady_clock::now(); chrono::duration<double> elapsed = (end - start)
  / (60.); cout << "...................." << endl; cout << "Elapsed time:" <<
  elapsed.count() << " min." << endl; cout << "...................." << endl;*/

  return 0;
}
