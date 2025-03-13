#include "composite/builder.h"

#include "acceleration/bvh.h"
#include "application/error.h"
#include "builder.h"
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

void EntityFactory::setData(const DataContainer& data) { m_input_data = data; }

PrimitivePtr EntityFactory::createPrimitive() {
  Transformation transformation;
  if (m_input_data.hasProperty(AppParameters::TRANSFORMATION_MATRIX)) {
    transformation =
        m_input_data.getPropertyAs<Mat4D>(AppParameters::TRANSFORMATION_MATRIX)
            .value();
  }
  auto type =
      m_input_data.getPropertyAs<AppParameters>(AppParameters::PRIMITIVE_TYPE);
  if (!type.has_value()) {
    throw std::runtime_error("Primitive type must be specified!");
  }
  switch (type.value()) {
    case AppParameters::CONE:
      return Cone::create(transformation);
      break;
    case AppParameters::CUBE:
      return Cube::create(transformation);
      break;
    case AppParameters::CYLINDER: {
      auto ymin =
          m_input_data.getPropertyAs<float>(AppParameters::CYLINDER_Y_MIN)
              .value();
      auto ymax =
          m_input_data.getPropertyAs<float>(AppParameters::CYLINDER_Y_MAX)
              .value();
      auto closed =
          m_input_data.getPropertyAs<bool>(AppParameters::CYLINDER_CLOSED)
              .value();
      return Cylinder::create(ymin, ymax, closed, transformation);
      break;
    }
    case AppParameters::PLANE:
      return Plane::create(transformation);
      break;
    case AppParameters::SPHERE:
      return Sphere::create(transformation);
      break;
    case AppParameters::QUAD: {
      auto origin =
          m_input_data.getPropertyAs<Point3D>(AppParameters::QUAD_ORIGIN)
              .value();
      auto u_axis =
          m_input_data.getPropertyAs<Vec3D>(AppParameters::QUAD_UAXIS).value();
      auto v_axis =
          m_input_data.getPropertyAs<Vec3D>(AppParameters::QUAD_VAXIS).value();
      return Quad::create(origin, u_axis, v_axis, transformation);
      break;
    }
    default:
      APP_ASSERT(false, "Cannot create the specified primitive type!");
  }
  return nullptr;
}

TexturePtr EntityFactory::createTexture() {
  auto type =
      m_input_data.getPropertyAs<AppParameters>(AppParameters::TEXTURE_TYPE);
  if (!type.has_value()) {
    throw std::runtime_error("Texture type must be specified!");
  }
  switch (type.value()) {
    case AppParameters::CONSTANT_TEXTURE: {
      auto color = m_input_data.getPropertyAs<Vec3D>(AppParameters::COLOR)
                       .value_or(Vec3D());
      return ConstantTexture::create(color);
      break;
    }
    default:
      APP_ASSERT(false, "Cannot create the specified texture type!");
  }
  return nullptr;
}

MaterialPtr EntityFactory::createMaterial() {
  auto texture = createTexture();
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
      return Dielectric::create(std::move(texture), refractive_index);
      break;
    }
    case AppParameters::LAMBERTIAN:
      return Lambertian::create(std::move(texture));
      break;
    case AppParameters::METAL: {
      auto fuzz =
          m_input_data.getPropertyAs<float>(AppParameters::FUZZ).value_or(0.2);
      return Metal::create(std::move(texture), fuzz);
      break;
    }
    case AppParameters::DIFFUSE_LIGHT:
      return EmissiveMaterial::create(std::move(texture));
      break;
    case AppParameters::ISOTROPIC:
      return Isotropic::create(std::move(texture));
      break;
    case AppParameters::STANDARD:
      return StandardMaterial::create(std::move(texture), m_input_data);
      break;
    default:
      APP_ASSERT(false, "Cannot create the specified material type!");
  }
  return nullptr;
}

void WorldBuilder::reset() { m_world.reset(); }

void WorldBuilder::createWorld() { m_world = SceneElementNode::create(); }

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

void SceneDirector::createSceneElement(EntityFactory& factory,
                                       const DataContainer& data) {
  m_current_scene_element = std::make_shared<SceneElementNode>();
  factory.setData(data);
  auto primitive = factory.createPrimitive();
  auto material = factory.createMaterial();
  m_current_scene_element->setMaterial(material);
  m_current_scene_element->setPrimitive(primitive);
  m_element_container.emplace_back(m_current_scene_element);
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
  return m_current_scene_element;
}

SceneElementPtr SceneDirector::getSceneProduct() const {
  return m_scene_product;
}
