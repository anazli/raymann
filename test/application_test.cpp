#include "application/wavefront_reader.h"
#include "composite/iterator.h"
#include "gtesting.h"

class ApplicationTest : public testing::RTest {
 public:
  WavefrontReader reader;
};

TEST_F(ApplicationTest, parseInvalidInput) {
  reader.openFile("invalid_input.txt");
  reader.parseInput();
  EXPECT_TRUE(reader.vertexCollection().size() == 0);
}

TEST_F(ApplicationTest, parseValidInput) {
  reader.openFile("valid_input.txt");
  reader.parseInput();
  ASSERT_TRUE(reader.vertexCollection().size() == 4);
  compareVectors(reader.vertexCollection()[0], Vec3f(1.f, 2.f, 3.f));
  compareVectors(reader.vertexCollection()[1], Vec3f(-1.f, -2.f, -3.f));
  compareVectors(reader.vertexCollection()[2], Vec3f(1.5f, 4.5f, -0.3f));
  compareVectors(reader.vertexCollection()[3], Vec3f(-4.05f, 1.053f, 0.0005f));

  ASSERT_TRUE(reader.triangleCollection().size() == 2);
  comparePoints(reader.triangleCollection()[0].point(0),
                Point3f(reader.vertexCollection()[0]));
  comparePoints(reader.triangleCollection()[0].point(1),
                Point3f(reader.vertexCollection()[1]));
  comparePoints(reader.triangleCollection()[0].point(2),
                Point3f(reader.vertexCollection()[2]));

  comparePoints(reader.triangleCollection()[1].point(0),
                Point3f(reader.vertexCollection()[0]));
  comparePoints(reader.triangleCollection()[1].point(1),
                Point3f(reader.vertexCollection()[2]));
  comparePoints(reader.triangleCollection()[1].point(2),
                Point3f(reader.vertexCollection()[3]));
}

TEST_F(ApplicationTest, parseValidPolygonInput) {
  reader.openFile("valid_input_polygon.txt");
  reader.parseInput();
  ASSERT_TRUE(reader.vertexCollection().size() == 5);

  ASSERT_TRUE(reader.triangleCollection().size() == 3);
  comparePoints(reader.triangleCollection()[0].point(0),
                Point3f(reader.vertexCollection()[0]));
  comparePoints(reader.triangleCollection()[0].point(1),
                Point3f(reader.vertexCollection()[1]));
  comparePoints(reader.triangleCollection()[0].point(2),
                Point3f(reader.vertexCollection()[2]));

  comparePoints(reader.triangleCollection()[1].point(0),
                Point3f(reader.vertexCollection()[0]));
  comparePoints(reader.triangleCollection()[1].point(1),
                Point3f(reader.vertexCollection()[2]));
  comparePoints(reader.triangleCollection()[1].point(2),
                Point3f(reader.vertexCollection()[3]));

  comparePoints(reader.triangleCollection()[2].point(0),
                Point3f(reader.vertexCollection()[0]));
  comparePoints(reader.triangleCollection()[2].point(1),
                Point3f(reader.vertexCollection()[3]));
  comparePoints(reader.triangleCollection()[2].point(2),
                Point3f(reader.vertexCollection()[4]));
}

TEST_F(ApplicationTest, parseFileWithGroups) {
  reader.openFile("valid_input_groups.txt");
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
      Triangle *t1 = dynamic_cast<Triangle *>(tr1);
      if (t1) {
        comparePoints(t1->point(0), Point3f(reader.vertexCollection()[0]));
        comparePoints(t1->point(1), Point3f(reader.vertexCollection()[1]));
        comparePoints(t1->point(2), Point3f(reader.vertexCollection()[2]));
      }
    }

    WorldIterator it2(w2->getChildren());
    if (it2.first()) {
      SceneElementRawPtr tr2 = it2.currentElement();
      Triangle *t2 = dynamic_cast<Triangle *>(tr2);
      if (t2) {
        comparePoints(t2->point(0), Point3f(reader.vertexCollection()[0]));
        comparePoints(t2->point(1), Point3f(reader.vertexCollection()[2]));
        comparePoints(t2->point(2), Point3f(reader.vertexCollection()[3]));
      }
    }
  }
}
