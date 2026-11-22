#include "temprenderer/renderer/Cone.hpp"
#include "temprenderer/renderer/Cylinder.hpp"
#include "temprenderer/renderer/Plane.hpp"
#include "temprenderer/renderer/Sphere.hpp"

#include <doctest/doctest.h>

using temprenderer::core::math::Material;
using temprenderer::core::math::Ray;
using temprenderer::renderer::Cone;
using temprenderer::renderer::Cylinder;
using temprenderer::renderer::Plane;
using temprenderer::scene::Sphere;
using temprenderer::scene::SurfaceInteraction;

TEST_SUITE("TextureCoordinates") {
  TEST_CASE("sphere hit has spherical UVs and object-relative coordinates") {
    const Sphere sphere{{0.0F, 0.0F, 0.0F}, 1.0F, Material{}};
    const Ray ray{{0.0F, 0.0F, 3.0F}, {0.0F, 0.0F, -1.0F}};
    SurfaceInteraction hit{};

    REQUIRE(sphere.intersect(ray, &hit));
    CHECK(hit.textureContext.worldPosition.z == doctest::Approx(1.0F));
    CHECK(hit.textureContext.localPosition.z == doctest::Approx(1.0F));
    CHECK(hit.textureContext.uv.u == doctest::Approx(0.75F));
    CHECK(hit.textureContext.uv.v == doctest::Approx(0.5F));
    CHECK(hit.textureContext.repeatU);
    CHECK_FALSE(hit.textureContext.repeatV);
  }

  TEST_CASE("plane hit has stable tangent-plane coordinates") {
    const Plane plane{{0.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, Material{}};
    const Ray ray{{2.0F, 2.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    SurfaceInteraction hit{};

    REQUIRE(plane.intersect(ray, &hit));
    CHECK(hit.textureContext.worldPosition.x == doctest::Approx(2.0F));
    CHECK(hit.textureContext.uv.u == doctest::Approx(3.0F));
    CHECK(hit.textureContext.uv.v == doctest::Approx(2.0F));
  }

  TEST_CASE("cylinder side and cap each have UVs") {
    const Cylinder cylinder{
        {0.0F, 0.0F, 0.0F}, 1.0F, {0.0F, 2.0F, 0.0F}, Material{}};
    SurfaceInteraction sideHit{};
    const Ray sideRay{{0.0F, 1.0F, 3.0F}, {0.0F, 0.0F, -1.0F}};
    REQUIRE(cylinder.intersect(sideRay, &sideHit));
    CHECK(sideHit.textureContext.uv.u == doctest::Approx(0.5F));
    CHECK(sideHit.textureContext.uv.v == doctest::Approx(0.5F));
    CHECK(sideHit.textureContext.repeatU);
    CHECK_FALSE(sideHit.textureContext.repeatV);

    SurfaceInteraction capHit{};
    const Ray capRay{{0.0F, -2.0F, 0.0F}, {0.0F, 1.0F, 0.0F}};
    REQUIRE(cylinder.intersect(capRay, &capHit));
    CHECK(capHit.textureContext.uv.u == doctest::Approx(0.5F));
    CHECK(capHit.textureContext.uv.v == doctest::Approx(0.5F));
    CHECK_FALSE(capHit.textureContext.repeatU);
    CHECK_FALSE(capHit.textureContext.repeatV);
  }

  TEST_CASE("cone side and base each have UVs") {
    const Cone cone{{0.0F, 0.0F, 0.0F}, 1.0F, {0.0F, 2.0F, 0.0F}, Material{}};
    SurfaceInteraction sideHit{};
    const Ray sideRay{{0.0F, 1.0F, 3.0F}, {0.0F, 0.0F, -1.0F}};
    REQUIRE(cone.intersect(sideRay, &sideHit));
    CHECK(sideHit.textureContext.uv.u >= 0.0F);
    CHECK(sideHit.textureContext.uv.u <= 1.0F);
    CHECK(sideHit.textureContext.uv.v == doctest::Approx(0.5F));

    SurfaceInteraction baseHit{};
    const Ray baseRay{{0.0F, -2.0F, 0.0F}, {0.0F, 1.0F, 0.0F}};
    REQUIRE(cone.intersect(baseRay, &baseHit));
    CHECK(baseHit.textureContext.uv.u == doctest::Approx(0.5F));
    CHECK(baseHit.textureContext.uv.v == doctest::Approx(0.5F));
  }
}
