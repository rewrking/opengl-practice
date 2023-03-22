#pragma once

#if defined(OGL_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4201)
#else
#endif

#if defined(OGL_MSVC) && defined(OGL_CLANG)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
	#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

#include <glm/glm.hpp>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
// #include <glm/mat2x2.hpp>
// #include <glm/mat2x3.hpp>
// #include <glm/mat4x4.hpp>
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#if defined(OGL_MSVC)
	#pragma warning(pop)
#else
#endif

#if defined(OGL_MSVC) && defined(OGL_CLANG)
	#pragma clang diagnostic pop
#endif

namespace ogl
{
using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Mat4f = glm::mat4;
}
