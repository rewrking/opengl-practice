#pragma once

#include "Libraries/GLM.hpp"

namespace ogl
{
// constexpr size_t kMaxBoneInfluence = 4;
struct Vertex3D
{
	glm::vec3 position;
	glm::vec3 normal;

	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	// std::array<i32, kMaxBoneInfluence> m_boneIDs;
	// std::array<f32, kMaxBoneInfluence> m_weights;
};
}
