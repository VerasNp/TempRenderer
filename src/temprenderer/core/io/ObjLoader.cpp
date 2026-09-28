#include "core/config/ObjectConfig.hpp"

#include <fstream>
#include <sstream>

namespace temprenderer::io {

namespace {

std::size_t parseVertexIndex(const std::string &token,
                             const std::size_t vertexCountSoFar) {
  const std::size_t slash = token.find('/');
  const std::string posStr =
      (slash == std::string::npos) ? token : token.substr(0, slash);

  long idx = std::stol(posStr);
  if (idx < 0) {
    idx = static_cast<long>(vertexCountSoFar) + idx + 1;
  }
  return static_cast<std::size_t>(idx - 1);
}

} // namespace

core::config::MeshConfig loadObjMeshConfig(const std::string &path) {
  std::ifstream file(path);
  core::config::MeshConfig meshConfig;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    if (tag == "v") {
      kwp::Scalar x;
      kwp::Scalar y;
      kwp::Scalar z;
      iss >> x >> y >> z;
      meshConfig.vertices.emplace_back(
          static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    } else if (tag == "f") {
      std::vector<int> face;
      std::string token;
      while (iss >> token) {
        face.push_back(static_cast<int>(
            parseVertexIndex(token, meshConfig.vertices.size())));
      }
      meshConfig.faces.push_back(std::move(face));
    }
  }
  return meshConfig;
}

} // namespace temprenderer::io
