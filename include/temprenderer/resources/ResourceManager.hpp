#pragma once

namespace temprenderer::resources {
class ResourceManager {
public:
  ResourceManager();
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) noexcept = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager &operator=(ResourceManager &&) noexcept = delete;
  ~ResourceManager();
};
} // namespace temprenderer::resources
