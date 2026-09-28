#pragma once
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"
#include "renderer/Triangle.hpp"
#include "scene/Hittable.hpp"

#include <cstddef>
#include <functional>
#include <vector>

namespace temprenderer::scene {
struct HEdge;
struct HVertex;
struct HFace;

struct HEdge {
  HEdge *twin = nullptr;
  HEdge *next = nullptr;
  HEdge *prev = nullptr;
  HVertex *tip = nullptr;
  HFace *lFace = nullptr;

  [[nodiscard]] bool isBoundary() const noexcept { return lFace == nullptr; }
};

struct HVertex {
  kwp::Point3 position;
  HEdge *leaving = nullptr;
  std::size_t index = 0;
};

struct HFace {
  HEdge *edge = nullptr;
  std::size_t index = 0;
};

class Mesh : public Hittable {
public:
  Mesh(const std::vector<kwp::Point3> &rawVertices,
       const std::vector<std::size_t> &indices,
       const core::math::Material &material);

  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            SurfaceInteraction *isec) const noexcept override;

private:
  struct EdgeKey {
    std::size_t a, b;
    bool operator==(const EdgeKey &o) const noexcept {
      return a == o.a && b == o.b;
    }
  };
  struct EdgeKeyHash {
    std::size_t operator()(const EdgeKey &k) const noexcept {
      return std::hash<std::size_t>{}(k.a) ^
             (std::hash<std::size_t>{}(k.b) << 1);
    }
  };

  void linkTwins();
  [[nodiscard]] renderer::Triangle faceToTriangle(const HFace &f) const;

  std::vector<HEdge> edges_;
  std::vector<HVertex> vertices_;
  std::vector<HFace> faces_;
  std::vector<renderer::Triangle> triangles_;
  core::math::Material material_;
};
} // namespace temprenderer::scene
