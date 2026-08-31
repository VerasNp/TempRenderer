#pragma once
#include "temprenderer/renderer/Canvas.hpp"
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::renderer {
/**
* @brief Interface for algorithms that resolve a scene into a
 * rendered image.
 */
class Integrator {
public:
  virtual ~Integrator() = default;
  /**
   * @brief Renders the given scene into a fully resolved Canvas.
   *
   * @param world The scene to render against
   * @return A Canvas containing the resolved color of every pixel.
   */
  [[nodiscard]] virtual Canvas render(const scene::Hittable &world) = 0;
};

} // namespace temprenderer::renderer
