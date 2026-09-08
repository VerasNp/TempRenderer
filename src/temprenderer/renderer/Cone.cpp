#include "renderer/Cone.hpp"
namespace temprenderer::renderer {
Cone::Cone(const kwp::Point3 &baseCenter, float baseRadius, kwp::Point3 vertex,
           core::math::Material material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->vertex_ = vertex;
  this->material_ = material;
  this->direction_ = (this->vertex_ - this->baseCenter_).normalize();
  this->height_ = (this->vertex_ - this->baseCenter_).length();
}

bool Cone::intersect(const core::math::Ray &ray,
                     scene::SurfaceInteraction *isect) const noexcept {
  kwp::Scalar a = ra
}

} // namespace temprenderer::renderer
