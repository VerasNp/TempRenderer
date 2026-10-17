#include "temprenderer/core/math/Materials.hpp"

#include <doctest/doctest.h>

#include <cmath>

using temprenderer::core::math::ColorF;
using temprenderer::core::math::WoodTexture;

TEST_SUITE("WoodTexture") {
  TEST_CASE("samples are deterministic and remain in the display range") {
    const WoodTexture texture{
        {0.72F, 0.46F, 0.22F}, {0.38F, 0.20F, 0.08F}, 18.0F, 0.8F};
    const kwp::Point3 point{14.25F, -40.0F, 42.75F};
    const ColorF first = texture.sample(point);
    const ColorF second = texture.sample(point);

    CHECK(first.r == doctest::Approx(second.r));
    CHECK(first.g == doctest::Approx(second.g));
    CHECK(first.b == doctest::Approx(second.b));
    CHECK(first.r >= 0.0F);
    CHECK(first.r <= 1.0F);
    CHECK(first.g >= 0.0F);
    CHECK(first.g <= 1.0F);
    CHECK(first.b >= 0.0F);
    CHECK(first.b <= 1.0F);
  }

  TEST_CASE("grain varies within each plank") {
    const WoodTexture texture{
        {0.72F, 0.46F, 0.22F}, {0.38F, 0.20F, 0.08F}, 18.0F, 0.8F};
    const ColorF first = texture.sample({5.0F, 0.0F, 10.0F});
    bool varies = false;

    for (float x = 6.0F; x < 30.0F; x += 1.0F) {
      for (float z = 10.0F; z < 110.0F; z += 3.0F) {
        const ColorF sample = texture.sample({x, 0.0F, z});
        if (std::abs(sample.r - first.r) + std::abs(sample.g - first.g) +
                std::abs(sample.b - first.b) >
            0.02F) {
          varies = true;
          break;
        }
      }
      if (varies) {
        break;
      }
    }

    CHECK(varies);
  }

  TEST_CASE("scale and turbulence change the sampled grain") {
    const ColorF base{0.72F, 0.46F, 0.22F};
    const ColorF rings{0.38F, 0.20F, 0.08F};
    const WoodTexture fineGrain{base, rings, 6.0F, 0.0F};
    const WoodTexture coarseGrain{base, rings, 24.0F, 1.4F};
    float difference = 0.0F;

    for (float x = 3.0F; x < 32.0F; x += 2.0F) {
      for (float z = 4.0F; z < 116.0F; z += 7.0F) {
        const ColorF first = fineGrain.sample({x, 0.0F, z});
        const ColorF second = coarseGrain.sample({x, 0.0F, z});
        difference += std::abs(first.r - second.r) +
                      std::abs(first.g - second.g) +
                      std::abs(first.b - second.b);
      }
    }

    CHECK(difference > 1.0F);
  }

  TEST_CASE("plank joints are darker than the board surface") {
    const WoodTexture texture{
        {0.72F, 0.46F, 0.22F}, {0.38F, 0.20F, 0.08F}, 18.0F, 0.8F};
    const ColorF joint = texture.sample({35.1F, 0.0F, 20.0F});
    const ColorF board = texture.sample({39.0F, 0.0F, 20.0F});

    CHECK(joint.r + joint.g + joint.b < board.r + board.g + board.b);
  }
}
