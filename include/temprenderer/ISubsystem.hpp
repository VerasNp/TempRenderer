#pragma once

namespace temprenderer {
class ISubsystem {
public:
  ISubsystem() = default;
  virtual ~ISubsystem() = default;

  virtual void startUp() = 0;
  virtual void shutDown() = 0;
};
} // namespace temprenderer
