#pragma once
#include "Scene.hpp"

namespace temprenderer::scene {
struct SceneComposer {
  [[nodiscard]] static Scene
  compose(const core::config::SceneConfig &sceneConfig,
          const core::config::RenderConfig &renderConfig);
};
} // namespace temprenderer::scene
