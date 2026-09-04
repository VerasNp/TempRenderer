#pragma once
#include "Hittable.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Light.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace temprenderer::scene {
class Scene : public Hittable {
public:
  void addObject(std::shared_ptr<Hittable> object) {
    this->objects_.push_back(std::shared_ptr(object));
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

  void addLight(const std::shared_ptr<renderer::Light> &light) {
    this->light_.push_back(light);
  };

  [[nodiscard]] std::shared_ptr<renderer::Light> getSpecificLightByType(
      const core::config::LightType &lightType) const noexcept {
    auto it =
        std::find_if(this->light_.begin(), this->light_.end(),
                     [lightType](const std::shared_ptr<renderer::Light> &l) {
                       return l.get()->getLightType() == lightType;
                     });
    return (it != this->light_.end()) ? *it : nullptr;
  }

private:
  std::vector<std::shared_ptr<renderer::Light>> light_;
  std::vector<std::shared_ptr<Hittable>> objects_;
};
} // namespace temprenderer::scene
