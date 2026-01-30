#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

MATCHER_P2(Point3Near, v, eps, "") {
  return fabs(arg.x() - v.x()) < eps && fabs(arg.y() - v.y()) < eps &&
         fabs(arg.z() - v.z()) < eps;
}

MATCHER_P2(Vec3Near, v, eps, "") {
  return fabs(arg.x() - v.x()) < eps && fabs(arg.y() - v.y()) < eps &&
         fabs(arg.z() - v.z()) < eps;
}