#pragma once
#include "core/logging/LoggerManager.hpp"
#include "temprenderer/scene/Mesh.hpp"
#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>

namespace temprenderer::core::io {

class MeshLoaderPort {
public:
  virtual ~MeshLoaderPort() = default;
  [[nodiscard]] virtual config::MeshConfig
  loadFile(const std::string &path) = 0;
};

class TinyobjloaderMeshAdapter final : public MeshLoaderPort {
public:
  [[nodiscard]] config::MeshConfig loadFile(const std::string &path) override {
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.triangulate = true;
    tinyobj::ObjReader reader;
    readerConfig.mtl_search_path = "";
    if (!reader.ParseFromFile(path, readerConfig)) {
      if (!reader.Error().empty()) {
        LC_LOG(core::logging::LogLevel::ERROR, reader.Error());
      }
      exit(1);
    }
    if (!reader.Warning().empty()) {
      LC_LOG(core::logging::LogLevel::WARNING, reader.Warning());
    }
    config::MeshConfig meshConfig;
    meshConfig.path = path;
    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();
    if (attrib.vertices.size() % 3 != 0) {
      LC_LOG(logging::LogLevel::ERROR,
             "Vertex array size is not a multiple of 3");
      exit(1);
    }
    const std::size_t vertexCount = attrib.vertices.size() / 3;
    std::vector<kwp::Point3> allVertices;
    allVertices.reserve(vertexCount);
    for (std::size_t i = 0; i < vertexCount; ++i) {
      allVertices.push_back(kwp::Point3{attrib.vertices[3 * i + 0],
                                        attrib.vertices[3 * i + 1],
                                        attrib.vertices[3 * i + 2]});
    }

    meshConfig.meshes.reserve(shapes.size());
    for (const auto &shape : shapes) {
      config::ShapeMeshConfig shapeMeshConfig;
      shapeMeshConfig.name = shape.name;
      shapeMeshConfig.vertices = allVertices;
      std::size_t indexOffset = 0;
      for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
        const std::size_t fv = shape.mesh.num_face_vertices[f];
        std::vector<std::size_t> faceVertices;
        faceVertices.reserve(fv);
        for (std::size_t v = 0; v < fv; ++v) {
          const tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
          faceVertices.push_back(static_cast<std::size_t>(idx.vertex_index));
        }
        shapeMeshConfig.faces.push_back(std::move(faceVertices));
        indexOffset += fv;
      }
      meshConfig.meshes.emplace_back(std::move(shapeMeshConfig));
    }
    return meshConfig;
  }
};

} // namespace temprenderer::core::io
