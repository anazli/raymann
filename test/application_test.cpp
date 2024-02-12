#include "application/wavefront_reader.h"
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
