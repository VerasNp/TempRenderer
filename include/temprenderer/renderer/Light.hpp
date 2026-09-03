#pragma once
#include "kwp/Point3.hpp"

namespace temprenderer::renderer {
class Light {
public:
  Light(kwp::Point3 position) : position_(position) {}
  ~Light() = default;
  [[nodiscard]] kwp::Point3 getPosition() const noexcept {
    return this->position_;
  }

private:
  kwp::Point3 position_;
};
} // namespace temprenderer::renderer
