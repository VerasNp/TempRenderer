#include "temprenderer/ApplicationManager.hpp"

#include "renderer/PhongIntegrator.hpp"
#include "scene/SceneComposer.hpp"
#include "temprenderer/core/debug/ApplicationDebug.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/renderer/Sphere.hpp"

namespace temprenderer {

void ApplicationManager::startUp() {
  if (this->isApplicationInit_) {
    return;
  }
  LC_LOG(core::logging::LogLevel::INFO,
         "Starting up application manager, workers working...");
  this->windowManager_.startUp();
  const platform::graphics::WindowProps windowsProps{
      this->applicationConfig_.window.title,
      this->applicationConfig_.window.width,
      this->applicationConfig_.window.height,
  };
  if (!this->windowManager_.createWindow(windowsProps)) {
    LC_LOG(core::logging::LogLevel::ERROR, "Failed to create window");
    this->isApplicationInit_ = false;
    return;
  }
  this->editorManager_.setWindowManager(this->windowManager_);
  this->editorManager_.startUp();
  this->renderManager_.startUp();
  this->scene_ = scene::SceneComposer::compose(this->projectConfig_.scene,
                                               this->applicationConfig_.render);
  this->editorManager_.mainLayout().setOnRenderRequested([this]() {
    this->renderScene();
    this->editorManager_.renderResult().setTexture(
        this->renderManager_.getTextureId(),
        this->applicationConfig_.render.resolutionWidth,
        this->applicationConfig_.render.resolutionHeight);
    this->editorManager_.renderResult().open();
  });
  this->isApplicationInit_ = true;
}

void ApplicationManager::shutDown() {
  if (!this->isApplicationInit_) {
    return;
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO,
                 "Begin to shut down the application");
  this->renderManager_.shutDown();
  this->editorManager_.shutDown();
  this->windowManager_.shutDown();
  this->isApplicationInit_ = false;
}
void ApplicationManager::setApplicationConfig(
    const core::config::ApplicationConfig &config) {
  this->applicationConfig_ = config;
}

void ApplicationManager::renderScene() {
  renderer::PhongIntegrator integrator(
      *this->scene_.getCamera(),
      this->applicationConfig_.render.resolutionWidth,
      this->applicationConfig_.render.resolutionHeight,
      core::math::Color::fromFloat(this->scene_.getBackgroundColor()),
      this->applicationConfig_.render.shadows);
  renderer::Canvas canvas = integrator.render(this->scene_);
  this->renderManager_.setCanvas(canvas);
}

void ApplicationManager::run() {
  while (!this->windowManager_.shouldClose()) {
    this->windowManager_.update();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->editorManager_.beginFrame();
    this->editorManager_.endFrame();
    this->windowManager_.swapBuffers();
  }
}

} // namespace temprenderer
