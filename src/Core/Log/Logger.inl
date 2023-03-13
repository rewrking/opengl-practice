#include "Core/Log/Logger.hpp"

#include "Core/Types.hpp"

namespace ogl::priv
{
/*****************************************************************************/
template <typename Arg>
inline std::string Logger::logArgument(Arg&& arg) noexcept
{
	std::stringstream stream;

	using Type = std::decay_t<Arg>;
	if constexpr (std::is_same_v<Type, bool>)
	{
		stream.setf(std::ios_base::boolalpha);
	}
	else if constexpr (std::is_same_v<Type, f64> || std::is_same_v<Type, f32>)
	{
		stream.precision(std::numeric_limits<Type>::digits);
	}

	stream << std::forward<Arg>(arg) << " ";
	return stream.str();
}

/*****************************************************************************/
template <typename... Args>
inline void Logger::log(const LogLevel inLevel, Args&&... args)
{
	std::stringstream stream;
	((stream << logArgument(std::forward<Args>(args))), ...);
	_log(inLevel, stream.str());
}

}
