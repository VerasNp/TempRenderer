#pragma once
#include "Scene.hpp"

namespace temprenderer::scene {
struct SceneComposer {
  [[nodiscard]] static Scene
  compose(const core::config::SceneConfig &sceneConfig);
};
} // namespace temprenderer::scene
