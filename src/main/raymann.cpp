#include <chrono>
//#include <fstream>
#include <iostream>

#include "application/wavefront_reader.h"
#include "camera/camera.h"
#include "composite/builder.h"
#include "container/canvas.h"
#include "geometry/quad.h"
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
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(278.f, 0.f, -245.f) *
                               scale(279.f, eps, 556.f));
  builder->applyLambertianMaterial(ConstantTexture::create(white));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Ceil
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(278.f, 555.f, -245.f) *
                               scale(279.f, eps, 556.f));
  builder->applyLambertianMaterial(ConstantTexture::create(white));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Left Wall
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(0.f, 278.f, -245.f) *
                               scale(eps, 279.f, 556.f));
  builder->applyLambertianMaterial(ConstantTexture::create(green));
  builder->addElementToProduct();
  /*---------------------------------------------------------------------------
   *				Right Wall
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(555.f, 278.f, -245.f) *
                               scale(eps, 279.f, 556.f));
  builder->applyLambertianMaterial(ConstantTexture::create(red));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Center Wall
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(0.f, 278.f, -245.f) *
                               scale(556.f, 279.f, eps));
  builder->applyLambertianMaterial(ConstantTexture::create(white));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Light Wall
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(new Quad(Point3D(213.f, 554.f, -485.f),
                                    Vec3D(130.f, 0.f, 0.f),
                                    Vec3D(0.f, 0.f, 105.f)));
  // builder->applyTransformation(translation(277.f, 554.5f, -455.f) *
  //                              scale(80.f, eps, 30.f));
  builder->applyEmissiveMaterial(ConstantTexture::create(diffuseLight));
  auto diffuseLightElem = builder->getCurrentElement();
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Left Box
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(195.f, 1.f, -390.f) *
                               scale(70.f, 290.f, 60.f) * rotationOverY(-0.4f));
  builder->applyLambertianMaterial(ConstantTexture::create(white));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Right Box
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::CUBE);
  builder->applyTransformation(translation(367.f, 1.f, -440.f) *
                               scale(60.f, 110.f, 60.f) * rotationOverY(0.4f));
  builder->applyLambertianMaterial(ConstantTexture::create(white));
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Left Sphere
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::SPHERE);
  builder->applyTransformation(translation(200.f, 320.f, -395.f) *
                               scale(30.f, 30.f, 30.f));
  DataContainer prop;
  prop.setProperty(DataContainer::FUZZ, 1.8f);
  builder->applyDielectricMaterial(ConstantTexture::create(white), prop);
  builder->addElementToProduct();

  /*---------------------------------------------------------------------------
   *				Right Sphere
   * -------------------------------------------------------------------------*/
  builder->createPrimitive(PrimitiveType::SPHERE);
  builder->applyTransformation(translation(367.f, 158.f, -440.f) *
                               scale(60.f, 60.f, 60.f));
  prop.setProperty(DataContainer::REFRACTIVE_INDEX, 1.5f);
  builder->applyDielectricMaterial(ConstantTexture::create(white), prop);
  builder->addElementToProduct();

  //----------------------------------------------------------------------------
  auto world = builder->getProductBVHierarchy();

  auto canvas = Canvas(600, 600);
  canvas.setFileName("scenes/scene.ppm");
  auto camera = make_shared<Camera>(canvas.width(), canvas.height(), 1.34f);
  auto from = Point3D(278.f, 260.f, -860.f);
  auto to = Point3D(278.f, 278.f, 0.f);
  auto up = Vec3D(0.0f, 1.0f, 0.0f);
  camera->setTransform(view_transform(from, to, up));

  auto samplesPerPixel = 50;
  auto materialDepth = 10;
  BaseRendererPtr renderer =
      make_unique<PathTracer>(std::make_unique<BruteForceSampler>(
          camera, samplesPerPixel, materialDepth));
  renderer->setBackgroundColor(Vec3D(0.3f, 0.3f, 0.3f));
  renderer->addDiffuseLight(diffuseLightElem.get());
  auto start = chrono::steady_clock::now();
  canvas.renderParallel(world, camera, std::move(renderer));
  canvas.save();

  auto end = chrono::steady_clock::now();
  auto elapsed = (end - start) / (60.);
  cout << "...................." << endl;
  cout << "Elapsed time:" << elapsed.count() << " min." << endl;
  cout << "...................." << endl;

  return 0;
}
