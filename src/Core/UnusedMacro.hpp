#pragma once

namespace ogl::priv
{
template <typename... Args>
constexpr void unused(Args&&... args)
{
	(static_cast<void>(std::forward<Args>(args)), ...);
}
}

#define UNUSED(...) ogl::priv::unused(__VA_ARGS__)
