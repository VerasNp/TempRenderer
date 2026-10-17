#include "temprenderer/core/math/Materials.hpp"

#include <doctest/doctest.h>

#include <cmath>

using temprenderer::core::math::ColorF;
using temprenderer::core::math::evaluateMetalSpecular;
using temprenderer::core::math::MetalTexture;
using temprenderer::core::math::TextureContext;

TEST_SUITE("MetalTexture") {
  TEST_CASE("procedural metal samples are repeatable, bounded, and varied") {
    const MetalTexture texture;
    TextureContext firstContext;
    firstContext.uv = {0.137F, 0.41F};
    TextureContext secondContext = firstContext;
    const auto first = texture.sample(firstContext);
    const auto repeated = texture.sample(secondContext);

    CHECK(first.r == doctest::Approx(repeated.r));
    CHECK(first.g == doctest::Approx(repeated.g));
    CHECK(first.b == doctest::Approx(repeated.b));
    CHECK(first.r >= 0.0F);
    CHECK(first.r <= 1.0F);
    CHECK(first.g >= 0.0F);
    CHECK(first.g <= 1.0F);
    CHECK(first.b >= 0.0F);
    CHECK(first.b <= 1.0F);

    bool hasVariation = false;
    for (float u = 0.0F; u < 1.0F; u += 0.013F) {
      TextureContext context;
      context.uv = {u, 0.41F};
      const auto sample = texture.sample(context);
      if (std::abs(sample.r - first.r) + std::abs(sample.g - first.g) +
              std::abs(sample.b - first.b) >
          0.04F) {
        hasVariation = true;
        break;
      }
    }
    CHECK(hasVariation);
  }

  TEST_CASE("galvanized surface varies roughness and subtly perturbs normals") {
    const MetalTexture texture;
    TextureContext firstContext;
    firstContext.localPosition = {0.35F, 0.42F, 0.81F};
    firstContext.uv = {0.19F, 0.31F};
    TextureContext secondContext;
    secondContext.localPosition = {-0.62F, 0.17F, 0.76F};
    secondContext.uv = {0.63F, 0.27F};
    const float firstRoughness = texture.sampleRoughness(firstContext, 0.48F);
    const float secondRoughness = texture.sampleRoughness(secondContext, 0.48F);
    const auto firstNormal = texture.sampleNormal(
        firstContext, kwp::Vec3{0.35F, 0.42F, 0.81F}.normalize());
    const kwp::Vec3 originalNormal{0.35F, 0.42F, 0.81F};

    CHECK(firstRoughness >= 0.04F);
    CHECK(firstRoughness <= 1.0F);
    CHECK(secondRoughness >= 0.04F);
    CHECK(secondRoughness <= 1.0F);
    CHECK(std::abs(firstRoughness - secondRoughness) > 0.001F);
    CHECK(kwp::dot(firstNormal, originalNormal.normalize()) > 0.99F);
    CHECK(kwp::dot(firstNormal, originalNormal.normalize()) < 1.0F);
  }

  TEST_CASE(
      "metal specular response is colored and roughness broadens highlight") {
    const ColorF goldF0{0.9F, 0.62F, 0.18F};
    const auto smooth =
        evaluateMetalSpecular(goldF0, 1.0F, 1.0F, 1.0F, 1.0F, 0.12F);
    const auto rough =
        evaluateMetalSpecular(goldF0, 1.0F, 1.0F, 1.0F, 1.0F, 0.75F);
    CHECK(std::isfinite(smooth.r));
    CHECK(std::isfinite(smooth.g));
    CHECK(std::isfinite(smooth.b));
    CHECK(smooth.r > rough.r);
    CHECK(smooth.g > rough.g);
    CHECK(smooth.b > rough.b);
    CHECK(smooth.r > smooth.g);
    CHECK(smooth.g > smooth.b);
  }

  TEST_CASE("metal response is zero outside the lit hemisphere") {
    const auto response = evaluateMetalSpecular({0.8F, 0.7F, 0.6F}, -0.1F, 1.0F,
                                                0.5F, 0.5F, 0.4F);
    CHECK(response.r == doctest::Approx(0.0F));
    CHECK(response.g == doctest::Approx(0.0F));
    CHECK(response.b == doctest::Approx(0.0F));
  }
}
