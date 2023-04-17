#pragma once

#include <type_traits>

namespace ogl
{
template <class Derived, class Base>
concept base_of = std::is_base_of_v<Base, Derived>;
}