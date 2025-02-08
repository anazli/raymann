#pragma once

enum class AppParameters {
  // Primitive types
  PRIMITIVE_TYPE,
  CONE,
  CUBE,
  CYLINDER,
  PLANE,
  SPHERE,
  TRIANGLE,
  QUAD,
  WORLD,

  // Primitive parameters
  SPHERE_CENTER,
  SPHERE_RADIUS,
  QUAD_ORIGIN,
  QUAD_UAXIS,
  QUAD_VAXIS,
  TRIANGLE_POINTS,
  SMOOTH_TRIANGLE_POINTS,
  SMOTH_TRIANGLE_VECTORS,

  // Transformation parameters
  TRANSFORMATION_MATRIX,

  // Material types
  MATERIAL_TYPE,
  STANDARD,
  LAMBERTIAN,
  ISOTROPIC,
  DIFFUSE_LIGHT,
  METAL,
  DIELECTRIC,

  // Material parameters
  COLOR,             // Vec3D
  AMBIENT,           // float [0,1]
  DIFFUSE,           // float [0,1]
  SPECULAR,          // float [0,1]
  SHININESS,         // float [0,..]
  REFLECTION,        // float [0,1]
  TRANSPARENCY,      // float [0,1]
  REFRACTIVE_INDEX,  // float [0,..]
  FUZZ,              // float [0,1]

  // Texture types
  TEXTURE_TYPE,
  CONSTANT_TEXTURE,
  CHECKER_TEXTURE,
  PERLIN_TEXTURE,
  PERLIN_SCALE,
};