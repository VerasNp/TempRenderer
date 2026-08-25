#pragma once

#include "temprenderer/ISubsystem.hpp"
#include "temprenderer/core/config/EngineConfig.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

namespace temprenderer::renderer {
class RendererManager final : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;

  RendererManager() {};
  virtual ~RendererManager() {};

  void setEngineConfig(const core::config::EngineConfig &config);

private:
  platform::graphics::WindowManager window_;
  core::config::EngineConfig config_;
};
} // namespace temprenderer::renderer
