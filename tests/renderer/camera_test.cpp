#include "../../include/temprenderer/renderer/Camera.hpp"

#include <cstdint>
#include <doctest/doctest.h>

TEST_SUITE("camera") {
  kwp::Point3 eye(0, 0, 0);
  std::uint16_t defaultResolutionWidth = 1220;
  std::uint16_t defaultResolutionHeight = 1220;
  std::uint16_t defaultViewportWidth = 1220;
  std::uint16_t defaultViewportHeight = 3;
  TEST_CASE("should init the camera with given values") {
    temprenderer::renderer::Camera camera(

    );
  }
}
