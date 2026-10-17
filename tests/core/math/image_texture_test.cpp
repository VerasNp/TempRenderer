#include "temprenderer/core/config/utils.hpp"
#include "temprenderer/core/math/ImageTexture.hpp"

#include <doctest/doctest.h>

#include <filesystem>
#include <fstream>
#include <string>

using temprenderer::core::math::ImageTexture;
using temprenderer::core::math::TextureContext;

TEST_SUITE("ImageTexture") {
  TEST_CASE("loads PPM pixels and bilinearly samples UV coordinates") {
    const auto path = std::filesystem::temp_directory_path() /
                      "temprenderer_image_texture_test.ppm";
    {
      std::ofstream image(path);
      image << "P3\n2 2\n255\n"
               "255 0 0  0 255 0\n"
               "0 0 255  255 255 255\n";
    }

    const ImageTexture texture(path);
    TextureContext context;
    context.uv = {0.25F, 0.25F};
    const auto topLeft = texture.sample(context);
    CHECK(topLeft.r == doctest::Approx(1.0F));
    CHECK(topLeft.g == doctest::Approx(0.0F));
    CHECK(topLeft.b == doctest::Approx(0.0F));

    context.uv = {0.75F, 0.25F};
    const auto topRight = texture.sample(context);
    CHECK(topRight.r == doctest::Approx(0.0F));
    CHECK(topRight.g == doctest::Approx(1.0F));
    CHECK(topRight.b == doctest::Approx(0.0F));

    context.uv = {0.5F, 0.5F};
    const auto center = texture.sample(context);
    CHECK(center.r == doctest::Approx(0.5F));
    CHECK(center.g == doctest::Approx(0.5F));
    CHECK(center.b == doctest::Approx(0.5F));

    context.uv = {1.25F, 0.25F};
    const auto wrapped = texture.sample(context);
    CHECK(wrapped.r == doctest::Approx(topLeft.r));
    CHECK(wrapped.g == doctest::Approx(topLeft.g));
    CHECK(wrapped.b == doctest::Approx(topLeft.b));

    context.uv = {0.25F, 1.25F};
    const auto wrappedV = texture.sample(context);
    CHECK(wrappedV.r == doctest::Approx(topLeft.r));
    CHECK(wrappedV.g == doctest::Approx(topLeft.g));
    CHECK(wrappedV.b == doctest::Approx(topLeft.b));

    context.repeatV = false;
    const auto clampedV = texture.sample(context);
    CHECK(clampedV.r == doctest::Approx(0.0F));
    CHECK(clampedV.g == doctest::Approx(0.0F));
    CHECK(clampedV.b == doctest::Approx(1.0F));
    std::filesystem::remove(path);
  }

  TEST_CASE("rejects missing or unsupported image files") {
    const auto missing = std::filesystem::temp_directory_path() /
                         "temprenderer_missing_image_texture.ppm";
    CHECK_THROWS_AS(ImageTexture{missing}, std::runtime_error);

    const auto path = std::filesystem::temp_directory_path() /
                      "temprenderer_unsupported_image_texture.txt";
    {
      std::ofstream image(path);
      image << "not an image";
    }
    CHECK_THROWS_AS(ImageTexture{path}, std::runtime_error);
    std::filesystem::remove(path);
  }

  TEST_CASE("loads binary PPM images") {
    const auto path = std::filesystem::temp_directory_path() /
                      "temprenderer_binary_image_texture_test.ppm";
    {
      std::ofstream image(path, std::ios::binary);
      image << "P6\n1 1\n255\n";
      image.put(static_cast<char>(40));
      image.put(static_cast<char>(120));
      image.put(static_cast<char>(220));
    }

    const ImageTexture texture(path);
    TextureContext context;
    context.uv = {0.5F, 0.5F};
    const auto color = texture.sample(context);
    CHECK(color.r == doctest::Approx(40.0F / 255.0F));
    CHECK(color.g == doctest::Approx(120.0F / 255.0F));
    CHECK(color.b == doctest::Approx(220.0F / 255.0F));
    std::filesystem::remove(path);
  }

  TEST_CASE("projects a sphere preview image without sampling its background") {
    const auto directory = std::filesystem::temp_directory_path() /
                           "temprenderer_sphere_preview_test";
    std::filesystem::create_directories(directory);
    const auto path = directory / "preview.ppm";
    {
      std::ofstream image(path, std::ios::binary);
      image << "P6\n64 64\n255\n";
      for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
          const float dx = static_cast<float>(x) - 31.5F;
          const float dy = static_cast<float>(y) - 31.5F;
          const bool insideSphere = dx * dx + dy * dy <= 30.0F * 30.0F;
          image.put(static_cast<char>(insideSphere ? 180 : 0));
          image.put(static_cast<char>(insideSphere ? 210 : 0));
          image.put(static_cast<char>(insideSphere ? 240 : 0));
        }
      }
    }

    temprenderer::core::parsers::ConfigValue::Table imageConfig;
    imageConfig["type"] =
        temprenderer::core::parsers::ConfigValue(std::string{"image"});
    imageConfig["path"] =
        temprenderer::core::parsers::ConfigValue(std::string{"preview.ppm"});
    imageConfig["projection"] =
        temprenderer::core::parsers::ConfigValue(std::string{"sphere_preview"});
    const temprenderer::core::parsers::ConfigValue config(imageConfig);
    const auto texture =
        temprenderer::core::config::parseTextureFromConfig(config, directory);

    for (const auto &normal :
         {kwp::Vec3{1.0F, 0.0F, 0.0F}, kwp::Vec3{-1.0F, 0.0F, 0.0F},
          kwp::Vec3{0.0F, 1.0F, 0.0F}, kwp::Vec3{0.0F, -1.0F, 0.0F},
          kwp::Vec3{0.0F, 0.0F, 1.0F}, kwp::Vec3{0.0F, 0.0F, -1.0F}}) {
      TextureContext context;
      context.normal = normal;
      const auto color = texture->sample(context);
      CHECK(color.r == doctest::Approx(180.0F / 255.0F));
      CHECK(color.g == doctest::Approx(210.0F / 255.0F));
      CHECK(color.b == doctest::Approx(240.0F / 255.0F));
    }
    std::filesystem::remove_all(directory);
  }

  TEST_CASE("parses relative image texture paths against the project folder") {
    const auto directory = std::filesystem::temp_directory_path() /
                           "temprenderer_image_config_test";
    std::filesystem::create_directories(directory);
    {
      std::ofstream image(directory / "texture.ppm");
      image << "P3\n1 1\n255\n40 120 220\n";
    }
    temprenderer::core::parsers::ConfigValue::Table imageConfig;
    imageConfig["type"] =
        temprenderer::core::parsers::ConfigValue(std::string{"image"});
    imageConfig["path"] =
        temprenderer::core::parsers::ConfigValue(std::string{"texture.ppm"});
    const temprenderer::core::parsers::ConfigValue config(imageConfig);

    const auto texture =
        temprenderer::core::config::parseTextureFromConfig(config, directory);
    TextureContext context;
    context.uv = {0.5F, 0.5F};
    const auto color = texture->sample(context);
    CHECK(color.r == doctest::Approx(40.0F / 255.0F));
    CHECK(color.g == doctest::Approx(120.0F / 255.0F));
    CHECK(color.b == doctest::Approx(220.0F / 255.0F));
    std::filesystem::remove_all(directory);
  }
}
