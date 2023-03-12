#include "Core/Helpers.hpp"

namespace ogl
{
/*****************************************************************************/
bool String::endsWith(const std::string_view inEnd, const std::string& inString)
{
	if (inEnd.size() > inString.size())
		return false;

	return std::equal(inEnd.rbegin(), inEnd.rend(), inString.rbegin());
}

}
