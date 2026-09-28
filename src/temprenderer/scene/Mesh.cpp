#include "scene/Mesh.hpp"

#include <stdexcept>

namespace temprenderer::scene {

Mesh::Mesh(const std::vector<kwp::Point3> &rawVertices,
           const std::vector<std::size_t> &indices,
           const core::math::Material &material) {
  const std::size_t triangleCount = indices.size() / 3;
  this->material_ = material;
  this->vertices_.reserve(rawVertices.size());
  for (std::size_t i = 0; i < rawVertices.size(); ++i) {
    HVertex vertex;
    vertex.index = i;
    vertex.position = rawVertices[i];
    this->vertices_.push_back(vertex);
  }
  this->faces_.reserve(triangleCount);
  this->edges_.reserve(indices.size() * 2);
  for (std::size_t t = 0; t < triangleCount; ++t) {
    const std::size_t i0 = indices[3 * t + 0];
    const std::size_t i1 = indices[3 * t + 1];
    const std::size_t i2 = indices[3 * t + 2];
    HFace face;
    face.index = t;
    this->faces_.push_back(face);
    HFace *facePtr = &this->faces_.back();
    const std::size_t base = this->edges_.size();
    this->edges_.push_back(HEdge{});
    this->edges_.push_back(HEdge{});
    this->edges_.push_back(HEdge{});
    HEdge *e0 = &this->edges_[base + 0];
    HEdge *e1 = &this->edges_[base + 1];
    HEdge *e2 = &this->edges_[base + 2];
    e0->tip = &this->vertices_[i1];
    e1->tip = &this->vertices_[i2];
    e2->tip = &this->vertices_[i0];
    e0->next = e1;
    e1->next = e2;
    e2->next = e0;
    e0->prev = e2;
    e1->prev = e0;
    e2->prev = e1;
    e0->lFace = e1->lFace = e2->lFace = facePtr;
    facePtr->edge = e0;
    this->vertices_[i0].leaving = e0;
    this->vertices_[i1].leaving = e1;
    this->vertices_[i2].leaving = e2;
  }
  this->linkTwins();
  this->triangles_.reserve(this->faces_.size());
  for (const HFace &f : this->faces_) {
    this->triangles_.push_back(this->faceToTriangle(f));
  }
}

renderer::Triangle Mesh::faceToTriangle(const HFace &f) const {
  const HEdge *e0 = f.edge;
  const HVertex *v0 = e0->prev->tip;
  const HVertex *v1 = e0->tip;
  const HVertex *v2 = e0->next->tip;
  renderer::Triangle tri(v0->position, v1->position, v2->position,
                         this->material_);
  tri.setCullBackfaces(true);
  return tri;
}

bool Mesh::intersect(const core::math::Ray &ray,
                     SurfaceInteraction *isec) const noexcept {
  bool hitAnything = false;
  kwp::Scalar closestSoFar = std::numeric_limits<kwp::Scalar>::max();
  SurfaceInteraction tempIsect{};
  for (const auto &tri : this->triangles_) {
    if (tri.intersect(ray, &tempIsect)) {
      const kwp::Scalar dist = (tempIsect.point - ray.getOrigin()).length();
      if (dist < closestSoFar) {
        closestSoFar = dist;
        *isec = tempIsect;
        hitAnything = true;
      }
    }
  }
  return hitAnything;
}

void Mesh::linkTwins() {
  std::unordered_map<EdgeKey, HEdge *, EdgeKeyHash> edgeMap;
  edgeMap.reserve(this->edges_.size() * 2);
  const std::size_t interiorCount = this->edges_.size();
  for (std::size_t i = 0; i < interiorCount; ++i) {
    HEdge &e = this->edges_[i];
    const std::size_t originIdx = e.prev->tip->index;
    const std::size_t destIdx = e.tip->index;
    edgeMap[EdgeKey{.a = originIdx, .b = destIdx}] = &e;
  }
  for (std::size_t i = 0; i < interiorCount; ++i) {
    HEdge &e = this->edges_[i];
    if (e.twin != nullptr) {
      continue;
    }
    const std::size_t originIdx = e.prev->tip->index;
    const std::size_t destIdx = e.tip->index;
    if (auto it = edgeMap.find(EdgeKey{.a = destIdx, .b = originIdx});
        it != edgeMap.end()) {
      e.twin = it->second;
      it->second->twin = &e;
    } else {
      HEdge boundary;
      boundary.tip = &this->vertices_[originIdx];
      boundary.lFace = nullptr;
      this->edges_.push_back(boundary);
      HEdge *boundaryPtr = &this->edges_.back();
      e.twin = boundaryPtr;
      boundaryPtr->twin = &e;
    }
  }
}

} // namespace temprenderer::scene
