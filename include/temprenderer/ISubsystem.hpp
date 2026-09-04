#pragma once

namespace temprenderer {
/**
 * @brief Lifecycle contract for engine subsystems
 */
class ISubsystem {
public:
  ISubsystem() = default;
  virtual ~ISubsystem() = default;

  /**
   * @brief Acquire external resources and config the subsystem
   */
  virtual void startUp() = 0;
  /**
   * @brief Free acquired resources
   */
  virtual void shutDown() = 0;
};
} // namespace temprenderer
