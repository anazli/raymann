#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>

#include "application/wavefront_reader.h"
#include "composite/iterator.h"

using testing::Eq;

class ApplicationTest : public testing::Test {
 public:
  void SetUp() override {
    test_directory = std::filesystem::current_path().string() + "/test_data/";
  }
  WavefrontReader reader;
  std::string test_directory;
};

TEST_F(ApplicationTest, parseInvalidInput) {
  reader.setFileName(test_directory + "invalid_input.txt");
  reader.parseInput();
  EXPECT_TRUE(reader.vertexCollection().size() == 0);
}

TEST_F(ApplicationTest, parseValidInput) {
  reader.setFileName(test_directory + "valid_input.txt");
  reader.parseInput();
  ASSERT_TRUE(reader.vertexCollection().size() == 4);
  EXPECT_THAT(reader.vertexCollection()[0], Eq(Vec3f(1.f, 2.f, 3.f)));
  EXPECT_THAT(reader.vertexCollection()[1], Eq(Vec3f(-1.f, -2.f, -3.f)));
  EXPECT_THAT(reader.vertexCollection()[2], Eq(Vec3f(1.5f, 4.5f, -0.3f)));
  EXPECT_THAT(reader.vertexCollection()[3], Eq(Vec3f(-4.05f, 1.053f, 0.0005f)));

  ASSERT_TRUE(reader.triangleCollection().size() == 2);
  EXPECT_THAT(reader.triangleCollection()[0].point(0),
              Eq(Point3f(reader.vertexCollection()[0])));
  EXPECT_THAT(reader.triangleCollection()[0].point(1),
              Eq(Point3f(reader.vertexCollection()[1])));
  EXPECT_THAT(reader.triangleCollection()[0].point(2),
              Eq(Point3f(reader.vertexCollection()[2])));

  EXPECT_THAT(reader.triangleCollection()[1].point(0),
              Eq(Point3f(reader.vertexCollection()[0])));
  EXPECT_THAT(reader.triangleCollection()[1].point(1),
              Eq(Point3f(reader.vertexCollection()[2])));
  EXPECT_THAT(reader.triangleCollection()[1].point(2),
              Eq(Point3f(reader.vertexCollection()[3])));
}

TEST_F(ApplicationTest, parseValidInputWithVertexNormals) {
  reader.setFileName(test_directory + "valid_input_vn.txt");
  reader.parseInput();
  EXPECT_TRUE(reader.vertexNormalCollection().size() == 3);
  EXPECT_THAT(reader.vertexNormalCollection()[0], Eq(Normal3f(0.f, 0.f, 1.f)));
  EXPECT_THAT(reader.vertexNormalCollection()[1],
              Eq(Normal3f(0.707f, 0.f, -0.707f)));
  EXPECT_THAT(reader.vertexNormalCollection()[2], Eq(Normal3f(1.f, 2.f, 3.f)));
}

TEST_F(ApplicationTest, parseValidPolygonInput) {
  reader.setFileName(test_directory + "valid_input_polygon.txt");
  reader.parseInput();
  ASSERT_TRUE(reader.vertexCollection().size() == 5);

  ASSERT_TRUE(reader.triangleCollection().size() == 3);
  EXPECT_THAT(reader.triangleCollection()[0].point(0),
              Eq(Point3f(reader.vertexCollection()[0])));
  EXPECT_THAT(reader.triangleCollection()[0].point(1),
              Eq(Point3f(reader.vertexCollection()[1])));
  EXPECT_THAT(reader.triangleCollection()[0].point(2),
              Eq(Point3f(reader.vertexCollection()[2])));

  EXPECT_THAT(reader.triangleCollection()[1].point(0),
              Eq(Point3f(reader.vertexCollection()[0])));
  EXPECT_THAT(reader.triangleCollection()[1].point(1),
              Eq(Point3f(reader.vertexCollection()[2])));
  EXPECT_THAT(reader.triangleCollection()[1].point(2),
              Eq(Point3f(reader.vertexCollection()[3])));

  EXPECT_THAT(reader.triangleCollection()[2].point(0),
              Eq(Point3f(reader.vertexCollection()[0])));
  EXPECT_THAT(reader.triangleCollection()[2].point(1),
              Eq(Point3f(reader.vertexCollection()[3])));
  EXPECT_THAT(reader.triangleCollection()[2].point(2),
              Eq(Point3f(reader.vertexCollection()[4])));
}

