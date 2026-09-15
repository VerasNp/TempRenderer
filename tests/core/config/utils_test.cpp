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
}
