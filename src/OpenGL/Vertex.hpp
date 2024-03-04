#pragma once

#include "OpenGL/GLM.hpp"

namespace ogl
{
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
}
