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
    woodMap["plank_width"] = temprenderer::core::parsers::ConfigValue(4.0F);
    woodMap["plank_length"] = temprenderer::core::parsers::ConfigValue(10.0F);
    woodMap["offset_x"] = temprenderer::core::parsers::ConfigValue(1.0F);
    woodMap["offset_z"] = temprenderer::core::parsers::ConfigValue(2.0F);
    auto woodTable = temprenderer::core::parsers::ConfigValue(woodMap);

    auto woodTexture =
        temprenderer::core::config::parseTextureFromConfig(woodTable);
    auto woodSample = woodTexture->sample({4.0F, 0.0F, 10.0F});
    const temprenderer::core::math::WoodTexture expectedWoodTexture{
        {0.72F, 0.46F, 0.22F},
        {0.38F, 0.20F, 0.08F},
        8.0F,
        1.25F,
        4.0F,
        10.0F,
        1.0F,
        2.0F};
    const auto expectedWoodSample =
        expectedWoodTexture.sample({4.0F, 0.0F, 10.0F});
    CHECK(woodSample.r == doctest::Approx(expectedWoodSample.r));
    CHECK(woodSample.g == doctest::Approx(expectedWoodSample.g));
    CHECK(woodSample.b == doctest::Approx(expectedWoodSample.b));
    CHECK(woodSample.r >= 0.0F);
    CHECK(woodSample.g >= 0.0F);
    CHECK(woodSample.b >= 0.0F);
  }

  TEST_CASE("should create metal texture from config") {
    using temprenderer::core::math::TextureContext;
    temprenderer::core::parsers::ConfigValue::Table metalMap;
    metalMap["type"] =
        temprenderer::core::parsers::ConfigValue(std::string{"metal"});
    metalMap["base_color"] = temprenderer::core::parsers::ConfigValue(
        temprenderer::core::parsers::ConfigValue::Table{
            {"r", temprenderer::core::parsers::ConfigValue(
                      static_cast<std::uint8_t>(190))},
            {"g", temprenderer::core::parsers::ConfigValue(
                      static_cast<std::uint8_t>(200))},
            {"b", temprenderer::core::parsers::ConfigValue(
                      static_cast<std::uint8_t>(210))}});
    metalMap["scale"] = temprenderer::core::parsers::ConfigValue(1.0F);
    metalMap["roughness_variation"] =
        temprenderer::core::parsers::ConfigValue(0.0F);
    metalMap["bump_strength"] = temprenderer::core::parsers::ConfigValue(0.0F);
    const temprenderer::core::parsers::ConfigValue metalConfig(metalMap);
    const auto texture =
        temprenderer::core::config::parseTextureFromConfig(metalConfig);
    TextureContext context;
    context.uv = {0.0F, 0.4F};
    context.localPosition = {0.3F, 0.4F, 0.5F};
    const auto color = texture->sample(context);
    const auto normal = kwp::Vec3{0.3F, 0.4F, 0.5F}.normalize();

    CHECK(color.r >= 0.0F);
    CHECK(color.r <= 1.0F);
    CHECK(color.g >= 0.0F);
    CHECK(color.g <= 1.0F);
    CHECK(color.b >= 0.0F);
    CHECK(color.b <= 1.0F);
    CHECK(color.r < color.g);
    CHECK(color.g < color.b);
    CHECK(texture->sampleRoughness(context, 0.48F) == doctest::Approx(0.48F));
    CHECK(kwp::dot(texture->sampleNormal(context, normal), normal) ==
          doctest::Approx(1.0F));
  }

  TEST_CASE("should parse rectangular area light dimensions") {
    using temprenderer::core::parsers::ConfigValue;
    ConfigValue::Table lightTable;
    lightTable["type"] = ConfigValue(std::string{"area"});
    lightTable["name"] = ConfigValue(std::string{"softbox"});
    lightTable["intensity"] = ConfigValue(2.0F);
    lightTable["width"] = ConfigValue(8.0F);
    lightTable["height"] = ConfigValue(3.0F);

    const auto light = temprenderer::core::config::LightConfig::loadConfig(
        ConfigValue(lightTable));
    CHECK(light.type == temprenderer::core::config::LightType::AREA);
    CHECK(light.width == doctest::Approx(8.0F));
    CHECK(light.height == doctest::Approx(3.0F));
  }

  TEST_CASE("should parse metallic and roughness material parameters") {
    using temprenderer::core::parsers::ConfigValue;
    ConfigValue::Table materialTable;
    materialTable["metallic"] = ConfigValue(0.85F);
    materialTable["roughness"] = ConfigValue(0.28F);
    ConfigValue::Table objectTable;
    objectTable["type"] = ConfigValue(std::string{"sphere"});
    objectTable["material"] = ConfigValue(materialTable);

    const auto object = temprenderer::core::config::ObjectConfig::loadConfig(
        ConfigValue(objectTable));
    CHECK(object.material.metallic == doctest::Approx(0.85F));
    CHECK(object.material.roughness == doctest::Approx(0.28F));
  }
}
