#include "temprenderer/core/config/utils.hpp"

#include <doctest/doctest.h>
#include <string>

TEST_SUITE("Utils") {
  TEST_CASE("should given aspect ratio as string on format <integer>:<integer> "
            "convert to it correspondent enum") {
    auto result = temprenderer::core::config::stringToAspectRatio("4:3");
    CHECK_EQ(result, temprenderer::core::config::AspectRatio::STANDARD);
  }
  TEST_CASE("should given aspect ratio as string on format <integer>:<integer> "
            "not existent set default aspect ratio") {
    auto result = temprenderer::core::config::stringToAspectRatio("123:564");
    CHECK_EQ(result, temprenderer::core::config::AspectRatio::WIDESCREEN);
  }
  TEST_CASE("should calculate the viewport width with given resolution height, "
            "width and viewport height") {
    auto result =
        temprenderer::core::config::calculateViewportWidth(1280, 720, 2.0F);
    CHECK_EQ(result, 3.55555558F);
  }
  TEST_CASE("should given an aspect ratio as enum covert it to scalar") {
    float result = temprenderer::core::config::aspectRatioToScalar(
        temprenderer::core::config::AspectRatio::WIDESCREEN);
    CHECK_EQ(result, doctest::Approx(1.77778F));
  }
  TEST_CASE("should calculate the resolution height with given aspect ratio "
            "and resolution width") {
    auto result = temprenderer::core::config::calculateResolutionHeight(
        temprenderer::core::config::AspectRatio::WIDESCREEN, 1280);
    CHECK_EQ(result, 720);
  }
  TEST_CASE("should create solid texture and wood texture from config") {
    temprenderer::core::parsers::ConfigValue::Table solidMap;
    solidMap["type"] = temprenderer::core::parsers::ConfigValue("solid");
    temprenderer::core::parsers::ConfigValue::Table solidColor;
    solidColor["r"] = temprenderer::core::parsers::ConfigValue(
        static_cast<std::uint8_t>(255));
    solidColor["g"] =
        temprenderer::core::parsers::ConfigValue(static_cast<std::uint8_t>(0));
    solidColor["b"] = temprenderer::core::parsers::ConfigValue(
        static_cast<std::uint8_t>(128));
    solidMap["color"] = temprenderer::core::parsers::ConfigValue(solidColor);
    auto solidTable = temprenderer::core::parsers::ConfigValue(solidMap);

    auto solidTexture =
        temprenderer::core::config::parseTextureFromConfig(solidTable);
    auto solidSample = solidTexture->sample({0.0F, 0.0F, 0.0F});
    CHECK_EQ(solidSample.r, doctest::Approx(1.0F));
    CHECK_EQ(solidSample.g, doctest::Approx(0.0F));
    CHECK_EQ(solidSample.b, doctest::Approx(128.0F / 255.0F));

    temprenderer::core::parsers::ConfigValue::Table woodMap;
    woodMap["type"] = temprenderer::core::parsers::ConfigValue("wood");
    woodMap["scale"] = temprenderer::core::parsers::ConfigValue(8.0F);
    woodMap["turbulence"] = temprenderer::core::parsers::ConfigValue(1.25F);
    auto woodTable = temprenderer::core::parsers::ConfigValue(woodMap);

    auto woodTexture =
        temprenderer::core::config::parseTextureFromConfig(woodTable);
    auto woodSample = woodTexture->sample({4.0F, 0.0F, 10.0F});
    const temprenderer::core::math::WoodTexture expectedWoodTexture{
        {0.72F, 0.46F, 0.22F}, {0.38F, 0.20F, 0.08F}, 8.0F, 1.25F};
    const auto expectedWoodSample =
        expectedWoodTexture.sample({4.0F, 0.0F, 10.0F});
    CHECK(woodSample.r == doctest::Approx(expectedWoodSample.r));
    CHECK(woodSample.g == doctest::Approx(expectedWoodSample.g));
    CHECK(woodSample.b == doctest::Approx(expectedWoodSample.b));
    CHECK(woodSample.r >= 0.0F);
    CHECK(woodSample.g >= 0.0F);
    CHECK(woodSample.b >= 0.0F);
  }
}
