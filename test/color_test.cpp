#include "color/spectrum.h"
#include "gtesting.h"

class ColorTest : public testing::RTest {
 public:
  Spectrum s1;
  Spectrum s2;
  ColorTest() {
    s1.samples()[0] = 1.f;
    s1.samples()[1] = 2.f;
    s1.samples()[2] = 3.f;

    s2.samples()[0] = 4.f;
    s2.samples()[1] = 5.f;
    s2.samples()[2] = 6.f;
  }
};

TEST_F(ColorTest, unaryMinus) {
  s1 = -s1;
  EXPECT_EQ(s1.samples()[0], -1.f);
  EXPECT_EQ(s1.samples()[1], -2.f);
  EXPECT_EQ(s1.samples()[2], -3.f);
}

TEST_F(ColorTest, unaryAddition) {
  s1 += -s1;
  EXPECT_EQ(s1.samples()[0], 0.f);
  EXPECT_EQ(s1.samples()[1], 0.f);
  EXPECT_EQ(s1.samples()[2], 0.f);
}

TEST_F(ColorTest, unarySubtraction) {
  s1 -= s1;
  EXPECT_EQ(s1.samples()[0], 0.f);
  EXPECT_EQ(s1.samples()[1], 0.f);
  EXPECT_EQ(s1.samples()[2], 0.f);
}

TEST_F(ColorTest, unaryMultiplication) {
  s1 *= s1;
  EXPECT_EQ(s1.samples()[0], 1.f);
  EXPECT_EQ(s1.samples()[1], 4.f);
  EXPECT_EQ(s1.samples()[2], 9.f);
}

TEST_F(ColorTest, unaryDivision) {
  s1 /= s1;
  EXPECT_EQ(s1.samples()[0], 1.f);
  EXPECT_EQ(s1.samples()[1], 1.f);
  EXPECT_EQ(s1.samples()[2], 1.f);
}

TEST_F(ColorTest, clampSpectrum) {
  s1.samples()[0] = 200.f;
  s1.samples()[1] = -200.f;
  s1.samples()[2] = -600.f;
  s1.samples()[3] = 900.f;
  s1 = s1.clamp(0.f, 100.f);
  EXPECT_EQ(s1.samples()[0], 100.f);
  EXPECT_EQ(s1.samples()[1], 0.f);
  EXPECT_EQ(s1.samples()[2], 0.f);
  EXPECT_EQ(s1.samples()[3], 100.f);
}

TEST_F(ColorTest, spectrumEquality) {
  Spectrum s3 = s1;
  ASSERT_TRUE(s1 == s3);
}

TEST_F(ColorTest, spectrumInequality) {
  Spectrum s3;
  s3.samples()[0] = 4.f;
  s3.samples()[1] = 5.f;
  s3.samples()[2] = 6.f;
  ASSERT_TRUE(s1 != s3);
}

TEST_F(ColorTest, spectrumIsBlackTest) {
  EXPECT_FALSE(s1.isBlack());
  Spectrum s3(0.f);
  EXPECT_TRUE(s3.isBlack());
}

TEST_F(ColorTest, spectrumHasNaNsTest) {
  EXPECT_FALSE(s1.hasNaNs());
  Spectrum s3(0.f);
  s3.samples()[s3.samples().size() - 1] = std::nan("9");
  EXPECT_TRUE(s3.hasNaNs());
}

TEST_F(ColorTest, binaryAdditionWithSpectrum) {
  Spectrum s3 = s1 + s2;
  EXPECT_EQ(s3.samples()[0], 5.f);
  EXPECT_EQ(s3.samples()[1], 7.f);
  EXPECT_EQ(s3.samples()[2], 9.f);
}

TEST_F(ColorTest, binarySubtractionWithSpectrum) {
  Spectrum s3 = s2 - s1;
  EXPECT_EQ(s3.samples()[0], 3.f);
  EXPECT_EQ(s3.samples()[1], 3.f);
  EXPECT_EQ(s3.samples()[2], 3.f);
}

TEST_F(ColorTest, binaryMultiplicationWithSpectrum) {
  Spectrum s3 = s1 * s2;
  EXPECT_EQ(s3.samples()[0], 4.f);
  EXPECT_EQ(s3.samples()[1], 10.f);
  EXPECT_EQ(s3.samples()[2], 18.f);
}

TEST_F(ColorTest, binaryDivisionWithSpectrum) {
  Spectrum s3 = s2 / s1;
  EXPECT_EQ(s3.samples()[0], 4.f);
  EXPECT_EQ(s3.samples()[1], 2.5f);
  EXPECT_EQ(s3.samples()[2], 2.f);
}

TEST_F(ColorTest, binaryAdditionWithNumber) {
  Spectrum s3 = s1 + 5.f;
  EXPECT_EQ(s3.samples()[0], 6.f);
  EXPECT_EQ(s3.samples()[1], 7.f);
  EXPECT_EQ(s3.samples()[2], 8.f);
}

TEST_F(ColorTest, binarySubtractionWithNumber) {
  Spectrum s3 = s1 - 5.f;
  EXPECT_EQ(s3.samples()[0], -4.f);
  EXPECT_EQ(s3.samples()[1], -3.f);
  EXPECT_EQ(s3.samples()[2], -2.f);
}

TEST_F(ColorTest, binaryMultiplicationWithNumber) {
  Spectrum s3 = s1 * 5.f;
  EXPECT_EQ(s3.samples()[0], 5.f);
  EXPECT_EQ(s3.samples()[1], 10.f);
  EXPECT_EQ(s3.samples()[2], 15.f);
}

TEST_F(ColorTest, binaryDivisionWithNumber) {
  Spectrum s3 = s1 / 5.f;
  EXPECT_EQ(s3.samples()[0], 1.f / 5.f);
  EXPECT_EQ(s3.samples()[1], 2.f / 5.f);
  EXPECT_EQ(s3.samples()[2], 3.f / 5.f);
}

TEST_F(ColorTest, sqrtOfSpectrum) {
  Spectrum s3 = sqrt(s1);
  float eps = 1E-7f;
  EXPECT_NEAR(s3.samples()[0], std::sqrt(1.f), eps);
  EXPECT_NEAR(s3.samples()[1], std::sqrt(2.f), eps);
  EXPECT_NEAR(s3.samples()[2], std::sqrt(3.f), eps);

  Spectrum s4 = sqrt(s2);
  EXPECT_NEAR(s4.samples()[0], std::sqrt(4.f), eps);
  EXPECT_NEAR(s4.samples()[1], std::sqrt(5.f), eps);
  EXPECT_NEAR(s4.samples()[2], std::sqrt(6.f), eps);
}

TEST_F(ColorTest, expOfSpectrum) {
  Spectrum s3 = exp(s1);
  float eps = 1E-7f;
  EXPECT_NEAR(s3.samples()[0], std::exp(1.f), eps);
  EXPECT_NEAR(s3.samples()[1], std::exp(2.f), eps);
  EXPECT_NEAR(s3.samples()[2], std::exp(3.f), eps);
}

TEST_F(ColorTest, powOfSpectrum) {
  Spectrum s3 = pow(s1, 2);
  EXPECT_EQ(s3.samples()[0], 1.f);
  EXPECT_EQ(s3.samples()[1], 4.f);
  EXPECT_EQ(s3.samples()[2], 9.f);

  s3 = pow(s1, 3);
  EXPECT_EQ(s3.samples()[0], 1.f);
  EXPECT_EQ(s3.samples()[1], 8.f);
  EXPECT_EQ(s3.samples()[2], 27.f);
}
