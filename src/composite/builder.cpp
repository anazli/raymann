#include "composite/builder.h"

#include "acceleration/bvh.h"
#include "application/error.h"
#include "builder.h"
#include "composite/world.h"
#include "geometry/cone.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"
#include "geometry/quad.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "materials/dielectric.h"
#include "materials/emissive.h"
#include "materials/isotropic.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/standard.h"
#include "transformations/transformation.h"

void PrimitiveBuilder::reset() {
  m_current_element.reset();
  m_current_material.reset();
  m_current_texture.reset();
}

void PrimitiveBuilder::setData(const DataContainer& data) {
  m_input_data = data;
}

void PrimitiveBuilder::buildPrimitive() {
  auto type =
      m_input_data.getPropertyAs<AppParameters>(AppParameters::PRIMITIVE_TYPE);
  if (!type.has_value()) {
    throw std::runtime_error("Primitive type must be specified!");
  }
  switch (type.value()) {
    case AppParameters::CONE:
      m_current_element = Cone::create();
      break;
    case AppParameters::CUBE:
      m_current_element = Cube::create();
      break;
    case AppParameters::CYLINDER:
      m_current_element = Cylinder::create();
      break;
    case AppParameters::PLANE:
      m_current_element = Plane::create();
      break;
    case AppParameters::SPHERE:
      m_current_element = Sphere::create();
      break;
    case AppParameters::QUAD: {
      auto origin =
          m_input_data.getPropertyAs<Point3D>(AppParameters::QUAD_ORIGIN)
              .value();
      auto u_axis =
          m_input_data.getPropertyAs<Vec3D>(AppParameters::QUAD_UAXIS).value();
      auto v_axis =
          m_input_data.getPropertyAs<Vec3D>(AppParameters::QUAD_VAXIS).value();
      m_current_element = Quad::create(origin, u_axis, v_axis);
      break;
    }
    default:
      APP_ASSERT(false, "Cannot create the specified primitive type!");
  }
}

void PrimitiveBuilder::buildTexture() {
  auto type =
      m_input_data.getPropertyAs<AppParameters>(AppParameters::TEXTURE_TYPE);
  if (!type.has_value()) {
    throw std::runtime_error("Texture type must be specified!");
  }
  switch (type.value()) {
    case AppParameters::CONSTANT_TEXTURE: {
      auto color = m_input_data.getPropertyAs<Vec3D>(AppParameters::COLOR)
                       .value_or(Vec3D());
      m_current_texture = ConstantTexture::create(color);
      break;
    }
    case AppParameters::PERLIN_TEXTURE: {
      auto color = m_input_data.getPropertyAs<Vec3D>(AppParameters::COLOR)
                       .value_or(Vec3D());
      auto scale =
          m_input_data.getPropertyAs<float>(AppParameters::PERLIN_SCALE)
              .value_or(0.5f);
      m_current_texture = PerlinTexture::create(scale, color);
      break;
    }
    default:
      APP_ASSERT(false, "Cannot create the specified texture type!");
  }
}

void PrimitiveBuilder::buildMaterial() {
  const auto type =
      m_input_data.getPropertyAs<AppParameters>(AppParameters::MATERIAL_TYPE);
  if (!type.has_value()) {
    throw std::runtime_error("Material type must be specified!");
  }
  switch (type.value()) {
    case AppParameters::DIELECTRIC: {
      auto refractive_index =
          m_input_data.getPropertyAs<float>(AppParameters::REFRACTIVE_INDEX)
              .value_or(1);
      m_current_material =
          Dielectric::create(std::move(m_current_texture), refractive_index);
      break;
    }
    case AppParameters::LAMBERTIAN:
      m_current_material = Lambertian::create(std::move(m_current_texture));
      break;
    case AppParameters::METAL: {
      auto fuzz =
          m_input_data.getPropertyAs<float>(AppParameters::FUZZ).value_or(0.2);
      m_current_material = Metal::create(std::move(m_current_texture), fuzz);
      break;
    }
    case AppParameters::DIFFUSE_LIGHT:
      m_current_material =
          EmissiveMaterial::create(std::move(m_current_texture));
      break;
    case AppParameters::ISOTROPIC:
      m_current_material = Isotropic::create(std::move(m_current_texture));
      break;
    case AppParameters::STANDARD:
      m_current_material =
          StandardMaterial::create(std::move(m_current_texture), m_input_data);
      break;
    default:
      APP_ASSERT(false, "Cannot create the specified material type!");
  }
}

void PrimitiveBuilder::buildTransformation() {
  if (!m_input_data.hasProperty(AppParameters::TRANSFORMATION_MATRIX)) {
    m_transformation = Transformation();
  } else {
    m_transformation =
        m_input_data.getPropertyAs<Mat4D>(AppParameters::TRANSFORMATION_MATRIX)
            .value();
  }
}

SceneElementPtr PrimitiveBuilder::getProduct() const {
  m_current_element->setMaterial(m_current_material);
  m_current_element->setTransformation(m_transformation);
  return m_current_element;
}

void WorldBuilder::reset() { m_world.reset(); }

void WorldBuilder::createWorld() { m_world = World::create(); }

void WorldBuilder::addLight(const PointLight& light) {
  m_world->setLight(light);
}

void WorldBuilder::addElement(SceneElementPtr element) {
  APP_ASSERT(m_world, "World not created yet!");
  m_world->add(element);
}

void WorldBuilder::addWorld(SceneElementPtr world) { m_world->add(world); }

SceneElementPtr WorldBuilder::getProduct() const { return m_world; }

SceneElementPtr WorldBuilder::getProductWithBVHierarchy() {
  BVHierarchy bvh;
  bvh.divideWorld(m_world);
  return m_world;
}

void SceneDirector::createSceneElement(PrimitiveBuilder& builder,
                                       const DataContainer& data) {
  builder.setData(data);
  builder.buildPrimitive();
  builder.buildTexture();
  builder.buildMaterial();
  builder.buildTransformation();
  m_element_container.emplace_back(builder.getProduct());
  m_current_element = builder.getProduct();
  builder.reset();
}

void SceneDirector::createWorld(WorldBuilder& builder,
                                const PointLight& light) {
  builder.createWorld();
  builder.addLight(light);
  for (auto element : m_element_container) {
    builder.addElement(element);
  }
  m_scene_product = builder.getProductWithBVHierarchy();
}

SceneElementPtr SceneDirector::getCurrentElement() const {
  return m_current_element;
}

SceneElementPtr SceneDirector::getSceneProduct() const {
  return m_scene_product;
}