TEST_F(ApplicationTest, parseFileWithGroups) {
  reader.setFileName(test_directory + "valid_input_groups.txt");
  reader.parseInput();
  SceneElementPtr world = reader.getStructure();
  ASSERT_TRUE(world->getChildren().size() == 2);
  WorldIterator it(world->getChildren());
  if (it.first()) {
    SceneElementRawPtr w1 = it.currentElement();
    it.advance();
    SceneElementRawPtr w2 = it.currentElement();

    WorldIterator it1(w1->getChildren());
    if (it1.first()) {
      SceneElementRawPtr tr1 = it1.currentElement();
      auto t1 = dynamic_cast<Triangle *>(tr1->getPrimitive().get());
      if (t1) {
        EXPECT_THAT(t1->point(0), Eq(Point3f(reader.vertexCollection()[0])));
        EXPECT_THAT(t1->point(1), Eq(Point3f(reader.vertexCollection()[1])));
        EXPECT_THAT(t1->point(2), Eq(Point3f(reader.vertexCollection()[2])));
      }
    }

    WorldIterator it2(w2->getChildren());
    if (it2.first()) {
      SceneElementRawPtr tr2 = it2.currentElement();
      auto t2 = dynamic_cast<Triangle *>(tr2->getPrimitive().get());
      if (t2) {
        EXPECT_THAT(t2->point(0), Eq(Point3f(reader.vertexCollection()[0])));
        EXPECT_THAT(t2->point(1), Eq(Point3f(reader.vertexCollection()[2])));
        EXPECT_THAT(t2->point(2), Eq(Point3f(reader.vertexCollection()[3])));
      }
    }
  }
}

TEST_F(ApplicationTest, parseFileWithSmoothTriangles) {
  reader.setFileName(test_directory + "valid_input_sm_triangle.txt");
  reader.parseInput();
  SceneElementPtr world = reader.getStructure();
  ASSERT_TRUE(world->getChildren().size() == 2);
  WorldIterator it(world->getChildren());
  if (it.first()) {
    SceneElementRawPtr tr1 = it.currentElement();
    auto t1 = dynamic_cast<SmoothTriangle *>(tr1->getPrimitive().get());
    it.advance();
    SceneElementRawPtr tr2 = it.currentElement();
    auto t2 = dynamic_cast<SmoothTriangle *>(tr2->getPrimitive().get());

    EXPECT_THAT(t1->points(0), Eq(Point3f(reader.vertexCollection()[0])));
    EXPECT_THAT(t1->points(1), Eq(Point3f(reader.vertexCollection()[1])));
    EXPECT_THAT(t1->points(2), Eq(Point3f(reader.vertexCollection()[2])));

    EXPECT_THAT(t1->normals(0), Eq(reader.vertexNormalCollection()[2]));
    EXPECT_THAT(t1->normals(1), Eq(reader.vertexNormalCollection()[0]));
    EXPECT_THAT(t1->normals(2), Eq(reader.vertexNormalCollection()[1]));

    EXPECT_THAT(t2->points(0), Eq(Point3f(reader.vertexCollection()[0])));
    EXPECT_THAT(t2->points(1), Eq(Point3f(reader.vertexCollection()[1])));
    EXPECT_THAT(t2->points(2), Eq(Point3f(reader.vertexCollection()[2])));

    EXPECT_THAT(t2->normals(0), Eq(reader.vertexNormalCollection()[2]));
    EXPECT_THAT(t2->normals(1), Eq(reader.vertexNormalCollection()[0]));
    EXPECT_THAT(t2->normals(2), Eq(reader.vertexNormalCollection()[1]));
  }
}
