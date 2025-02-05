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
#include "geometry/quad.h"
#include "geometry/sphere.h"
#include "renderers/path_tracer.h"
#include "renderers/phong_model.h"
#include "stochastic/stochastic_method.h"
#include "textures/texture.h"

using namespace std;

int main() {
  auto light = PointLight(Point3D(0.f, 0.f, 2.f), Vec3D(1.0f, 1.0f, 1.0f));
  auto builder = make_unique<WorldBuilder>();
  builder->createWorld();
  auto eps = 0.05f;
  auto white = Vec3D(0.73f, 0.73f, 0.73f);
  auto green = Vec3D(0.12f, 0.45f, 0.15f);
  auto red = Vec3D(0.65f, 0.05f, 0.05f);
  auto diffuseLight = Vec3D(15.f, 15.f, 15.f);

  /*---------------------------------------------------------------------------
   *				Floor
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(278.f, 0.f, -245.f) *
                               scale(279.f, eps, 556.f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Ceil
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(278.f, 555.f, -245.f) *
                               scale(279.f, eps, 556.f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Left Wall
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(0.f, 278.f, -245.f) *
                               scale(eps, 279.f, 556.f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(green));
  builder->addElement();
  /*---------------------------------------------------------------------------
   *				Right Wall
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(555.f, 278.f, -245.f) *
                               scale(eps, 279.f, 556.f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(red));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Center Wall
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(0.f, 278.f, -245.f) *
                               scale(556.f, 279.f, eps));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Light Wall
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Quad(Point3D(213.f, 554.f, -485.f),
                                        Vec3D(130.f, 0.f, 0.f),
                                        Vec3D(0.f, 0.f, 105.f)));
  // builder->applyTransformation(translation(277.f, 554.5f, -455.f) *
  //                              scale(80.f, eps, 30.f));
  builder->applyEmissiveMaterial(make_unique<ConstantTexture>(diffuseLight));
  SceneElementRawPtr diffuseLightElem = builder->getCurrentElement();
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Left Box
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(195.f, 1.f, -390.f) *
                               scale(70.f, 290.f, 60.f) * rotationOverY(-0.4f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Right Box
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Cube);
  builder->applyTransformation(translation(367.f, 1.f, -440.f) *
                               scale(60.f, 110.f, 60.f) * rotationOverY(0.4f));
  builder->applyLambertianMaterial(make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Left Sphere
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(200.f, 320.f, -395.f) *
                               scale(30.f, 30.f, 30.f));
  builder->applyDielectricMaterial(1.8f, make_unique<ConstantTexture>(white));
  builder->addElement();

  /*---------------------------------------------------------------------------
   *				Right Sphere
   * -------------------------------------------------------------------------*/
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(367.f, 158.f, -440.f) *
                               scale(60.f, 60.f, 60.f));
  builder->applyDielectricMaterial(1.5f, make_unique<ConstantTexture>(white));
  builder->addElement();

  //----------------------------------------------------------------------------
  SceneElementPtr world = builder->getProductBVHierarchy();

  auto canvas = Canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = make_shared<Camera>(canvas.width(), canvas.height(), 1.34f);
  auto from = Point3D(278.f, 260.f, -860.f);
  auto to = Point3D(278.f, 278.f, 0.f);
  auto up = Vec3D(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  int samplesPerPixel = 500;
  int materialDepth = 50;
  BaseRendererPtr renderer =
      make_unique<PathTracer>(std::make_unique<StratifiedSampler>(
          camera, samplesPerPixel, materialDepth));
  renderer->setBackgroundColor(Vec3D(0.3f, 0.3f, 0.3f));
  renderer->addDiffuseLight(diffuseLightElem);
  chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
  canvas.renderParallel(world, camera, std::move(renderer));
  canvas.save();

  chrono::time_point<chrono::steady_clock> end = chrono::steady_clock::now();
  chrono::duration<double> elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
