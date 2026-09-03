#pragma once
#include "Hittable.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Light.hpp"

#include <memory>
#include <vector>

namespace temprenderer::scene {
class Scene : public Hittable {
public:
  void add(std::shared_ptr<Hittable> object) {
    objects_.push_back(std::shared_ptr(object));
  }

  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            SurfaceInteraction *isec) const noexcept override {
    bool hitAnything = false;
    kwp::Scalar closestSoFar = std::numeric_limits<kwp::Scalar>::max();
    SurfaceInteraction tempIsect{};

    for (const auto &obj : objects_) {
      if (obj->intersect(ray, &tempIsect)) {
        kwp::Scalar dist = (tempIsect.point - ray.getOrigin()).length();
        if (dist < closestSoFar) {
          closestSoFar = dist;
          *isec = tempIsect;
          hitAnything = true;
        }
      }
    }
    return hitAnything;
  }

  [[nodiscard]] std::shared_ptr<renderer::Light> getLight() const {
    return this->light_;
  }

  void setLight(const std::shared_ptr<renderer::Light> &light) {
    this->light_ = light;
  };

private:
  std::shared_ptr<renderer::Light> light_;
  std::vector<std::shared_ptr<Hittable>> objects_;
};
} // namespace temprenderer::scene
