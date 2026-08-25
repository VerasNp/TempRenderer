#pragma once

#include "temprenderer/ISubsystem.hpp"
#include "temprenderer/core/config/EngineConfig.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

namespace temprenderer::renderer {
class RendererManager : public ISubsystem {
public:
  RendererManager() = default;
  ~RendererManager() override = default;

  void startUp() override;
  void shutDown() override;

  void setEngineConfig(const core::config::EngineConfig &config);
  void run() const;

private:
  platform::graphics::WindowManager window_;
  core::config::EngineConfig config_{};
  bool isRendererInit_ = false;
  bool configSet_ = false;
};
} // namespace temprenderer::renderer
