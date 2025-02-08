#pragma once

enum class AppParameters {
  // Primitive types
  CONE,
  CUBE,
  CYLINDER,
  PLANE,
  SPHERE,
  TRIANGLE,
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
  FUZZ               // float [0,1]
};