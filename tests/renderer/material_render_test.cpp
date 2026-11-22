#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/Light.hpp"
#include "temprenderer/renderer/PhongIntegrator.hpp"
#include "temprenderer/renderer/Plane.hpp"
#include "temprenderer/renderer/Sphere.hpp"
#include "temprenderer/scene/Scene.hpp"

#include <doctest/doctest.h>

#include <memory>

using temprenderer::core::math::Color;
using temprenderer::core::math::ColorF;
using temprenderer::core::math::Material;
using temprenderer::core::math::MetalTexture;
using temprenderer::core::math::WoodTexture;
using temprenderer::renderer::Camera;
using temprenderer::renderer::Light;
using temprenderer::renderer::PhongIntegrator;
using temprenderer::renderer::Plane;
using temprenderer::scene::Scene;
using temprenderer::scene::Sphere;

TEST_SUITE("MaterialRendering") {
  TEST_CASE("renders a metallic sphere above the existing wood floor") {
    Camera camera{
        "material-test", {0.0F, 0.0F, 0.0F}, 1.0F, 32, 24, 2.0F, 1.5F};
    Scene scene;
    scene.setBackgroundColor({0.02F, 0.02F, 0.02F});

    Material metal;
    metal.texture = std::make_shared<MetalTexture>(
        ColorF{0.68F, 0.72F, 0.8F}, ColorF{0.92F, 0.95F, 1.0F}, 1.0F);
    metal.metallic = 1.0F;
    metal.roughness = 0.24F;
    scene.addObject(std::make_shared<temprenderer::scene::Sphere>(
        kwp::Point3{0.0F, 0.0F, -5.0F}, 1.0F, metal));

    Material wood;
    wood.texture = std::make_shared<WoodTexture>(
        ColorF{0.72F, 0.46F, 0.22F}, ColorF{0.38F, 0.20F, 0.08F}, 18.0F, 0.8F);
    wood.ka = ColorF{0.12F, 0.08F, 0.05F};
    wood.ks = ColorF{0.03F, 0.03F, 0.03F};
    wood.alpha = 8.0F;
    scene.addObject(std::make_shared<Plane>(kwp::Point3{0.0F, -1.3F, 0.0F},
                                            kwp::Vec3{0.0F, 1.0F, 0.0F}, wood));

    scene.addLight(std::make_shared<Light>(
        "key", temprenderer::core::config::LightType::AREA,
        kwp::Point3{0.0F, 0.0F, -2.0F}, ColorF{1.0F, 1.0F, 1.0F}, 1.0F, 1.4F,
        1.0F));
    scene.addLight(std::make_shared<Light>(
        "ambient", temprenderer::core::config::LightType::AMBIENT,
        kwp::Point3{0.0F, 0.0F, 0.0F}, ColorF{0.2F, 0.2F, 0.2F}, 1.0F));

    PhongIntegrator integrator{camera, 32, 24, Color{5, 5, 5}, false};
    const auto image = integrator.render(scene);
    const Color spherePixel = image.get(16, 12);
    const Color floorPixel = image.get(16, 22);

    const bool sphereIsLit =
        spherePixel.r > 5 || spherePixel.g > 5 || spherePixel.b > 5;
    const bool floorIsVisible =
        floorPixel.r > 5 || floorPixel.g > 5 || floorPixel.b > 5;
    CHECK(sphereIsLit);
    CHECK(floorIsVisible);
    CHECK(floorPixel.r > floorPixel.b);
  }
}
