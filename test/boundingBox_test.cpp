#include "geometry/bounding_box.h"

#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;
using namespace std;

class TBox : public Test {
 public:
    SceneElementPtr box;
    Ray ray;
};

TEST_F(TBox, rayIntersectsCube) {
    box = make_shared<BoundingBox>();

    ray = Ray(Point3f(5.f, 0.5f, 0.f), Vec3f(-1.f, 0.f, 0.f));
    auto rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);

    ray = Ray(Point3f(-5.f, 0.5f, 0.f), Vec3f(1.f, 0.f, 0.f));
    rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);

    ray = Ray(Point3f(0.5f, 5.f, 0.f), Vec3f(0.f, -1.f, 0.f));
    rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);

    ray = Ray(Point3f(0.5f, -5.f, 0.f), Vec3f(0.f, 1.f, 0.f));
    rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);

    ray = Ray(Point3f(0.5f, 0.f, 5.f), Vec3f(0.f, 0.f, -1.f));
    rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);

    ray = Ray(Point3f(0.5f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f));
    rec = IntersectionRecord{};
    EXPECT_TRUE(box->intersect(ray, rec));
    EXPECT_TRUE(rec.count == 2);
    EXPECT_EQ(rec.t1, 4.f);
    EXPECT_EQ(rec.t2, 6.f);
}