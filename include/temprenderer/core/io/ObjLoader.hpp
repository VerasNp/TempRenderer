#pragma once

#include "core/config/ObjectConfig.hpp"

#include <string>

namespace temprenderer::io {

[[nodiscard]] core::config::MeshConfig
loadObjMeshConfig(const std::string &path);

} // namespace temprenderer::io
